void menuCheck() {

  if (menuCount < 8 ) {
    CursorPos = 0;
    if (digitalRead(4) == LOW) {
      delay(300);
      state = true;
      while (state) {
        displayPPM();
        PPM = map(ppmCount, 0, 254, 0, 1000);
        if (digitalRead(4) == LOW) {
          menuCount = 10;
          state = false;
          break;
        }
      }
    }
  }

  if (menuCount > 8) {
    CursorPos = 10;
    if (digitalRead(4) == LOW) {
      delay(1000);
      state = true;
      while (state) {
        displayRunning();
        menuCount = 0;
        state = false;
        Start = true;
        Setting = false;
        break;
      }
    }
  }

  if (menuCount > 20) {
    menuCount = 0;
  }
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
  display.println(PPM);
  display.display();
  display.clearDisplay();
  delay(50);
}


void displayRunning() {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 10);
  display.println("RUNNING IN");
  display.display();
  display.clearDisplay();
  delay(1000);
  for (int i = 5 ; i > 0; i--) {
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(60, 8);
    display.println(i);
    display.display();
    display.clearDisplay();
    delay(1000);
  }
}


void showPhrase(String phrase) {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 10);
  display.println(phrase);
  display.display();
  display.clearDisplay();
}
