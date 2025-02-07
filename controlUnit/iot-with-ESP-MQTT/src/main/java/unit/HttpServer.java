package unit;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import io.vertx.core.http.HttpServerResponse;

import java.util.Arrays;

public class HttpServer extends AbstractVerticle {

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
        router.post("/mode/manual").handler(this::handleManualMode);
        router.post("/mode/automatic").handler(this::handleManualMode);

        router.post("/window").handler(this::handleWindowControl);

        router.post("/reset-alarm").handler(this::handleResetAlarm);

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

        // Recuperiamo l'ultimo dal DataStore
        JsonObject latestData = DataStore.getLatestData();

        response.putHeader("content-type", "application/json")
                .end(latestData.encode());
    }

    private void getLatestNtemperature(RoutingContext context) {
        HttpServerResponse response = context.response();

        String nString = context.request().getParam("count");
        int N = (nString != null && !nString.isEmpty()) ? Integer.parseInt(nString) : 10;

        JsonObject latestNtemperature = DataStore.getLatestNtemperature(N);

        response.putHeader("content-type", "application/json")
                .end(latestNtemperature.encode());
    }

    private void handleManualMode(RoutingContext context) {
        /*
         * DataStore.setMode("manual");
         * 
         * context.response()
         * .setStatusCode(200)
         * .end(new JsonObject().put("message", "Modalità manuale attivata").encode());
         */
    }

    private void handleWindowControl(RoutingContext context) {
        /*
         * String openValue = context.request().getParam("open");
         * DataStore.setWindowState(openValue);
         * 
         * context.response()
         * .setStatusCode(200)
         * .end(new JsonObject().put("message", "Finestra controllata").encode());
         */
    }

    private void handleResetAlarm(RoutingContext context) {
        /*
         * DataStore.resetAlarm();
         * 
         * context.response()
         * .setStatusCode(200)
         * .end(new JsonObject().put("message", "Allarme resettato").encode());
         */
    }
}
