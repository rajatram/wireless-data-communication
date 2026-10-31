
#include <RH_ASK.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RH_ASK rf_driver(2000, PB0, PB1);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char SECRET_PIN[] = "1234";
bool authenticated = false;

#define LED_PIN PC13
#define BUZZER_PIN PB5

bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.print("System Starting");

  delay(1000);

  if (!rf_driver.init()) {
    lcd.clear();
    lcd.print("RF Error!");
    while (1);
  }

  lcd.clear();
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("Data...");
}

void loop() {
  uint8_t buf[8];
  uint8_t buflen = sizeof(buf);

  if (!rf_driver.recv(buf, &buflen))
    return;

  buf[buflen] = '\0';

  char* msg = (char*)buf;

  if (!authenticated) {
    // Expect "PIN:XXXX"
    if (strncmp(msg, "PIN:", 4) == 0) {
      char* rxPin = msg + 4;

      if (strcmp(rxPin, SECRET_PIN) == 0) {
        authenticated = true;

        lcd.clear();
        lcd.print("Access Granted!");

        char ack = 'G';

        rf_driver.send((uint8_t*)&ack, 1);
        rf_driver.waitPacketSent();

        delay(1000);

        lcd.clear();
        lcd.print("Control Mode");

        // Short confirmation beep
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);

      } else {
        lcd.clear();
        lcd.print("Access Denied!");

        char ack = 'D';

        rf_driver.send((uint8_t*)&ack, 1);
        rf_driver.waitPacketSent();

        delay(1500);

        lcd.clear();
        lcd.print("Waiting for");
        lcd.setCursor(0, 1);
        lcd.print("Data...");
      }
    }

  } else {
    // Control phase
    char cmd = msg[0];

    if (cmd == 'A') {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      lcd.setCursor(0, 1);
      lcd.print("LED: ");
      lcd.print(ledState ? "ON " : "OFF");

    } else if (cmd == 'B') {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(300);
      digitalWrite(BUZZER_PIN, LOW);

      lcd.setCursor(0, 1);
      lcd.print("Buzzer: BEEP   ");

    } else if (cmd == 'C') {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);

      lcd.setCursor(0, 1);
      lcd.print("LED+BUZ: Done ");
    }
  }
}
