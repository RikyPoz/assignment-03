package main.java;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;
import io.netty.handler.codec.mqtt.MqttQoS;

public class MQTTAgent extends AbstractVerticle {

    private static final String MQTT_BROKER = "broker.mqtt-dashboard.com";
    private static final String TOPIC_TEMPERATURE = "temperature/data"; // ESP32 → Control Unit
    private static final String TOPIC_FREQUENCY = "temperature/frequency"; // Control Unit → ESP32

    private static final double T1 = 25.0; // Soglia HOT
    private static final double T2 = 30.0; // Soglia TOO_HOT
    private static final int F1 = 10; // Frequenza in NORMAL state (secondi)
    private static final int F2 = 5; // Frequenza in HOT state (secondi)

    private MqttClient client;

    @Override
    public void start() {
        client = MqttClient.create(vertx);

        client.connect(1883, MQTT_BROKER, conn -> {
            if (conn.succeeded()) {
                System.out.println("[MQTT] Connesso al broker");

                client.subscribe(TOPIC_TEMPERATURE, MqttQoS.AT_LEAST_ONCE);
                client.publishHandler(message -> {
                    JsonObject json = new JsonObject(message.payload().toString());
                    double temperature = json.getDouble("temperature");

                    System.out.println("[MQTT] Temperatura ricevuta: " + temperature);

                    determineSystemState(temperature);
                });

            } else {
                System.out.println("[MQTT] Connessione fallita!");
            }
        });
    }

    // determina lo stato e salva i dati nel DataStore
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

        System.out.println("[MQTT] Stato del sistema: " + state);
        DataStore.saveTemperatureData(temperature, state, frequency);
        sendUpdateFrequency(frequency);
    }

    // Invia la frequenza di aggiornamento
    private void sendUpdateFrequency(int frequency) {
        JsonObject json = new JsonObject().put("frequency", frequency);
        client.publish(TOPIC_FREQUENCY,
                Buffer.buffer(json.encode()),
                MqttQoS.AT_LEAST_ONCE,
                false,
                false);
        System.out.println("[MQTT] Frequenza inviata: " + frequency + "s");
    }
}
