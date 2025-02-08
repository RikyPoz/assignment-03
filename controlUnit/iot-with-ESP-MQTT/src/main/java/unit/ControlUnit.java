package unit;

import io.vertx.core.json.JsonObject;

public class ControlUnit {
    private final DataStore dataStore;
    private String mode = "AUTO"; // Modalità di default
    private double windowPosition = 0.0; // Posizione finestra (0 = chiusa, 100 = aperta)

    public ControlUnit(DataStore dataStore) {
        this.dataStore = dataStore;
    }

    public JsonObject getSystemStatus() {
        JsonObject status = new JsonObject()
                .put("mode", mode)
                .put("windowPosition", windowPosition)
                .put("latestTemperature", dataStore.getLatestData());
        return status;
    }

    public void setMode(String newMode) {
        if (newMode.equals("AUTO") || newMode.equals("MANUAL")) {
            this.mode = newMode;
            System.out.println("Modalità cambiata a: " + mode);
        }
    }

    public void setWindowPosition(double position) {
        if (mode.equals("MANUAL")) { // Solo in manuale si può cambiare la finestra
            this.windowPosition = position;
            System.out.println("Finestra impostata al " + position + "%");
        }
    }
}
