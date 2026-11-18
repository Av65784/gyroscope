# Gyro Biplane Dodge

A browser-based 3D biplane dodge game. Control the plane with phone orientation or an MPU6050 connected to an Arduino Uno over USB serial.

## Run

Serve the repository with a local web server:

```bash
python -m http.server 8000
```

Open `http://localhost:8000` in Chrome or Edge. Web Serial requires HTTPS or localhost.

## Arduino Uno + MPU6050 wiring

The firmware is in `biplane-dodge-esp32.ino`; the filename is retained from the earlier ESP32 version, but the current sketch is adapted for an Arduino Uno.

| MPU6050 breakout | Arduino Uno |
| --- | --- |
| SDA | A4 / SDA |
| SCL | A5 / SCL |
| GND | GND |
| VCC | 3.3 V, or 5 V only if the breakout explicitly supports 5 V input |

The Uno uses 5 V logic. For a bare MPU6050 or a breakout without 5 V-safe I2C level shifting, use a bidirectional I2C level shifter on SDA and SCL. Check the breakout board's specifications before connecting power.

Install the **Adafruit MPU6050** and **Adafruit Unified Sensor** libraries in Arduino IDE, select **Arduino Uno**, and upload the sketch. Connect the Uno to the computer over USB. It emits `roll,pitch,yaw` at 115200 baud.

## Controls

- **Keyboard:** arrow keys or WASD
- **Phone:** click **Use Phone Gyro** and tilt the device
- **Arduino Uno:** click **Connect Arduino USB** and select the board's serial port

The webpage accepts comma-separated `roll,pitch,yaw` values or JSON with `roll`, `pitch`, and `yaw` fields. The MPU6050's yaw is gyro-integrated and will drift over time; the game movement uses roll and pitch.

## Model

The game loads `Glorious Borwo-Bombul.stl` when it is reachable at the repository URL. If it cannot load, it uses a geometric placeholder plane.
