package operator;

import java.util.ArrayDeque;
import java.util.Queue;

public class MsgService {
    private static final int FULL = 0;
    private static final int TEMP_ALERT = 1;
    private static final int WASTE_LEVEL = 2;
    private static final int TEMPERATURE = 3;
    private static final String EMPTY = "empty";
    private static final String RESTORE = "restore";

    private final Gui gui;
    private final CommChannel channel;
    private final Queue<String> msgToSend = new ArrayDeque<>();

    public MsgService(final Gui gui, String port) throws Exception {
        this.gui = gui;
        this.channel = new SerialCommChannel(port, 9600);

        this.gui.addEmptyObserver(() -> this.enqueueMessage(EMPTY));
        this.gui.addRestoreObserver(() -> this.enqueueMessage(RESTORE));
    }

    public void read() {
        try {
            String msg = channel.receiveMsg();

            String[] values = msg.split(",");
            if (values.length == 4) {
                if (values[FULL].equals("0")) {
                    gui.disableEmptyBtn();
                    gui.removeWasteAlert();
                } else {
                    gui.enableEmptyBtn();
                    gui.displayWasteAlert();
                }
                if (values[TEMP_ALERT].equals("0")) {
                    gui.disableRestoreBtn();
                    gui.removeTempAlert();
                } else {
                    gui.enableRestoreBtn();
                    gui.displayTempAlert();
                }
                gui.updateValues(values[WASTE_LEVEL], values[TEMPERATURE]);
                System.out.println("Received");
            }   

        } catch (InterruptedException e) {
            System.out.println("Read failed");
        }
    }

    public void send() {
        if (!msgToSend.isEmpty()) {
            System.out.println("Sending");
            var msg = msgToSend.poll();
            System.out.println(msg);
            channel.sendMsg(msg);
        }
    }

    public void enqueueMessage(String message) {
        msgToSend.add(message);
    }
}
