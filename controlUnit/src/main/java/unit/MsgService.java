package unit;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class MsgService {
    private final ControlUnit controlUnit;

    private final SerialCommChannel channel;
    private final Queue<String> msgToSend = new ConcurrentLinkedQueue<>();

    public MsgService(String port, ControlUnit controlUnit) throws Exception {
        this.channel = new SerialCommChannel(port, 9600);
        this.controlUnit = controlUnit;
        controlUnit.addSerialMsgObserver(msg -> enqueueMessage(msg));
    }

    public void read() {
        try {
            if (channel.isMsgAvailable()) {
                String msg = channel.receiveMsg();
                System.out.println("[SERIAL] Ricevuto: " + msg);

                String[] values = msg.split(",");
                if (values.length == 2) {
                    int windowPosition = Integer.parseInt(values[0]);
                    controlUnit.setWindowPosition(windowPosition, false);
                    controlUnit.setMode(values[1], false);
                }
            }
        } catch (InterruptedException e) {
            System.out.println("[SERIAL] Lettura fallita");
        }
    }

    public void send() {
        if (!msgToSend.isEmpty()) {
            String msg = msgToSend.poll();
            channel.sendMsg(msg);
        }
        channel.sendMsg("temperature_" + controlUnit.getLatestNtemperature(0));
    }

    public void enqueueMessage(String message) {
        msgToSend.add(message);
    }
}
