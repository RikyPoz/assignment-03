package unit;

import java.util.Arrays;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.Promise;

public class Main extends AbstractVerticle {

    @Override
    public void start(Promise<Void> startPromise) {

        DataStore dataStore = new DataStore();
        ControlUnit controlUnit = new ControlUnit(dataStore);
        VerticleService[] verticles = {new HttpServer(controlUnit), new MQTTAgent(controlUnit), new SerialService(controlUnit), new TimerService(controlUnit)};

        Arrays.stream(verticles).forEach(v -> vertx.deployVerticle(v));

        startPromise.complete();

    }

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new Main());
    }
}
