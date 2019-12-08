package generator.dataset;

import entity.Task;
import generator.task.TaskGenerator;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import planner.RoundRobinPlanner;

import java.util.*;

public class AverageDowntimeByCreationIntervalDatasetGenerator {
    private Map<Integer, Double> result;

    public AverageDowntimeByCreationIntervalDatasetGenerator() {
        result = new HashMap<>();
        init();
    }

    private double getDowntimePercentByInterval(int interval) {
        List<Task> tasksToDo = new ArrayList<>();
        TaskGenerator taskGenerator = TaskGenerator.builder()
                .minTaskDuration(5)
                .maxTaskDuration(95)
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

        int totalTime = roundRobinPlanner.getCurrentTime();
        int downtime = roundRobinPlanner.getDowntimeCount();

        return (((double) downtime) / ((double) totalTime)) * 100;
    }

    private void init() {
        for (int i = 1; i <= 100; i++) {
            result.put(i, getDowntimePercentByInterval(i));
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
