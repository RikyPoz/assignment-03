/*
 * package unit;
 * 
 * import io.vertx.core.json.JsonObject;
 * 
 * import java.util.Queue;
 * import java.util.concurrent.ConcurrentLinkedQueue;
 * 
 * public class MsgService {
 * private static final String OPEN = "open";
 * private static final String CLOSE = "close";
 * 
 * private final SerialCommChannel channel;
 * private final Queue<String> msgToSend = new ConcurrentLinkedQueue<>();
 * 
 * public MsgService(String port) throws Exception {
 * this.channel = new SerialCommChannel(port, 9600);
 * }
 * 
 * public void read() {
 * try {
 * if (channel.isMsgAvailable()) {
 * String msg = channel.receiveMsg();
 * System.out.println("[SERIAL] Ricevuto: " + msg);
 * 
 * String[] values = msg.split(",");
 * if (values.length == 2) {
 * double windowPosition = Double.parseDouble(values[0]);
 * double temperature = Double.parseDouble(values[1]);
 * 
 * // Salviamo i dati nel DataStore
 * JsonObject json = new JsonObject()
 * .put("windowPosition", windowPosition)
 * .put("temperature", temperature)
 * .put("timestamp", System.currentTimeMillis());
 * 
 * DataStore.saveTemperatureData(temperature, "UNKNOWN", 0);
 * }
 * }
 * } catch (InterruptedException e) {
 * System.out.println("[SERIAL] Lettura fallita");
 * }
 * }
 * 
 * public void send() {
 * if (!msgToSend.isEmpty()) {
 * String msg = msgToSend.poll();
 * channel.sendMsg(msg);
 * }
 * }
 * 
 * public void enqueueMessage(String message) {
 * msgToSend.add(message);
 * }
 * }
 */
