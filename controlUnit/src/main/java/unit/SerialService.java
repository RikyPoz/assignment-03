package unit;

import io.vertx.core.Promise;

public class SerialService extends VerticleService {

    private MsgService msgService;

    public SerialService(ControlUnit controlUnit) {
        super(controlUnit);

    }

    @Override
    public void start(Promise<Void> startPromise) {
        try {
            msgService = new MsgService("/dev/ttyACM1", controlUnit);

            // Lettura periodica dei dati
            vertx.setPeriodic(200, id -> msgService.read());

            // Invio periodico di comandi se ci sono messaggi in coda
            vertx.setPeriodic(1000, id -> msgService.send());

            startPromise.complete();
        } catch (Exception e) {
            startPromise.fail(e);
        }
    }
}
