#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char input[12];
int count = 0;
int a = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD’s I2C address if needed

SoftwareSerial mySerial(12, 11);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  pinMode(9, OUTPUT); // Relay control pin
  digitalWrite(9, HIGH); // Ensure relay is off initially

  Serial.begin(9600);
  Serial.println("WELCOME TO ");
  Serial.println("FINGERPRINT BASED");
  Serial.println("VEHICLE IGNITION");
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("FINGERPRINT BASED");
  lcd.setCursor(0, 1);
  lcd.print("VEHICLE IGNITION");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please TAP        ");
  Serial.println("Please TAP");
  lcd.setCursor(0, 1);
  lcd.print("Your License        ");
  Serial.println("Your License");
}

void loop()
{
  if (Serial.available())
  {
    count = 0;
    while (Serial.available() && count < 12)
    {
      input[count] = Serial.read();
      count++;
      delay(5);
    }
    if (count == 12)
    {
      if ((strncmp(input, "4B006F74DC8C", 12) == 0) && (a == 0))
      {
        lcd.setCursor(0, 0);
        lcd.print("License Valid       ");
        Serial.println("License Valid");
        lcd.setCursor(0, 1);
        lcd.print("Welcome             ");
        Serial.println("Welcome");
        delay(1000);
        a = 1;
        fingerprint();
      }
      else if ((strncmp(input, "4B006FB8FF63", 12) == 0) && (a == 0))
      {
        lcd.setCursor(0, 0);
        lcd.print("License Valid       ");
        Serial.println("License Valid");
        lcd.setCursor(0, 1);
        lcd.print("Welcome             ");
        Serial.println("Welcome");
        delay(1000);
        a = 1;
        fingerprint();
      }
      else
      {
        if (a != 1)
        {
          lcd.setCursor(0, 0);
          lcd.print("License Invalid     ");
          Serial.println("License Invalid");
          lcd.setCursor(0, 1);
          lcd.print("Try Again!!!        ");
          Serial.println("Try Again!!!");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Please swipe        ");
          Serial.println("Please swipe");
          lcd.setCursor(0, 1);
          lcd.print("Your License        ");
          Serial.println("Your License");
        }
      }
    }
  }
}

int getFingerprintID()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  return finger.fingerID;
}

void fingerprint()
{
  finger.begin(57600);
  while (a == 1)
  {
    int fingerprintID = getFingerprintID(); // Correct function name
    delay(50);
    if (fingerprintID != -1)
    {
      lcd.setCursor(0, 0);
      lcd.print("Access Granted      ");
      Serial.println("Access Granted");
      lcd.setCursor(0, 1);
      lcd.print("Vehicle Started     ");
      Serial.println("Vehicle Started");
      digitalWrite(9, LOW); // Activate the relay to power the motor
      while (1); // Infinite loop to keep the motor running
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Pls Place a         ");
      Serial.println("Pls Place a");
      lcd.setCursor(0, 1);
      lcd.print("Valid Finger        ");
      Serial.println("Valid Finger");
    }
  }
}
