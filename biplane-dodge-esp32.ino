/*
 Arduino Uno + MPU6050 USB serial adapter
 Upload this sketch to an Arduino Uno using the Arduino IDE.
 Required libraries: Adafruit_MPU6050 and Adafruit_Sensor.

 Wiring (Arduino Uno):
   MPU6050 SDA -> A4 (SDA)
   MPU6050 SCL -> A5 (SCL)
   MPU6050 GND -> GND
   MPU6050 VCC -> 3.3V, or 5V only if your breakout board explicitly supports it

 Important: The Uno uses 5 V logic. Use a bidirectional I2C level shifter
 unless your MPU6050 breakout explicitly includes 5 V-safe I2C level shifting.

 USB serial output at 115200 baud: roll,pitch,yaw
 The browser game accepts this CSV format.
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Uno hardware I2C pins: SDA=A4, SCL=A5

  if (!mpu.begin()) {
    Serial.println("ERROR:MPU6050_NOT_FOUND");
    while (true) {
      delay(1000);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t accel, gyro, temperature;
  mpu.getEvent(&accel, &gyro, &temperature);

  const float radiansToDegrees = 57.2957795f;
  const float roll = atan2(accel.acceleration.y, accel.acceleration.z) * radiansToDegrees;
  const float pitch = atan2(-accel.acceleration.x,
                            sqrt(accel.acceleration.y * accel.acceleration.y +
                                 accel.acceleration.z * accel.acceleration.z)) * radiansToDegrees;

  // Integrate Z gyro rate with measured elapsed time. Yaw will drift over time
  // because this simple setup has no magnetometer correction; the game mainly
  // uses roll and pitch for movement.
  static float yawPosition = 0.0f;
  static unsigned long previousMicros = 0;
  const unsigned long nowMicros = micros();
  if (previousMicros != 0) {
    const float elapsedSeconds = (nowMicros - previousMicros) / 1000000.0f;
    yawPosition += gyro.gyro.z * elapsedSeconds * radiansToDegrees;
  }
  previousMicros = nowMicros;

  Serial.print(roll, 1);
  Serial.print(',');
  Serial.print(pitch, 1);
  Serial.print(',');
  Serial.println(yawPosition, 1);

  delay(20);
}
