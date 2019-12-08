import chart.LineChartEx;
import generator.dataset.AverageDowntimeByCreationIntervalDatasetGenerator;
import generator.dataset.AverageWaitingTimeByCreationIntervalDatasetGenerator;
import generator.dataset.AverageWaitingTimeByPriorityDatasetGenerator;

public class Main {
    public static void main(String[] args) {
        AverageWaitingTimeByPriorityDatasetGenerator averageWaitingTimeByPriorityDatasetGenerator =
                new AverageWaitingTimeByPriorityDatasetGenerator();
//        AverageWaitingTimeByCreationIntervalDatasetGenerator averageWaitingTimeByCreationIntervalDatasetGenerator =
//                new AverageWaitingTimeByCreationIntervalDatasetGenerator();
//        AverageDowntimeByCreationIntervalDatasetGenerator averageDowntimeByCreationIntervalDatasetGenerator =
//                new AverageDowntimeByCreationIntervalDatasetGenerator();

        LineChartEx lineChartEx1 = new LineChartEx(averageWaitingTimeByPriorityDatasetGenerator.getDataset(),
                "Час очікування", "Пріоритет", "Залежність часу очікування від пріоритету");

//        LineChartEx lineChartEx2 = new LineChartEx(averageWaitingTimeByCreationIntervalDatasetGenerator.getDataset(),
//                "Час очікування", "Інтервал", "Залежність часу очікування від інтервалу");
//
//        LineChartEx lineChartEx3 = new LineChartEx(averageDowntimeByCreationIntervalDatasetGenerator.getDataset(),
//                "Час простою", "Інтервал", "Залежність часу простою від інтервалу");

        lineChartEx1.setVisible(true);
//        lineChartEx2.setVisible(true);
//        lineChartEx3.setVisible(true);
    }
}
