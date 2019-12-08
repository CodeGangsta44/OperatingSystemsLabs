package generator.task;

import entity.Task;
import lombok.*;

import java.util.concurrent.ThreadLocalRandom;

@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class TaskGenerator {
    private int minTaskDuration;
    private int maxTaskDuration;

    private int minCreationInterval;
    private int maxCreationInterval;

    private int minTaskPriority;
    private int maxTaskPriority;

    private int lastCreationTime;
    private int lastGeneratedId;

    public void reset() {
        lastCreationTime = 0;
        lastGeneratedId = 0;
    }

    public Task generateNextTask() {
        int creationTime = lastCreationTime +
                ThreadLocalRandom.current().nextInt(minCreationInterval, maxCreationInterval + 1);
        int duration = ThreadLocalRandom.current().nextInt(minTaskDuration, maxTaskDuration + 1);
        int priority = ThreadLocalRandom.current().nextInt(minTaskPriority, maxTaskPriority + 1);

        lastCreationTime = creationTime;

        return Task.builder()
                .id(++lastGeneratedId)
                .creationTime(creationTime)
                .duration(duration)
                .remainingTime(duration)
                .priority(priority)
                .waitingTime(0)
                .build();
    }
}
