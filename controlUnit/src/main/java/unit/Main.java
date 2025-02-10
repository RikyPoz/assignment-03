package unit;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.Promise;

public class Main extends AbstractVerticle {

    @Override
    public void start(Promise<Void> startPromise) {

        DataStore dataStore = new DataStore();
        ControlUnit controlUnit = new ControlUnit(dataStore);
        HttpServer httpServer = new HttpServer(controlUnit);
        MQTTAgent mqttAgent = new MQTTAgent(controlUnit);
        SerialService serialService = new SerialService(controlUnit);

        // Avvia il server HTTP per la Dashboard
        vertx.deployVerticle(httpServer);

        // Avvia il servizio MQTT per ESP
        vertx.deployVerticle(mqttAgent);

        // Avvia il servizio seriale per la comunicazione con Arduino
        vertx.deployVerticle(serialService, res -> {
            if (res.succeeded()) {
                System.out.println("[MAIN] SerialService avviato con successo!");
            } else {
                System.err.println("[MAIN] Errore nell'avvio di SerialService: " + res.cause());
            }
        });

        startPromise.complete();

    }

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new Main());
    }
}
