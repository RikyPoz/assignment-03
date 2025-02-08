package unit;

import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;

import java.util.LinkedList;

public class DataStore {

    private final LinkedList<JsonObject> dataStore = new LinkedList<>();

    // Salva i dati della temperatura
    public void saveTemperatureData(double temperature) {
        JsonObject dataObj = new JsonObject()
                .put("temperature", temperature)
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

    // Recupera le ultime N temperature
    public JsonObject getLatestNtemperature(int N) {
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
