package unit;

import java.io.Serial;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.Promise;

public class Main extends AbstractVerticle {

    @Override
    public void start(Promise<Void> startPromise) {

        HttpServer httpServer = new HttpServer();
        MQTTAgent mqttAgent = new MQTTAgent();
        SerialService serialService = new SerialService();
        DataStore centralUnit = new DataStore(httpServer, mqttAgent, serialService);

        // Avvia il server HTTP per la Dashboard
        vertx.deployVerticle(httpServer);

        // Avvia il servizio MQTT per ESP
        vertx.deployVerticle(mqttAgent);

        // Avvia il servizio seriale per la comunicazione con Arduino
        // vertx.deployVerticle(serialService);

        startPromise.complete();

    }

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new Main());
    }
}
