package unit;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;

import java.util.Observable;

import io.netty.handler.codec.mqtt.MqttQoS;

public class MQTTAgent extends VerticleService {

    private static final String MQTT_BROKER = "broker.mqtt-dashboard.com";
    private static final String TOPIC_TEMPERATURE = "temperatura"; // ESP32 → Control Unit
    private static final String TOPIC_FREQUENCY = "periodo"; // Control Unit → ESP32
    private static final double T1 = 25.0; // Soglia HOT
    private static final double T2 = 30.0; // Soglia TOO_HOT
    private static final int F1 = 10; // Frequenza in NORMAL state (secondi)
    private static final int F2 = 5; // Frequenza in HOT state (secondi)

    private MqttClient client;

    public MQTTAgent(ControlUnit controlUnit) {
        super(controlUnit);
    }

    @Override
    public void start() {
        client = MqttClient.create(vertx);

        client.connect(1883, MQTT_BROKER, conn -> {
            if (conn.succeeded()) {
                System.out.println("[MQTT] Connesso al broker");

                client.subscribe(TOPIC_TEMPERATURE, 2);
                client.publishHandler(message -> {
                    JsonObject json = new JsonObject(message.payload().toString());
                    double temperature = 0;
                    if (json.containsKey("temperatura")) {
                        temperature = json.getInteger("temperatura");
                    }
                    System.out.println("[MQTT] Temperatura ricevuta: " + temperature);

                    controlUnit.saveTemperature(temperature);
                    sendUpdateFrequency();
                });

            } else {
                System.out.println("[MQTT] Connessione fallita!");
            }
        });
    }

    // Invia la frequenza di aggiornamento
    private void sendUpdateFrequency() {
        if (controlUnit.isStateChanged()) {
            int period = controlUnit.getEspPeriod();
            JsonObject json = new JsonObject().put("period", period);
            client.publish(TOPIC_FREQUENCY,
                    Buffer.buffer(json.encode()),
                    MqttQoS.AT_LEAST_ONCE,
                    false,
                    false);
            System.out.println("[MQTT] Frequenza inviata: " + period + "s");
        }
    }
}
