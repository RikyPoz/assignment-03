package main.java;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.Promise;

public class Main extends AbstractVerticle {

    @Override
    public void start(Promise<Void> startPromise) {
        // Avvia il server HTTP per la Dashboard
        vertx.deployVerticle(new HttpServer());

        // Avvia il servizio MQTT per ESP
        vertx.deployVerticle(new MQTTAgent());

        // Avvia il servizio seriale per la comunicazione con Arduino
        vertx.deployVerticle(new SerialService());

        startPromise.complete();
    }

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new Main());
    }
}
