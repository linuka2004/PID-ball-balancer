#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <ESP32Servo.h>

Adafruit_VL53L0X lox;
Servo beamServo;

const int SERVO_PIN = 18;

// Servo limits
const int SERVO_MIN = 60;
const int SERVO_MAX = 120;

// Distance range for testing
const int DIST_MIN = 100;   // 100 mm = 10 cm
const int DIST_MAX = 500;   // 500 mm = 50 cm

void setup() {
  Serial.begin(115200);
  delay(1000);

  // I2C
  Wire.begin(21, 22);

  // VL53L0X
  if (!lox.begin()) {
    Serial.println("ERROR: VL53L0X not detected!");
    while (1) {
      delay(100);
    }
  }

  Serial.println("VL53L0X detected!");

  // Servo
  beamServo.setPeriodHertz(50);
  beamServo.attach(SERVO_PIN, 500, 2400);

  // Start at center
  beamServo.write(90);

  Serial.println("System ready");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {

    int distance = measure.RangeMilliMeter;

    // Limit distance to test range
    distance = constrain(distance, DIST_MIN, DIST_MAX);

    // Map distance to servo angle
    int servoAngle = map(
      distance,
      DIST_MIN,
      DIST_MAX,
      SERVO_MIN,
      SERVO_MAX
    );

    beamServo.write(servoAngle);

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" mm");

    Serial.print(" | Servo angle: ");
    Serial.println(servoAngle);

  } else {
    Serial.println("Out of range");
  }

  delay(100);
}