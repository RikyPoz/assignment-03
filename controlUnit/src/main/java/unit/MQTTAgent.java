package unit;

import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;

import io.netty.handler.codec.mqtt.MqttQoS;

public class MQTTAgent extends VerticleService {

    private static final String MQTT_BROKER = "broker.mqtt-dashboard.com";
    private static final String TOPIC_TEMPERATURE = "temperatura"; // ESP32 → Control Unit
    private static final String TOPIC_FREQUENCY = "periodo"; // Control Unit → ESP32

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

                    controlUnit.saveTemperature(temperature);
                });

            } else {
                System.out.println("[MQTT] Connessione fallita!");
            }
        });

        vertx.setPeriodic(100, id -> sendUpdateFrequency());
        
    }

    // Invia la frequenza di aggiornamento
    private void sendUpdateFrequency() {
        if (controlUnit.isStateChanged()) {
            int period = controlUnit.getEspPeriod();
            JsonObject json = new JsonObject().put("period", period);
            client.publish(TOPIC_FREQUENCY, Buffer.buffer(json.encode()), MqttQoS.AT_LEAST_ONCE, false, false);
        }
    }

}
