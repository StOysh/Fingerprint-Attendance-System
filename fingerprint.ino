#include "Adafruit_Fingerprint.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial fingerPrint(3, 2);

#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define register_back 14
#define delete_ok 15
#define forward 16
#define reverse 17
#define match 5
#define indFinger 7
#define buzzer 10
#define records 10

int user1, user2, user3, user4, user5, user6, user7, user8, user9, user10;

DateTime now;

void setup() {
  delay(1000);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(register_back, INPUT_PULLUP);
  pinMode(forward, INPUT_PULLUP);
  pinMode(reverse, INPUT_PULLUP);
  pinMode(delete_ok, INPUT_PULLUP);
  pinMode(match, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);
  digitalWrite(buzzer, LOW);

  if (digitalRead(register_back) == 0) {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.clear();
    lcd.print("Please wait !");
    lcd.setCursor(0, 1);
    lcd.print("Downloding Data");

    Serial.println("Please wait");
    Serial.println("Downloding Data..");
    Serial.println();

    Serial.print("S.No. ");
    for (int i = 0; i < records; i++) {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      Serial.print("      UserID  ");
      Serial.print(1903001 + i);
      Serial.print("            ");
    }
    Serial.println();
    int eepIndex = 0;
    for (int i = 0; i < 30; i++) {
      if (i + 1 < 10)
        Serial.print('0');
      Serial.print(i + 1);
      Serial.print("    ");
      int incr = 0;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      eepIndex = (i * 7) + incr;
      download(eepIndex);
      Serial.print("       ");
      incr += 210;
      Serial.println();
    }
  }
  if (digitalRead(forward) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("deleting.....");
    delay(1000);
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }

  if (digitalRead(delete_ok) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    lcd.setCursor(0, 1);
    lcd.print("Reseting.....");
    for (int i = 1000; i < 1011; i++) {
      EEPROM.write(i, 0);
    }
    for (int i = 0; i < 841; i++) {
      EEPROM.write(i, 0xff);
    }
    lcd.clear();
    lcd.print("System Reset");
    delay(1000);
  }

  lcd.clear();
  lcd.print(" Attendance ");
  lcd.setCursor(0, 1);
  lcd.print(" System ");
  delay(2000);
  lcd.clear();

  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  for (int i = 1000; i < 1000 + records; i++) {
    if (EEPROM.read(i) == 0xff)
      EEPROM.write(i, 0);
  }

  finger.begin(57600);
  Serial.begin(9600);
  lcd.clear();
  lcd.print("Finding Module..");
  lcd.setCursor(0, 1);
  delay(2000);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print(" Module Found");
    delay(2000);
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("Module Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
    while (1)
      ;
  }

  if (!rtc.begin())
    Serial.println("Couldn't find RTC");

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running!");
    // rtc.adjust(DateTime(2024, , 7, 11, 0, 0));
  }
  lcd.setCursor(0, 0);
  lcd.print(" Please Wait ");
  // lcd.setCursor(0, 1);
  // lcd.print(" Wait ");
  delay(3000);

  user1 = EEPROM.read(1000);
  user2 = EEPROM.read(1001);
  user3 = EEPROM.read(1002);
  user4 = EEPROM.read(1003);
  user5 = EEPROM.read(1004);
  user6 = EEPROM.read(1005);
  user7 = EEPROM.read(1006);
  user8 = EEPROM.read(1007);
  user9 = EEPROM.read(1008);
  user10 = EEPROM.read(1009);

  lcd.clear();
  digitalWrite(indFinger, HIGH);
}

void loop() {
  lcd.clear();
  now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.print(" ");
  delay(500);
  int result = getFingerprintIDez();
  if (result > 0) {
    digitalWrite(indFinger, LOW);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    lcd.clear();
    lcd.print("ID:");
    lcd.print(result);
    lcd.setCursor(0, 1);
    lcd.print("Please Wait....");
    delay(1000);
    attendance(result);
    lcd.clear();
    lcd.print("Attendance ");
    lcd.setCursor(0, 1);
    lcd.print("Registered");
    delay(1000);
    digitalWrite(indFinger, HIGH);
    return;
  }
  checkKeys();
  delay(300);
}

