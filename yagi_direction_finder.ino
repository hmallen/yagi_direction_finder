/*
   Yagi Antenna Direction Finder
*/

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Simple_AHRS.h>
#include <LiquidCrystal.h>
#include <Wire.h>

Adafruit_LSM303_Accel_Unified accel(30301);
Adafruit_LSM303_Mag_Unified   mag(30302);

Adafruit_Simple_AHRS          ahrs(&accel, &mag);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

bool debugMode = false;

void setup() {
  Serial.begin(115200);

  accel.begin();
  mag.begin();

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(F("Yagi Antenna"));
  lcd.setCursor(0, 1);
  lcd.print(F("Direction Finder"));
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Azimuth"));
  lcd.setCursor(0, 1);
  lcd.print(F("Elevation (Roll)"));
  delay(5000);
  lcd.clear();
}

void loop() {
  sensors_vec_t   orientation;

  if (ahrs.getOrientation(&orientation)) {
    float ahrsRoll = orientation.roll;
    float ahrsPitch = orientation.pitch;
    float ahrsHeading = orientation.heading;

    if (ahrsHeading > 0.0) ahrsHeading = 360.0 - ahrsHeading;
    else if (ahrsHeading < 0.0) ahrsHeading = abs(ahrsHeading);

    if (ahrsPitch > 0.0) ahrsPitch = -1 * ahrsPitch;
    else if (ahrsPitch < 0.0) ahrsPitch = abs(ahrsPitch);

    if (debugMode) {
      Serial.print(ahrsRoll, 1);
      Serial.print(" / ");
      Serial.print(ahrsPitch, 1);
      Serial.print(" / ");
      Serial.println(ahrsHeading, 1);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(ahrsHeading, 1);
      if (ahrsPitch < 0.0) lcd.setCursor(0, 1);
      else lcd.setCursor(1, 1);
      lcd.print(ahrsPitch, 1);
      lcd.setCursor(7, 1);
      lcd.print(F("("));
      lcd.print(ahrsRoll);
      lcd.print(F(")"));
    }
  }
  delay(500);
}
