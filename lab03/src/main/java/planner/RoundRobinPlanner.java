package planner;

import entity.Task;
import lombok.Getter;
import lombok.Setter;

import java.util.*;

@Getter
@Setter
public class RoundRobinPlanner {
    private int quantumSize;
    private int currentTime;
    private List<Task> futureTasks;
    private Map<Integer, List<Task>> queues;
    private List<Task> finishedTasks;
    private boolean isDowntime;
    private int downtimeCount;

    public RoundRobinPlanner(Integer quantumSize, List<Task> tasks) {
        this.quantumSize = quantumSize;
        this.currentTime = 0;
        this.futureTasks = tasks;
        this.queues = new HashMap<>();
        this.finishedTasks = new ArrayList<>();
        this.isDowntime = true;
        this.downtimeCount = 0;

        this.refreshQueues();
    }

    public List<Task> execute() {
        while (futureTasks.size() > 0 || getPendingTasksCount() > 0) {
            Task currentTask = findNextTask();
            isDowntime = Objects.isNull(currentTask);

            for (int i = 0; i < quantumSize; i++) {
                currentTime++;
                incrementWaitingTimeForPendingTasks();
                refreshQueues();
                if (isDowntime) {
                    downtimeCount++;
                    break;
                } else {
                    currentTask.giveTic();

                    if (currentTask.isFinished()) {
                        break;
                    }
                }
            }

            if (isDowntime) {
                continue;
            }

            if (!currentTask.isFinished()) {
                putTask(currentTask);
            } else {
                finishedTasks.add(currentTask);
            }
        }

        return finishedTasks;
    }

    private void refreshQueues() {
        List<Task> tasksToEvict = new ArrayList<>();
        futureTasks.stream()
                .filter(task -> task.getCreationTime() <= currentTime)
                .forEach(task -> {
                    List<Task> list = Optional
                            .ofNullable(queues
                                    .get(task.getPriority()))
                            .orElseGet(ArrayList::new);
                    list.add(task);

                    tasksToEvict.add(task);
                    queues.putIfAbsent(task.getPriority(), list);
                });

        futureTasks.removeAll(tasksToEvict);
    }

    private Task findNextTask() {
        return queues.entrySet().stream()
                .filter(integerListEntry -> !integerListEntry.getValue().isEmpty())
                .min(Comparator.comparingInt(Map.Entry::getKey))
                .map(integerListEntry -> integerListEntry.getValue().remove(0))
                .orElse(null);
    }

    private void putTask(Task task) {
        List<Task> list = Optional
                .ofNullable(queues
                        .get(task.getPriority()))
                .orElseGet(ArrayList::new);
        list.add(task);
        queues.putIfAbsent(task.getPriority(), list);
    }

    private long getPendingTasksCount() {
        return queues.values().stream()
                .mapToInt(List::size)
                .sum();
    }

    private void incrementWaitingTimeForPendingTasks() {
        queues.values()
                .stream()
                .flatMap(Collection::stream)
                .forEach(Task::waitTic);
    }
}