void attendance(int id) {
  int user = 0, eepLoc = 0;
  if (id == 1) {
    eepLoc = 0;
    user = user1++;
  } 
  else if (id == 2) {
    eepLoc = 210;
    user = user2++;
  } else if (id == 3) {
    eepLoc = 420;
    user = user3++;
  } 
  else if (id == 4) {
    eepLoc = 630;
    user = user4++;
  } 
  else if (id == 5) {
    eepLoc = 840;
    user = user5++;
  } 
  else if (id == 6) {
    eepLoc = 1050;
    user = user5++;
  } 
  else if (id == 7) {
    eepLoc = 1260;
    user = user7++;
  } 
  else if (id == 8) {
    eepLoc = 1470;
    user = user8++;
  } 
  else if (id == 9) {
    eepLoc = 1680;
    user = user9++;
  } 
  else if (id == 10) {
    eepLoc = 1890;
    user = user8++;
  } 
  else
    return;

  int eepIndex = (user * 7) + eepLoc;
  EEPROM.write(eepIndex++, now.hour());
  EEPROM.write(eepIndex++, now.minute());
  EEPROM.write(eepIndex++, now.second());
  EEPROM.write(eepIndex++, now.day());
  EEPROM.write(eepIndex++, now.month());
  EEPROM.write(eepIndex++, now.year() >> 8);
  EEPROM.write(eepIndex++, now.year());

  EEPROM.write(1000, user1);
  EEPROM.write(1001, user2);
  EEPROM.write(1002, user3);
  EEPROM.write(1003, user4);
  EEPROM.write(1004, user5);
  EEPROM.write(1005, user6);
  EEPROM.write(1006, user7);
  EEPROM.write(1007, user8);
  EEPROM.write(1008, user9);
  EEPROM.write(1009, user10);
}

void checkKeys() {
  if (digitalRead(register_back) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(register_back) == 0)
      ;
    Enroll();
  }
  else if (digitalRead(delete_ok) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
  }
}

void Enroll() {
  int count = 1;
  while (1) {
    lcd.clear();
    lcd.print("Enter Finger ID:");
    lcd.setCursor(0, 1);
    lcd.print(count);
    delay(500);
    if (digitalRead(forward) == 0) {
      count++;
      if (count > records)
        count = 1;
      delay(500);
    }
    else if (digitalRead(reverse) == 0) {
      count--;
      if (count < 1)
        count = records;
      delay(500);
    }
    else if (digitalRead(delete_ok) == 0) {
      id = count;
      getFingerprintEnroll();
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) != 0xff) {
          EEPROM.write(i, id);
          break;
        }
      }
      return;
    } 
    else if (digitalRead(register_back) == 0) {
      return;
    }
  }
}

void delet() {
  int count = 1;
  lcd.clear();
  lcd.print("Enter Finger ID");

  while (1) {
    lcd.setCursor(0, 1);
    lcd.print(count);
    if (digitalRead(forward) == 0) {
      count++;
      if (count > records)
        count = 1;
      delay(500);
    } 
    else if (digitalRead(reverse) == 0) {
      count--;
      if (count < 1)
        count = records;
      delay(500);
    } 
    else if (digitalRead(delete_ok) == 0) {
      id = count;
      deleteFingerprint(id);
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) == id) {
          EEPROM.write(i, 0xff);
          break;
        }
      }
      return;
    } 
    else if (digitalRead(register_back) == 0) {
      return;
    }
  }
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        lcd.clear();
        lcd.print("No Finger Found");
        delay(2000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.print("Comm Error");
        delay(2000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.clear();
        lcd.print("Imaging Error");
        delay(2000);
        break;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.print("Unknown Error");
        delay(2000);
        break;
    }
  }


  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.print("Image too messy");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      delay(2000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      delay(2000);
      return p;
  }

  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print(" Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.print("Communication");
        lcd.setCursor(0, 1);
        lcd.print("Error");
        delay(2000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.clear();
        lcd.print("Imaging");
        lcd.setCursor(0, 1);
        lcd.print("Error");
        delay(2000);
        break;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.print("Unknown");
        lcd.setCursor(0, 1);
        lcd.print("Error");
        delay(2000);
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.print("Image Too");
      lcd.setCursor(0, 1);
      lcd.print("Messy");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Communication");
      lcd.setCursor(0, 1);
      lcd.print("Error");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Unknown");
      lcd.setCursor(0, 1);
      lcd.print("Error");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Unknown");
      lcd.setCursor(0, 1);
      lcd.print("Error");
      delay(2000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown");
      lcd.setCursor(0, 1);
      lcd.print("Error");
      delay(2000);
      return p;
  }

  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } 
  else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } 
  else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print(" Finger Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Finger Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Try Later");
    delay(2000);
    return -1;
  }
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Finger Deleted");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
    delay(1000);
  }

  else {
    Serial.print("Something Wrong");
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0, 1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }
}

void download(int eepIndex) {

  if (EEPROM.read(eepIndex) != 0xff) {
    Serial.print("T->");
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
    Serial.print(EEPROM.read(eepIndex++));
    Serial.print(':');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
    Serial.print(EEPROM.read(eepIndex++));
    Serial.print(':');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
    Serial.print(EEPROM.read(eepIndex++));
    Serial.print(" D->");
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
    Serial.print(EEPROM.read(eepIndex++));
    Serial.print('/');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
    Serial.print(EEPROM.read(eepIndex++));
    Serial.print('/');
    uint16_t value = (EEPROM.read(eepIndex++) << 8) | EEPROM.read(eepIndex++);
    char buffer[8];
    sprintf(buffer, "%04u", value);
    Serial.print(buffer);
  } 
  else {
    Serial.print("T->00:00:00 D->00/00/0000");
  }

  Serial.print(" ");
}
