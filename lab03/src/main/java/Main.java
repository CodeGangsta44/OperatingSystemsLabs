import chart.LineChartEx;
import generator.dataset.AverageDowntimeByCreationIntervalDatasetGenerator;
import generator.dataset.AverageWaitingTimeByCreationIntervalDatasetGenerator;
import generator.dataset.AverageWaitingTimeByPriorityDatasetGenerator;

public class Main {
    public static void main(String[] args) {
        int minTaskDuration = 5;
        int maxTaskDuration = 100;
        int minTaskPriority = 0;
        int maxTaskPriority = 32;
        int minCreationInterval = 5;
        int maxCreationInterval = 100;


        AverageWaitingTimeByPriorityDatasetGenerator averageWaitingTimeByPriorityDatasetGenerator =
                AverageWaitingTimeByPriorityDatasetGenerator.builder()
                .minTaskDuration(minTaskDuration)
                .maxTaskDuration(maxTaskDuration)
                .minTaskPriority(minTaskPriority)
                .maxTaskPriority(maxTaskPriority)
                .minCreationInterval(minCreationInterval)
                .maxCreationInterval(maxCreationInterval)
                .build();

        averageWaitingTimeByPriorityDatasetGenerator.init();


        AverageWaitingTimeByCreationIntervalDatasetGenerator averageWaitingTimeByCreationIntervalDatasetGenerator =
                AverageWaitingTimeByCreationIntervalDatasetGenerator.builder()
                        .minTaskDuration(minTaskDuration)
                        .maxTaskDuration(maxTaskDuration)
                        .minTaskPriority(minTaskPriority)
                        .maxTaskPriority(maxTaskPriority)
                        .minCreationInterval(minCreationInterval)
                        .maxCreationInterval(maxCreationInterval)
                        .build();
        averageWaitingTimeByCreationIntervalDatasetGenerator.init();

        AverageDowntimeByCreationIntervalDatasetGenerator averageDowntimeByCreationIntervalDatasetGenerator =
                AverageDowntimeByCreationIntervalDatasetGenerator.builder()
                        .minTaskDuration(minTaskDuration)
                        .maxTaskDuration(maxTaskDuration)
                        .minTaskPriority(minTaskPriority)
                        .maxTaskPriority(maxTaskPriority)
                        .minCreationInterval(minCreationInterval)
                        .maxCreationInterval(maxCreationInterval)
                        .build();

        averageDowntimeByCreationIntervalDatasetGenerator.init();

        LineChartEx lineChartEx1 = new LineChartEx(averageWaitingTimeByPriorityDatasetGenerator.getDataset(),
                "Час очікування", "Пріоритет", "Залежність часу очікування від пріоритету");

        LineChartEx lineChartEx2 = new LineChartEx(averageWaitingTimeByCreationIntervalDatasetGenerator.getDataset(),
                "Час очікування", "Інтервал", "Залежність часу очікування від інтервалу");

        LineChartEx lineChartEx3 = new LineChartEx(averageDowntimeByCreationIntervalDatasetGenerator.getDataset(),
                "Час простою", "Інтервал", "Залежність часу простою від інтервалу");

        lineChartEx1.setVisible(true);
        lineChartEx2.setVisible(true);
        lineChartEx3.setVisible(true);
    }
}
