package unit;
import java.util.Arrays;

public enum State {

    NORMAL(2000, 20),
    HOT(1000, 26),
    TOO_HOT(500, 9999),
    ALARM(-1, -1);

    private final int period;
    private final double limit;
    private State(int period, double sup) {
        this.period = period;
        this.limit = sup;
    }

    public int getPeriod() {
        return period;
    }

    public double getLimit() {
        return this.limit;
    }

    public static State getState(double temp) {
        return Arrays.stream(State.values()).filter(s -> temp < s.getLimit()).findFirst().get();
    }
}