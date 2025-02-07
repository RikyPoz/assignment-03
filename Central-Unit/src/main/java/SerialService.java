package main.java;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;

public class SerialService extends AbstractVerticle {

    private MsgService msgService;

    @Override
    public void start(Promise<Void> startPromise) {
        try {
            msgService = new MsgService("/dev/ttyACM0");

            // Lettura periodica dei dati
            vertx.setPeriodic(200, id -> msgService.read());

            // Invio periodico di comandi se ci sono messaggi in coda
            vertx.setPeriodic(200, id -> msgService.send());

            startPromise.complete();
        } catch (Exception e) {
            startPromise.fail(e);
        }
    }
}
