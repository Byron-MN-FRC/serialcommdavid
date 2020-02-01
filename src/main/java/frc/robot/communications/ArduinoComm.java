package frc.robot.communications;

import edu.wpi.first.wpilibj.SerialPort;
import edu.wpi.first.wpilibj.SerialPort.Port;

public class ArduinoComm {
    private static SerialPort serial = new SerialPort(115200, Port.kUSB1);
    private static int blockPos = -999;
    private static int blockHeight = 999;

    public static int blockPosition(){ return blockPos; }
    public static int blockHeight() { return blockHeight; }

    public static boolean RequestDataFromPixy(boolean useLED){
        byte[] b = new byte[1];
        final int responseLen = ("S-999,999E").length();

        // See if there are any bytes in the buffer.  If so, reset so we  
        // are in a known state.
        if (serial.getBytesReceived()> 0) {
            serial.reset();
        }

        // Send 'T' if you want the LEDs turned on for sensing
        b[0] = (byte) (useLED ? 'T' : 'F');
        serial.write(b,1);
        
        // if the arduino doesn't respond, we need to rely on the command
        // that called this routine to timeout and end.
        while (serial.getBytesReceived() < responseLen) {
            //loop until all the bytes we are looking for are there.
        }

        if (serial.getBytesReceived() >= responseLen) {
            String response = serial.readString(responseLen);
            blockPos = decodeX(response);
            blockHeight = decodeHeight(response);
        }

        return (blockPos != -999);
    }

    private static int decodeX(String input){
        int sPos = input.indexOf('S');
        int commaPos = input.indexOf(',');
        return Integer.parseInt(input.substring(sPos+1, commaPos));
    }    
    
    private static int decodeHeight(String input){
        int commaPos = input.indexOf(',');
        int ePos = input.indexOf('E');
        return Integer.parseInt(input.substring(commaPos+1, ePos));
    }  
}
