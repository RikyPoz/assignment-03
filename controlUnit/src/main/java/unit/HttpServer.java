package unit;

import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import io.vertx.core.http.HttpServerResponse;

public class HttpServer extends VerticleService {

    public HttpServer(ControlUnit controlUnit) {
        super(controlUnit);
    }

    @Override
    public void start() {
        Router router = Router.router(vertx);

        // Aggiungi il CORS handler
        router.route().handler(CorsHandler.create("*") // Permetti tutte le origini (usa '*' per tutti i domini)
                .allowedMethod(io.vertx.core.http.HttpMethod.GET)
                .allowedMethod(io.vertx.core.http.HttpMethod.POST)
                .allowedHeader("Content-Type"));

        // Per gestire i body delle richieste (necessario se si inviano dati)
        router.route().handler(BodyHandler.create());

        router.get("/status").handler(this::handleGetSystemStatus);
        router.get("/latestNtemperature").handler(this::getLatestNtemperature);
        router.post("/mode").handler(this::handleSetMode);
        router.post("/window").handler(this::handleWindowPosition);
        router.get("/reset-alarm").handler(this::handleResetAlarm);

        // Avvio del server HTTP
        vertx.createHttpServer()
                .requestHandler(router)
                .listen(8080, http -> {
                    if (http.succeeded()) {
                        System.out.println("HTTP Server started on port 8080");
                    } else {
                        System.out.println("Failed to start HTTP server");
                    }
                });
    }

    private void handleGetSystemStatus(RoutingContext context) {
        HttpServerResponse response = context.response();
        JsonObject latestData = controlUnit.getLatestData();

        response.putHeader("content-type", "application/json").end(latestData.encode());
    }

    private void getLatestNtemperature(RoutingContext context) {
        HttpServerResponse response = context.response();

        String nString = context.request().getParam("count");
        int N = (nString != null && !nString.isEmpty()) ? Integer.parseInt(nString) : 10;

        JsonObject latestNtemperature = controlUnit.getLatestNtemperature(N);

        response.putHeader("content-type", "application/json").end(latestNtemperature.encode());
    }

    private void handleSetMode(RoutingContext context) {
        JsonObject body = context.getBodyAsJson();
        controlUnit.setMode(body.getString("mode"),true);
        context.response().end(new JsonObject().put("status", "Mode changed").encode());
    }

    private void handleWindowPosition(RoutingContext context) {
        JsonObject body = context.getBodyAsJson();
        int position = body.getInteger("position");

        controlUnit.setWindowPositionManually(position, true);
        context.response().end(new JsonObject().put("status", "Window position updated").encode());
    }

    private void handleResetAlarm(RoutingContext context) {
        controlUnit.resetAlarm();
    }
}
