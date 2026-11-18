# Gyro Biplane Dodge

A browser-based 3D biplane dodge game. The plane is controlled by phone orientation or by an MPU6050 on an ESP32 DevKit over USB.

## Run

Serve the repository with a local web server:

```bash
python -m http.server 8000
```

Open `http://localhost:8000` in Chrome or Edge. Web Serial requires HTTPS or localhost.

## Controls

- **Keyboard:** arrow keys or WASD
- **Phone:** enable Device Orientation and tilt the device
- **ESP32:** upload `biplane-dodge-esp32.ino`, then choose **Connect ESP32 USB**

The ESP32 emits `roll,pitch,yaw` at 115200 baud. The webpage also accepts JSON objects with `roll`, `pitch`, and `yaw` fields.

## Model

The game loads `Glorious Borwo-Bombul.stl` when it is reachable at the repository URL. Until the STL is present on the current branch, it falls back to a geometric placeholder plane.
