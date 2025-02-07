package unit;

import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;

import java.util.ArrayList;
import java.util.LinkedList;

public class DataStore {

    public DataStore(httpServer, mqttAgent, serialService){
        this.httpServer = httpServer;
        this.mqttAgent = mqttAgent;
        this.serialService = serialService;
    }

    private static final LinkedList<JsonObject> tempStore = new LinkedList<>();
    public String mode = "AUMTOMATIC";
    public int windowPosition = 0;
    public int frequency = 0;

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

    public static JsonObject getLatestNtemperature(int N) {
        if (dataStore.isEmpty()) {
            return new JsonObject().put("message", "No data available");
        }

        JsonArray latestTemp = new JsonArray();

        // Assicurati di non superare la dimensione della lista
        for (int i = 0; i < N && i < dataStore.size(); i++) {
            double temp = dataStore.get(i).getDouble("temperature"); // Usa getDouble per un valore double
            latestTemp.add(temp);
        }

        // Restituisci il JsonArray come parte di un JsonObject
        return new JsonObject().put("temperatures", latestTemp);
    }

    public void notifyManual() {

    }

    private void determineSystemState(double temperature) {
        String state;
        int frequency;

        if (temperature < T1) {
            state = "NORMAL";
            frequency = F1;
        } else if (temperature <= T2) {
            state = "HOT";
            frequency = F2;
        } else {
            state = "TOO_HOT";
            frequency = F2;
        }

    }
}
