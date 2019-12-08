package generator.dataset;

import entity.Task;
import generator.task.TaskGenerator;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import planner.RoundRobinPlanner;

import java.util.*;

public class AverageWaitingTimeByCreationIntervalDatasetGenerator {
    private Map<Integer, Double> result;

    public AverageWaitingTimeByCreationIntervalDatasetGenerator() {
        result = new HashMap<>();
        init();
    }

    private double getAverageWaitingTimeByInterval(int interval) {
        List<Task> tasksToDo = new ArrayList<>();
        TaskGenerator taskGenerator = TaskGenerator.builder()
                .minTaskDuration(50)
                .maxTaskDuration(150)
                .minTaskPriority(0)
                .maxTaskPriority(32)
                .minCreationInterval(interval)
                .maxCreationInterval(interval)
                .build();

        taskGenerator.reset();

        for (int i = 0; i < 1000; i++) {
            tasksToDo.add(taskGenerator.generateNextTask());
        }

        RoundRobinPlanner roundRobinPlanner = new RoundRobinPlanner(25, tasksToDo);

        List<Task> result = roundRobinPlanner.execute();

        OptionalDouble avg = result.stream()
                .mapToDouble(Task::getWaitingTime)
                .average();

        return avg.orElse(0);
    }

    private void init() {
        for (int i = 1; i <= 100; i++) {
            result.put(i, getAverageWaitingTimeByInterval(i));
        }
    }

    public XYDataset getDataset() {
        XYSeries series = new XYSeries("Залежність");

        result.forEach(series::add);

        XYSeriesCollection dataset = new XYSeriesCollection();
        dataset.addSeries(series);

        return dataset;
    }
}
