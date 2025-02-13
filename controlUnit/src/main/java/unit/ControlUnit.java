package unit;

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
    private Observer msgObserver;
    private Observer timerObserver;

    public ControlUnit(DataStore dataStore) {
        this.dataStore = dataStore;
        this.state = State.NORMAL;
        this.mode = Mode.AUTOMATIC;
        this.isStateChanged = false;
        this.windowPosition = 0;
        this.msgObserver = (msg -> {}) ;
        this.timerObserver = (event -> {}) ;
    }

    public void setMode(String newMode, Boolean notifyMode) {
        if (newMode.equals(ControlUnit.Mode.MANUAL.toString())) {
            mode = ControlUnit.Mode.MANUAL;
        } else if (newMode.equals(ControlUnit.Mode.AUTOMATIC.toString())) {
            mode = ControlUnit.Mode.AUTOMATIC;
            updateWindowPosition(dataStore.getTemperature());
        }
        if (notifyMode) {
            msgObserver.notifyEvent("mode_" + mode.toString());
        }
    }

    public void setWindowPositionManually(int position, Boolean notifyWindow) {
        if (mode == Mode.MANUAL) { // Solo in manuale si può cambiare la finestra
            setWindowPosition(position,notifyWindow);
        }
    }

    private void setWindowPosition(int newPos, Boolean notifyWindow) {
        if (newPos != this.windowPosition) {
            this.windowPosition = newPos;
            if (notifyWindow) {
                msgObserver.notifyEvent("position_" + String.valueOf(newPos));
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

    public void saveTemperature(double newTemp) {
        double oldTemp = dataStore.getTemperature();
        dataStore.saveTemperatureData(newTemp, mode.toString());

        if (oldTemp != newTemp) {
            updateState(newTemp);
            updateWindowPosition(newTemp);
            msgObserver.notifyEvent("temperature_"+String.valueOf(newTemp));
        }
    }

    public void addSerialMsgObserver(Observer ob) {
        this.msgObserver = ob;
    }

    public void addTimerObserver(Observer ob) {
        this.timerObserver = ob;
    }

    public void notifyTimer() {
        if (state == State.TOO_HOT) {
            setState(State.ALARM);
        }
    }

    public void resetAlarm() {
        setState(State.NORMAL);
    }

    private void updateState(double temp) {
        if (state != State.ALARM) {
            setState(State.getState(temp));
        }
    }

    private void setState(State newState) {
        isStateChanged = newState != state;
        if (isStateChanged && newState == State.TOO_HOT) {
            timerObserver.notifyEvent(null);
        }
        state = newState;
    }

    private void updateWindowPosition(double temp) {
        if (mode == Mode.AUTOMATIC) {
            switch (state) {
                case NORMAL: setWindowPosition(0, true);
                    break;
                case TOO_HOT: setWindowPosition(100, true);
                    break;
                case HOT: setWindowPosition(posPercent(temp), true);
                    break;
                case ALARM :
                    break;
            }
        }
    }

    private static int posPercent(double temp) {
        return (int) ((temp - State.NORMAL.getLimit())*100 / (State.HOT.getLimit() - State.NORMAL.getLimit()));
    }


}
