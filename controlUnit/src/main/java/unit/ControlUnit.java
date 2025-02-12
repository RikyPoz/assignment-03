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
        this.observer = (msg -> {}) ;
    }

    public void setMode(String newMode, Boolean notifyMode) {
        if (newMode.equals(ControlUnit.Mode.MANUAL.toString())) {
            mode = ControlUnit.Mode.MANUAL;
        } else if (newMode.equals(ControlUnit.Mode.AUTOMATIC.toString())) {
            mode = ControlUnit.Mode.AUTOMATIC;
            calculateWindowPosition(true);
        }
        if (notifyMode) {
            observer.notifyEvent("mode_" + mode.toString());
        }
        System.out.println("Modalità cambiata a: " + mode);
    }

    public void setWindowPositionManually(int position, Boolean notifyWindow) {
        if (mode == Mode.MANUAL) { // Solo in manuale si può cambiare la finestra
            setWindowPosition(position,notifyWindow);
        }
    }

    private void setWindowPosition(int pos, Boolean notifyWindow) {
        this.windowPosition = pos;
        //System.out.println("Finestra impostata al " + pos + "%");
        if (notifyWindow) {
            observer.notifyEvent("position_" + String.valueOf(pos));
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

    public void saveTemperature(double newTemp) {
        double oldTemp = dataStore.getTemperature();
        dataStore.saveTemperatureData(newTemp);

        if (oldTemp != newTemp) {
            calculateWindowPosition(false);
        }
        
    }

    public void addSerialMsgObserver(Observer ob) {
        this.observer = ob;
    }

    private void calculateWindowPosition(Boolean modeChanged) {
        final double temp = dataStore.getTemperature();
        State newState = Arrays.stream(State.values()).filter(s -> temp < s.getLimit()).findFirst().get();
        isStateChanged = state != newState;
        if (isStateChanged() || modeChanged) {
            state = newState;
            if (mode == Mode.AUTOMATIC) {
                if (state == State.NORMAL) {
                    setWindowPosition(0, true);
                } else if (state == State.TOO_HOT) {
                    setWindowPosition(100, true);
                }
            }
        }
        if (mode == Mode.AUTOMATIC && state == State.HOT) {
            final int pos = (int) ((temp - State.NORMAL.getLimit()) * 100
                    / (State.HOT.getLimit() - State.NORMAL.getLimit()));
            setWindowPosition(pos, true);
        }
        observer.notifyEvent("temperature_"+String.valueOf(temp));
    }
}
