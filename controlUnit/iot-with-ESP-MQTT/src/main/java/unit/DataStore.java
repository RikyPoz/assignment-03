package unit;

import io.vertx.core.json.JsonArray;
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

    // Recupera le ultime N temperature
    public static JsonObject getLatestNtemperature(int N) {
        if (dataStore.isEmpty()) {
            return new JsonObject().put("message", "No data available");
        }

        JsonArray latestTemp = new JsonArray();

        for (int i = 0; i < N && i < dataStore.size(); i++) {
            double temp = dataStore.get(i).getDouble("temperature");
            latestTemp.add(temp);
        }

        return new JsonObject().put("temperatures", latestTemp);
    }

}
