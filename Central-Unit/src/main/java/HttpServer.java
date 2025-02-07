package main.java;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.core.http.HttpServerResponse;

public class HttpServer extends AbstractVerticle {

    @Override
    public void start() {
        Router router = Router.router(vertx);

        // Per gestire i body delle richieste (necessario se si inviano dati)
        router.route().handler(BodyHandler.create());

        // Endpoint per restituire tutte le informazioni di sistema
        router.get("/system/status").handler(this::handleGetSystemStatus);

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
}
