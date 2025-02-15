package unit;

import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;

import java.util.LinkedList;
public class DataStore {

    private final LinkedList<JsonObject> dataStore = new LinkedList<>();

    // Salva i dati della temperatura
    public void saveTemperatureData(double temperature, String mode) {
        JsonObject dataObj = new JsonObject()
                .put("temperature", temperature)
                .put("mode", mode)
                .put("timestamp", System.currentTimeMillis());

        dataStore.addFirst(dataObj);

        // max 100
        if (dataStore.size() > 100) {
            dataStore.removeLast();
        }
    }

    // Recupera l'ultimo dato salvato
    public JsonObject getLatestData(String state, int windowPosition) {
        if (dataStore.isEmpty()) {
            return new JsonObject().put("message", "No data available");
        }
        return dataStore.getFirst().put("state", state).put("windowPosition", windowPosition);
    }

    public double getTemperature() {
        return dataStore.isEmpty() ? 0.0 : dataStore.getFirst().getDouble("temperature");
    }

    // Recupera le ultime N temperature
    public JsonObject getLatestNtemperature(int N) {

        JsonArray temperatures = new JsonArray();
        JsonArray timestamps = new JsonArray();

        dataStore.stream().limit(N).forEach(d -> {
            temperatures.add(d.getDouble("temperature"));
            timestamps.add(d.getLong("timestamp"));
        });

        return new JsonObject().put("temperatures", temperatures).put("timestamps", timestamps);
    }
}
