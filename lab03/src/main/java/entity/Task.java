package entity;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class Task {
    private int id;
    private int priority;
    private int creationTime;
    private int duration;
    private int remainingTime;

    private int waitingTime;

    private boolean isFinished;

    public void giveTic() {
        remainingTime--;
        if (remainingTime == 0) {
            isFinished = true;
        }
    }

    public void waitTic() {
        waitingTime++;
    }
}
