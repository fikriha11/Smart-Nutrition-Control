void menuCheck() {
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);
  if (menuCount < 8 ) {
    CursorPos = 0;
    if (digitalRead(4) == LOW) {
      Button = true;
    }
    if (digitalRead(4) == HIGH && Button) {
      state = true;
      Button = false;
      ppmCount = EEPROM.read(addrPPM);
      while (state) {
        MappingPPM();
        displayPPM();
        if (digitalRead(4) == LOW) {
          Button = true;
        }
        if (digitalRead(4) == HIGH && Button) {
          EEPROM.write(addrPPM, ppmCount);
          menuCount = 10;
          state = false;
          Button = false;
          break;
        }
      }
    }
  }

  else if (menuCount > 8) {
    CursorPos = 10;
    if (digitalRead(4) == LOW) {
      Button = true;
    }
    if (digitalRead(4) == HIGH && Button) {
      state = true;
      Button = false;
      while (state) {
        displayRunning();
        menuCount = 0;
        state = false;
        Start = true;
        Setting = false;
        detachInterrupt(digitalPinToInterrupt(encoder0PinA));
      }
    }
  }

  else if (menuCount > 10) {
    menuCount = 0;
  }
}

int MappingPPM() {
  int PPM = map(ppmCount, 0, 254, 0, 1000);
  return PPM;
}



void staticMenu() {

  display.setTextColor(WHITE);
  display.setTextSize(1.3);


  display.setCursor(10, 0);
  display.println("SETTING");

  display.setCursor(10, 10);
  display.println("START");

  display.setCursor(2, CursorPos);
  display.println(">");
  display.display();
  display.clearDisplay();
  delay(50);

}

void displayPPM() {
  display.setTextColor(WHITE);
  display.setTextSize(1.7);

  display.setCursor(10, 10);
  display.println("PPM: ");
  display.setCursor(40, 10);
  display.println(MappingPPM());
  display.display();
  display.clearDisplay();
  delay(50);
}

void ppmSetting() {
  ppmCount = EEPROM.read(addrPPM);
  display.setTextColor(WHITE);
  display.setTextSize(1.7);

  display.setCursor(35, 5);
  display.println("STAND BY");

  display.setCursor(22, 20);
  display.println("SET PPM : ");
  display.setCursor(80, 20);
  display.println(MappingPPM());
  display.display();
  display.clearDisplay();
  delay(50);
}


void displayRunning() {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(30, 10);
  display.println("RUNNING IN");
  display.display();
  display.clearDisplay();
  delay(1000);
  for (int i = 5 ; i > 0; i--) {
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(55, 5);
    display.println(i);
    display.display();
    display.clearDisplay();
    delay(1000);
  }
}


void showPhrase(String phrase, int col, int row) {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(col, row);
  display.println(phrase);
  display.display();
  display.clearDisplay();
  delay(100);
}
