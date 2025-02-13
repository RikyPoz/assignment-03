package unit;

public class TimerService extends VerticleService {
    private long timerId;
    public TimerService(ControlUnit controlUnit) {
        super(controlUnit);
        timerId = -1;
    }

    @Override
    public void start() {
        controlUnit.addTimerObserver((m) -> startTimer());
    }

    private void startTimer() {
        if (timerId != -1) {
            vertx.cancelTimer(timerId);
        }
        timerId = vertx.setTimer(10000, id -> {
            controlUnit.notifyTimer();
            timerId = -1;
        });
    }

}
