#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define BTN_UP PIN_PD1
#define BTN_DOWN PIN_PB4
#define BTN_OK PIN_PB2
#define BTN_BACK PIN_PB1
#define LDR PIN_PC0
#define LED PIN_PC1
#define SERVO_UP PIN_PD5
#define SERVO_DOWN PIN_PD6


// create variable
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo SERVO_CNT;
Servo SERVO_REL;

bool state_btn_up = false;
bool state_btn_down = false;
bool state_btn_ok = false;
bool state_btn_back = false;
bool state_candy = false;
int page = 0;
int select = 0;
int total_candy = 0;
int cnt = 0;
int lock_time = 0;

void setup() {
  // initialize
  // LCD
  lcd.init();
  lcd.backlight();
  // BUTTON
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(LED, OUTPUT);
  SERVO_CNT.attach(SERVO_UP);
  SERVO_REL.attach(SERVO_DOWN);
  SERVO_CNT.write(125);
  SERVO_REL.write(80);

  total_candy |= EEPROM.read(0) << 8;
  total_candy |= EEPROM.read(1) & 0xFF;

}

int angle = 45; //125 35

void loop()
{
  if (page == 0) { // select mode
    lcd.setCursor(0, 0);
    lcd.print("Always Sweet");
    lcd.setCursor(0, 1);
    if (select == 1) {
      lcd.print("mode: Eat");
    } else if (select == 2) {
      lcd.print("mode: Lock");
    } else if (select == 3) {
      lcd.print("mode: Refill");
    } else if (select == 4) {
      lcd.print("mode: Amount");
    } else if (select == 5) {
      lcd.print("mode: Save&Exit");
    }

    if (!state_btn_up && digitalRead(BTN_UP) == LOW) {
      state_btn_up = true;
    }
    if (state_btn_up && digitalRead(BTN_UP) == HIGH) {
      state_btn_up = false;
      select++;
      if (select > 5)
        select = 1;
      lcd.clear();
    }

    if (!state_btn_down && digitalRead(BTN_DOWN) == LOW) {
      state_btn_down = true;;
    }
    if (state_btn_down && digitalRead(BTN_DOWN) == HIGH) {
      state_btn_down = false;
      select--;
      if (select < 1)
        select = 5;
      lcd.clear();
    }

    if (!state_btn_ok && digitalRead(BTN_OK) == LOW) {
      state_btn_ok = true;
    }
    if (state_btn_ok && digitalRead(BTN_OK) == HIGH) {
      state_btn_ok = false;
      page = select;
      select = page;
      lcd.clear();
    }
  }
  if (page == 1) { // Eat
    lcd.setCursor(0, 0);
    lcd.print("Eat: ");
    lcd.setCursor(5, 0);
    lcd.print(cnt);
    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.setCursor(7, 1);
    lcd.print(total_candy);

    if (!state_btn_up && digitalRead(BTN_UP) == LOW) {
      state_btn_up = true;
    }
    if (state_btn_up && digitalRead(BTN_UP) == HIGH) {
      state_btn_up = false;
      cnt++;
      lcd.clear();
    }
    if (!state_btn_down && digitalRead(BTN_DOWN) == LOW) {
      state_btn_down = true;
    }
    if (state_btn_down && digitalRead(BTN_DOWN) == HIGH) {
      state_btn_down = false;
      cnt--;
      lcd.clear();
    }
    if (!state_btn_ok && digitalRead(BTN_OK) == LOW) {
      state_btn_ok = true;
    }
    if (state_btn_ok && digitalRead(BTN_OK) == HIGH) {
      state_btn_ok = false;
      for (int i = 0; i < cnt; i++) {
        SERVO_REL.write(55);
        delay(300);
        SERVO_REL.write(80);
        delay(300);
      }
      total_candy -= cnt;
      cnt = 0;
      lcd.clear();

    }

    if (!state_btn_back && digitalRead(BTN_BACK) == LOW) {
      state_btn_back = true;
    }
    if (state_btn_back && digitalRead(BTN_BACK) == HIGH) {
      state_btn_back = false;
      page = 0;
      cnt = 0;
      lcd.clear();
    }
  }
  if (page == 2) { // Lock
    lcd.setCursor(0, 0);
    lcd.print("Lock: ");
    lcd.setCursor(6, 0);
    lcd.print(lock_time);
    if (!state_btn_up && digitalRead(BTN_UP) == LOW) {
      state_btn_up = true;
    }
    if (state_btn_up && digitalRead(BTN_UP) == HIGH) {
      state_btn_up = false;
      lock_time++;
      lcd.clear();
    }
    if (!state_btn_down && digitalRead(BTN_DOWN) == LOW) {
      state_btn_down = true;
    }
    if (state_btn_down && digitalRead(BTN_DOWN) == HIGH) {
      state_btn_down = false;
      lock_time--;
      lcd.clear();
    }
    if (!state_btn_ok && digitalRead(BTN_OK) == LOW) {
      state_btn_ok = true;
    }
    if (state_btn_ok && digitalRead(BTN_OK) == HIGH) {
      state_btn_ok = false;
      while (lock_time > 0)
      {
        lcd.setCursor(0, 0);
        lcd.print("Time left: ");
        lcd.setCursor(12, 0);
        lcd.print(lock_time);
        delay(1000);
        lock_time--;
        lcd.clear();
      }

    }
    if (!state_btn_back && digitalRead(BTN_BACK) == LOW) {
      state_btn_back = true;
    }
    if (state_btn_back && digitalRead(BTN_BACK) == HIGH) {
      state_btn_back = false;
      lock_time = 0;
      page = 0;
      lcd.clear();
    }
  }
  if (page == 3) { // Refill
    lcd.setCursor(0, 0);
    lcd.print("Refill: ");
    lcd.setCursor(7, 0);
    lcd.print(total_candy);
    SERVO_CNT.write(35);
    digitalWrite(LED, HIGH);
    delay(50);
    //lcd.setCursor(0, 1);
    //lcd.print(analogRead(LDR));
    if (!state_candy && analogRead(LDR) < 750) {
      //lcd.print(analogRead(LDR));
      state_candy = true;
    }
    if (state_candy && analogRead(LDR) > 900) {
      total_candy++;
      state_candy = false;
      lcd.clear();
    }
    delay(10);


    if (!state_btn_back && digitalRead(BTN_BACK) == LOW) {
      state_btn_back = true;
    }
    if (state_btn_back && digitalRead(BTN_BACK) == HIGH) {
      state_btn_back = false;
      digitalWrite(LED, LOW);
      SERVO_CNT.write(120);
      page = 0;
      lcd.clear();
    }
  }
  if (page == 4) { // Amount
    lcd.setCursor(0, 0);
    lcd.print("Amount: ");
    lcd.setCursor(8, 0);
    lcd.print(total_candy);
    if (!state_btn_back && digitalRead(BTN_BACK) == LOW) {
      state_btn_back = true;
    }
    if (state_btn_back && digitalRead(BTN_BACK) == HIGH) {
      state_btn_back = false;
      page = 0;
      lcd.clear();
    }
  }
  if (page == 5) { // Exit
    lcd.setCursor(0, 0);
    lcd.print("Are you sure?");
    lcd.setCursor(0, 1);
    lcd.print("Yes or No");

    if (!state_btn_ok && digitalRead(BTN_OK) == LOW) {
      state_btn_ok = true;
    }
    if (state_btn_ok && digitalRead(BTN_OK) == HIGH) {
      state_btn_ok = false;
      lcd.clear();
      page = 100;
      EEPROM.write(0, total_candy >> 8);
      EEPROM.write(1, total_candy & 0xFF);

    }
    if (!state_btn_back && digitalRead(BTN_BACK) == LOW) {
      state_btn_back = true;
    }
    if (state_btn_back && digitalRead(BTN_BACK) == HIGH) {
      state_btn_back = false;
      page = 0;
      lcd.clear();
    }
  }

}



