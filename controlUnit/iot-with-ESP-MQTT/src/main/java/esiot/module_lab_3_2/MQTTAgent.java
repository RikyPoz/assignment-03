package esiot.module_lab_3_2;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.mqtt.MqttClient;

public class MQTTAgent extends AbstractVerticle {

    private static final String BROKER_ADDRESS = "broker.mqtt-dashboard.com";
    private static final String TOPIC_NAME = "temperatura";
    private static final int PERIOD = 5000;

    // Variabile per salvare l'ultima temperatura ricevuta.
    // Se non abbiamo ancora ricevuto un messaggio, viene inizializzata a un valore predefinito (ad es. 20)
    private int lastTemperature = 20;

    public MQTTAgent() {
    }

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        client.connect(1883, BROKER_ADDRESS, conn -> {
            if (conn.succeeded()) {
                log("Connected to broker " + BROKER_ADDRESS);

                // Imposta il publish handler per gestire i messaggi in arrivo.
                client.publishHandler(s -> {
                    String topic = s.topicName();
                    String payloadStr = s.payload().toString();
                    try {
                        // Utilizza JsonObject per il parsing del messaggio
                        JsonObject json = new JsonObject(payloadStr);
                        if (json.containsKey("temperatura")) {
                            lastTemperature = json.getInteger("temperatura");
                            log("Last temperature updated to: " + lastTemperature);
                        }
                    } catch (Exception ex) {
                        log("Error parsing JSON: " + ex.getMessage());
                    }
                })
                .subscribe(TOPIC_NAME, 2, sub -> {
                    if (sub.succeeded()) {
                        log("Subscribed to topic " + TOPIC_NAME);
                    } else {
                        log("Subscription failed: " + sub.cause().getMessage());
                    }
                });

                // Imposta un timer periodico ogni 10 secondi per inviare il nuovo sendPeriod.
                vertx.setPeriodic(PERIOD, timerId -> {
                    // Calcola il nuovo periodo: 50 * ultima temperatura (in ms).
                    int newPeriodMs = 50 * lastTemperature;

                    // Crea il JSON da inviare
                    JsonObject json = new JsonObject().put("sendPeriod", newPeriodMs);
                    Buffer buffer = Buffer.buffer(json.encode());

                    client.publish(TOPIC_NAME,
                            buffer,
                            MqttQoS.AT_LEAST_ONCE,
                            false,
                            false,
                            pubRes -> {
                                if (pubRes.succeeded()) {
                                    log("Published new sendPeriod: " + newPeriodMs + " ms");
                                } else {
                                    log("Failed to publish new sendPeriod: " + pubRes.cause().getMessage());
                                }
                            });
                });

                // (Opzionale) Pubblica un messaggio iniziale.
                log("Initial publishing...");
                client.publish(TOPIC_NAME,
                        Buffer.buffer("hello"),
                        MqttQoS.AT_LEAST_ONCE,
                        false,
                        false);
            } else {
                log("Connection failed: " + conn.cause().getMessage());
            }
        });
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
}
