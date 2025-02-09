package unit;

import io.vertx.core.AbstractVerticle;

public class VerticleService extends AbstractVerticle {
    protected final ControlUnit controlUnit;

    public VerticleService(ControlUnit controlUnit) {
        this.controlUnit = controlUnit;
    }
}
