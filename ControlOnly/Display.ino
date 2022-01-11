void menuCheck() {
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);

  /** Setting PPM **/
  if (menuCount < 8 ) {
    CursorPos = 0;
    if (digitalRead(SwitchBtn) == LOW) {
      Button = true;
    }
    if (digitalRead(SwitchBtn) == HIGH && Button) {
      state = true;
      Button = false;
      ppmCount = EEPROM.read(addrPPM);
      while (state) {
        MappingPPM();
        displayPPM();
        if (digitalRead(SwitchBtn) == LOW) {
          Button = true;
        }
        if (digitalRead(SwitchBtn) == HIGH && Button) {
          EEPROM.write(addrPPM, ppmCount);
          menuCount = 10;
          state = false;
          Button = false;
          break;
        }
      }
    }
  }

  /** START **/
  else if (menuCount > 8 && menuCount < 16) {
    CursorPos = 10;
    if (digitalRead(SwitchBtn) == LOW) {
      Button = true;
    }
    if (digitalRead(SwitchBtn) == HIGH && Button) {
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

  /** BUKA VALVE MANUAL **/
  else if (menuCount >= 16) {
    CursorPos = 20;
    if (digitalRead(SwitchBtn) == LOW) {
      Button = true;
    } if (digitalRead(SwitchBtn) == HIGH && Button) {
      state = true;
      Button = false;
      Mixing = true;
      while (state) {
        manual();
        menuCount = 0;
        state = false;
        Start = true;
        Setting = false;
        detachInterrupt(digitalPinToInterrupt(encoder0PinA));
      }
    }
  }

  /** START MIXING SEMI AUTO **/
  else if (menuCount >= 24) {
    CursorPos = 30;
    if (digitalRead(SwitchBtn) == LOW) {
      Button = true;
    } if (digitalRead(SwitchBtn) == HIGH && Button ) {
      State = true;
      Button = false;
      ProsesMixing = true;
      IsiTandonCampuran = true;
      BacaSensor = false;
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

  else if (menuCount > 32) {
    menuCount = 0;
  }
}

int MappingPPM() {
  int PPM = map(ppmCount, 0, 287, 0, 1000); // max 500 ml (ppm 1000)
  return PPM;
}

void manual() {
  while (Mixing) {
    showPhrase("PENGADUKAN", 33, 10);
    MotorMix(MotorMixCampuran, ON);
    delay(60000);
    MotorMix(MotorMixCampuran, OFF);
    delay(2000);
    State = true;
    Mixing = false;
    Distribusi = true;
  }

  /******* Proses Distribusi Pupuk ******/
  while (Distribusi) {
    showPhrase("PENGELUARAN", 33, 10);
    digitalWrite(ValveOut, LOW);
    if (digitalRead(sw_mixLow) == HIGH) {
      delay(480000);
      digitalWrite(ValveOut, HIGH);
      showPhrase("PENGELUARAN", 33, 10);
      Distribusi = false;
      ProsesMixing = false;
      BacaSensor = true;
    }
  }
}


void staticMenu() {

  display.setTextColor(WHITE);
  display.setTextSize(1.3);


  display.setCursor(10, 0);
  display.println("SETTING");

  display.setCursor(10, 10);
  display.println("Start Auto ");

  display.setCursor(10, 20);
  display.println("Buka Valve");

  display.setCursor(10, 30);
  display.println("Start Manual ");

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
