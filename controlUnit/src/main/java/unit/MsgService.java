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

                String[] values = msg.split("_");
                if (values.length == 2) {
                    if (values[0].equals("mode")) {
                        controlUnit.setMode(values[1], false);
                    } else if (values[0].equals("position")) {
                        controlUnit.setWindowPositionManually(Integer.parseInt(values[1]), false);
                    }
                }
            }
        } catch (InterruptedException e) {
            System.out.println("[SERIAL] Lettura fallita");
        }
    }

    public void send() {
        if (!msgToSend.isEmpty()) {
            final String msg = msgToSend.poll();
            //System.out.println("Invio messaggio: "+msg);
            channel.sendMsg(msg);
        }

    }

    public void enqueueMessage(String message) {
        msgToSend.add(message);
    }
}
