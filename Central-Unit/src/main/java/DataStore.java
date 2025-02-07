package main.java;

import io.vertx.core.json.JsonObject;
import java.util.LinkedList;

public class DataStore {

    private static final LinkedList<JsonObject> dataStore = new LinkedList<>();

    // Salva i dati della temperatura
    public static void saveTemperatureData(double temperature, String state, int frequency) {
        JsonObject dataObj = new JsonObject()
                .put("temperature", temperature)
                .put("state", state)
                .put("frequency", frequency)
                .put("windowPosition", 0.0) // Default
                .put("timestamp", System.currentTimeMillis());

        dataStore.addFirst(dataObj);

        // max 100
        if (dataStore.size() > 100) {
            dataStore.removeLast();
        }
    }

    // Recupera l'ultimo dato salvato
    public static JsonObject getLatestData() {
        if (dataStore.isEmpty()) {
            return new JsonObject().put("message", "No data available");
        }
        return dataStore.getFirst();
    }
}
