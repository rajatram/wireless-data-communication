
#include <Keypad.h>
#include <RH_ASK.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {PA0, PA1, PA2, PA3};
byte colPins[COLS] = {PA4, PA5, PA6, PA7};

Keypad customKeypad = Keypad(
  makeKeymap(hexaKeys),
  rowPins, colPins, ROWS, COLS
);

// RF Configuration
RH_ASK rf_driver(2000, PB0, PB1);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char SECRET_PIN[] = "1234";
char enteredPin[5] = "";
int pinIndex = 0;
bool authenticated = false;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Enter PIN:");

  if (!rf_driver.init()) {
    lcd.clear();
    lcd.print("RF Error!");
    while (1);
  }
}

void loop() {
  char key = customKeypad.getKey();

  if (!key) return;

  if (!authenticated) {
    // PIN entry mode
    if (key >= '0' && key <= '9' && pinIndex < 4) {
      enteredPin[pinIndex++] = key;
      lcd.setCursor(pinIndex - 1, 1);
      lcd.print('*');
    }

    if (pinIndex == 4) {
      enteredPin[4] = '\0';

      char msg[6] = "PIN:";
      strcat(msg, enteredPin);

      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();

      // Wait for ACK from receiver
      uint8_t buf[4];
      uint8_t len = sizeof(buf);

      delay(200);

      if (rf_driver.recv(buf, &len)) {
        if (buf[0] == 'G') {
          lcd.clear();
          lcd.print("Access Granted!");
          authenticated = true;
          lcd.setCursor(0, 1);
          lcd.print("A=LED B=BUZ C=ALL");
        } else {
          lcd.clear();
          lcd.print("Access Denied!");
          delay(1500);
          lcd.clear();
          lcd.print("Enter PIN:");
        }
      }

      pinIndex = 0;
      memset(enteredPin, 0, sizeof(enteredPin));

    } 
  } else {
    // Control mode: A=LED, B=Buzzer, C=Both
    char cmd = key;

    if (cmd == 'A' || cmd == 'B' || cmd == 'C') {
      rf_driver.send((uint8_t *)&cmd, 1);
      rf_driver.waitPacketSent();

      lcd.setCursor(0, 1);

      if (cmd == 'A')
        lcd.print("LED Toggle      ");
      else if (cmd == 'B')
        lcd.print("Buzzer Pulse    ");
      else
        lcd.print("LED+BUZ Both    ");
    }
  }
}
