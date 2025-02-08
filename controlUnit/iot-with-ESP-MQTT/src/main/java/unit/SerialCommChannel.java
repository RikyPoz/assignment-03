package unit;

import com.fazecast.jSerialComm.SerialPort;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class SerialCommChannel {
    private SerialPort serialPort;
    private BlockingQueue<String> queue = new LinkedBlockingQueue<>();
    private StringBuffer currentMsg = new StringBuffer("");

    public SerialCommChannel(String portName, int baudRate) throws Exception {
        serialPort = SerialPort.getCommPort(portName);
        serialPort.setBaudRate(baudRate);
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 0, 0);

        if (!serialPort.openPort()) {
            throw new Exception("Errore nell'apertura della porta seriale!");
        }

        System.out.println("[SERIAL] Connessione stabilita su " + portName);

        new Thread(() -> {
            byte[] buffer = new byte[1024];
            while (true) {
                int numBytes = serialPort.readBytes(buffer, buffer.length);
                if (numBytes > 0) {
                    String msg = new String(buffer, 0, numBytes).replaceAll("\r", "");
                    currentMsg.append(msg);

                    while (currentMsg.toString().contains("\n")) {
                        String[] parts = currentMsg.toString().split("\n", 2);
                        queue.offer(parts[0]); // Aggiunge il messaggio alla coda
                        currentMsg = new StringBuffer(parts.length > 1 ? parts[1] : "");
                    }
                }
            }
        }).start();
    }

    public void sendMsg(String msg) {
        try {
            serialPort.writeBytes((msg + "\n").getBytes());
            System.out.println("[SERIAL] Inviato: " + msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String receiveMsg() throws InterruptedException {
        return queue.take(); // Attende fino a quando un messaggio è disponibile
    }

    public boolean isMsgAvailable() {
        return !queue.isEmpty();
    }

    public void close() {
        serialPort.closePort();
    }
}
