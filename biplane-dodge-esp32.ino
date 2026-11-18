/*
 ESP32 DevKit + MPU6050 USB serial adapter
 Upload this with Arduino IDE.
 Required libraries: Adafruit_MPU6050, Adafruit_Sensor, ArduinoJson optional.

 Wiring:
   MPU6050 SCL -> ESP32 GPIO 22
   MPU6050 SDA -> ESP32 GPIO 21
   MPU6050 GND -> ESP32 GND
   MPU6050 VCC -> 3V3 (use module-specific voltage guidance)

 Output formats accepted by the web game:
   roll,pitch,yaw
   {"roll":R,"pitch":P,"yaw":Y}
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

constexpr int I2C_SDA_PIN = 21;
constexpr int I2C_SCL_PIN = 22;

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(700);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!mpu.begin()) {
    Serial.println("ERROR:MPU6050_NOT_FOUND");
    while (true) delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t accel, gyro, temperature;
  mpu.getEvent(&accel, &gyro, &temperature);

  // The atan2-based values provide roll/pitch-like orientation without a quaternion library.
  float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 57.2957795f;
  float pitch = atan2(-accel.acceleration.x,
                       sqrt(accel.acceleration.y * accel.acceleration.y +
                            accel.acceleration.z * accel.acceleration.z)) * 57.2957795f;

  static float yawPosition = 0.0f;
  yawPosition += gyro.gyro.z * 0.02f * 57.2957795f;

  // CSV is compact and directly accepted by the web game.
  Serial.print(roll, 1);
  Serial.print(',');
  Serial.print(pitch, 1);
  Serial.print(',');
  Serial.println(yawPosition, 1);

  delay(20);
}
