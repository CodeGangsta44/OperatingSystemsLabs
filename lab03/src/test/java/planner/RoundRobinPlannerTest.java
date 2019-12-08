package planner;

import entity.Task;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class RoundRobinPlannerTest {

    private List<Task> tasksToDo = new ArrayList<>();
    private List<Task> expectedResult = new ArrayList<>();

    private static final int QUANTUM = 1;

    @Before
    public void setUp() {
        Task task1 = Task.builder()
                .id(1)
                .creationTime(10)
                .duration(40)
                .remainingTime(40)
                .priority(5)
                .waitingTime(0)
                .isFinished(false)
                .build();

        Task task2 = Task.builder()
                .id(2)
                .creationTime(10)
                .duration(45)
                .remainingTime(45)
                .priority(0)
                .waitingTime(0)
                .isFinished(false)
                .build();

        Task task3 = Task.builder()
                .id(3)
                .creationTime(10)
                .duration(10)
                .remainingTime(10)
                .priority(3)
                .waitingTime(0)
                .isFinished(false)
                .build();

        Task task4 = Task.builder()
                .id(4)
                .creationTime(10)
                .duration(20)
                .remainingTime(20)
                .priority(7)
                .waitingTime(0)
                .isFinished(false)
                .build();

        tasksToDo.addAll(Arrays.asList(task1, task2, task3, task4));
        expectedResult.addAll(Arrays.asList(task2, task3, task1, task4));

    }

    @Test
    public void execute() {
        RoundRobinPlanner planner = new RoundRobinPlanner(QUANTUM, tasksToDo);
        List<Task> actualResult = planner.execute();

        for (int i = 0; i < expectedResult.size(); i++) {
            Assert.assertEquals(expectedResult.get(i), actualResult.get(i));
        }

        System.out.println(actualResult);
    }
}