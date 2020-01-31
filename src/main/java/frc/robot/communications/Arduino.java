package frc.robot.communications;

import edu.wpi.first.wpilibj.SerialPort;
import edu.wpi.first.wpilibj.SerialPort.Port;

public class Arduino {
    private SerialPort serial;

    public Arduino() {
        try {
            serial = new SerialPort(115200, Port.kUSB1);
        } catch (final Exception e) {
            System.out.println("something went wrong, " + e.getMessage());
        }
    }

    public String getData() {
        try {
            String s = this.serial.readString();
            System.out.println(s);
            return s;
        } catch (final Exception e) {
            System.out.println("something went wrong, " + e.getMessage());
            return null;
        }
    }

    public String RequestData(String request) {
        this.serial.writeString(request);
        return this.serial.readString();
    }

    public String RequestData() {
        if (this.serial.getBytesReceived() > 0)
            return this.serial.readString();
        else    
            return "-1";

    }

    // public boolean sendData([byte] buffer) throws Exception {
    // try {
    // final int count = buffer.length;
    // this.serial.write(buffer, count);
    // return true;
    // } catch (final Exception e) {
    // System.out.println("something went wrong, " + e.getMessage());
    // return false;
    // }
    // }

    // public boolean printf(final String data) {
    // try {
    // this.serial.print(data);
    // return true;
    // } catch (final Exception e) {
    // System.out.println("something went wrong, " + e.getMessage());
    // return false;
    // }
    // }

    // public String requestData() {
    // try {
    // this.serial.print("r");
    // return this.serial.readString();
    // } catch (final Exception e) {
    // System.out.println("something went wrong, " + e.getMessage());
    // return null;
    // }
    // }

    // public int requestData(final String request) {
    // try {
    // this.serial.print(request);
    // return NumberUtils.stringToInt(this.getData());
    // } catch (final Exception e) {
    // System.out.println("something went wrong, " + e.getMessage());
    // return 0;
    // }
    // }
}
