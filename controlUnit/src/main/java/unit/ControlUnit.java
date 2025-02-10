package unit;

import java.util.Arrays;

import io.vertx.core.json.JsonObject;

public class ControlUnit {
    public enum Mode {
        AUTOMATIC, MANUAL;
    }

    private final DataStore dataStore;
    private int windowPosition; // Posizione finestra (0 = chiusa, 100 = aperta)
    private State state;
    private Mode mode;
    private Boolean isStateChanged;
    private Observer observer;

    public ControlUnit(DataStore dataStore) {
        this.dataStore = dataStore;
        this.state = State.NORMAL;
        this.mode = Mode.AUTOMATIC;
        this.isStateChanged = false;
        this.windowPosition = 0;
    }

    public void setMode(String newMode, Boolean notifyMode) {
        if (newMode.equals(ControlUnit.Mode.MANUAL.toString())) {
            mode = ControlUnit.Mode.MANUAL;
        } else if (newMode.equals(ControlUnit.Mode.AUTOMATIC.toString())) {
            mode = ControlUnit.Mode.AUTOMATIC;
        }
        if (notifyMode) {
            observer.notifyEvent("mode_" + mode.toString());
        }
        System.out.println("Modalità cambiata a: " + mode);
    }

    public void setWindowPosition(int position, Boolean notifyWindow) {
        if (mode == Mode.MANUAL) { // Solo in manuale si può cambiare la finestra
            this.windowPosition = position;
            System.out.println("Finestra impostata al " + position + "%");
            if (notifyWindow) {
                observer.notifyEvent("position_" + String.valueOf(position));
            }
        }
    }

    public JsonObject getLatestData() {
        return dataStore.getLatestData(state.toString(), getWindowPosition());
    }

    public JsonObject getLatestNtemperature(int n) {
        return dataStore.getLatestNtemperature(n);
    }

    public boolean isStateChanged() {
        return isStateChanged;
    }

    public int getEspPeriod() {
        return state.getPeriod();
    }

    public int getWindowPosition() {
        return windowPosition;
    }

    public void saveTemperature(double temperature) {
        dataStore.saveTemperatureData(temperature);
        State newState = Arrays.stream(State.values()).filter(s -> temperature < s.getLimit()).findFirst().get();
        isStateChanged = state != newState;
        if (isStateChanged()) {
            state = newState;
            System.out.println(state);
            if (mode == Mode.AUTOMATIC) {
                if (state == State.NORMAL) {
                    windowPosition = 0;
                } else if (state == State.TOO_HOT) {
                    windowPosition = 100;
                } else {
                    windowPosition = (int) ((temperature - State.NORMAL.getLimit()) * 100
                            / (State.HOT.getLimit() - State.NORMAL.getLimit()));
                }
            }
        }
    }

    public void addSerialMsgObserver(Observer ob) {
        this.observer = ob;
    }
}
