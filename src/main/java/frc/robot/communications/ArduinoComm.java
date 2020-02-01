package frc.robot.communications;

import java.time.Duration;
import java.time.Instant;

import edu.wpi.first.wpilibj.SerialPort;
import edu.wpi.first.wpilibj.SerialPort.Port;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class ArduinoComm {
    private static SerialPort serial = new SerialPort(115200, Port.kUSB1);
    private static int blockPos = -999;
    private static int blockHeight = 999;

    public static int blockPosition() {
        return blockPos;
    }

    public static int blockHeight() {
        return blockHeight;
    }

    public static boolean RequestDataFromPixy(boolean useLED) {
        byte[] b = new byte[1];
        final int responseLen = ("S-999,999E").length();
        serial.disableTermination();

        // See if there are any bytes in the buffer. If so, reset so we
        // are in a known state.
        if (serial.getBytesReceived() > 0) {
            serial.reset();
        }

        // Send 'T' if you want the LEDs turned on for sensing
        b[0] = (byte) (useLED ? 'T' : 'F');
        serial.write(b, 1);

        // if the arduino doesn't respond, we need to rely on the command
        // that called this routine to timeout and end.
        Instant start = Instant.now();
        long timeElapsed = 0;
        while (serial.getBytesReceived() < responseLen && (timeElapsed < 100)) {
            // loop until all the bytes we are looking for are there.
            Instant finish = Instant.now();
            timeElapsed = Duration.between(start, finish).toMillis();
        }
        if (serial.getBytesReceived() >= responseLen) {
            String response = serial.readString(responseLen);
            SmartDashboard.putString("Data from Pixy", response);
            blockPos = decodeX(response);
            blockHeight = decodeHeight(response);
        } else {
            blockPos = -999;
            blockHeight = 999;
        }

        return (blockPos != -999);
    }

    private static int decodeX(String input) {
        int sPos = input.indexOf('S');
        int commaPos = input.indexOf(',');
        int result = -999;
        try {
            result = Integer.parseInt(input.substring(sPos + 1, commaPos));
        } catch (Exception e) {
            System.out.println("decodeX Error - input=" + input);
        }
        return result;
    }

    private static int decodeHeight(String input) {
        int commaPos = input.indexOf(',');
        int ePos = input.indexOf('E');
        int result = 999;
        try {
            result = Integer.parseInt(input.substring(commaPos + 1, ePos));
        } catch (Exception ex) {
            System.out.println("decodeHeight Error - input=" + input);
        }
        return result;
    }
}
