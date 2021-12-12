
#include <avr/wdt.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

#define encoder0PinA  19
#define encoder0PinB  18

/* Switch Sensor */
#define sw_airbaku  A1
#define sw_mixLow  A2
#define sw_mixHigh  A3
#define sw_A  A4
#define sw_B  A5

#define Pump 12
#define MotorMixA 11
#define MotorMixB 10
#define MotorMixCampuran 9
#define ValveMixA 7
#define ValveMixB 6
#define ValveOut 5
#define SensorFlowA  2
#define SensorFlowB  3


#define ON  1
#define OFF 2

int flowMixA = 0;
int flowMixB = 0;

bool State = true;
bool ProsesMixing = false;
bool IsiTandonCampuran = false;
bool PenambahanABmix = false;
bool Distribusi = false;
bool Mixing = false;
bool BacaSensor = true;

bool PupukA = false;
bool PupukB = false;

int CursorPos = 0;
bool state = false;
bool Button = false;


byte menuCount = 0;
byte ppmCount = 0;
byte pulse = 0;
int Step = 0;

/* trial */
String SerialData;
String Data[10];
String SplitData;
int StringData;

bool Start = true;
bool Setting = false;

/** SAVE EEPROM **/
int addrPPM = 0;
int addrStep = 0;
int addrFlowA = 0;
int addrFlowB = 0;

struct SensorFlow {
  int Count;
} FlowA, FlowB;



void Pompa(int Status) {
  if (Status == ON) {
    digitalWrite(Pump, LOW);
  } else {
    digitalWrite(Pump, HIGH);
  }
}

void MotorMix(int Type, int Status) {
  if (Status == ON) {
    digitalWrite(Type, LOW);
  } else {
    digitalWrite(Type, HIGH);
  }
}


void setup() {
  Wire.begin();
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  /* Read EEPROM */
  ppmCount = EEPROM.read(addrPPM);
  Step = EEPROM.read(addrStep);
  FlowA.Count = EEPROM.read(addrFlowA);
  FlowB.Count = EEPROM.read(addrFlowB);

  /* Switch */
  pinMode(sw_airbaku, INPUT);
  pinMode(sw_mixLow, INPUT);
  pinMode(sw_mixHigh, INPUT);
  pinMode(sw_A, INPUT);
  pinMode(sw_B, INPUT);
  pinMode(4, INPUT_PULLUP);

  /* Motor and Valve */
  pinMode(Pump, OUTPUT);
  pinMode(MotorMixCampuran, OUTPUT);
  pinMode(MotorMixA, OUTPUT);
  pinMode(MotorMixB, OUTPUT);
  pinMode(ValveMixA, OUTPUT);
  pinMode(ValveMixB, OUTPUT);
  pinMode(ValveOut, OUTPUT);

  digitalWrite(Pump, HIGH);
  digitalWrite(MotorMixA, HIGH);
  digitalWrite(MotorMixB, HIGH);
  digitalWrite(ValveMixA, HIGH);
  digitalWrite(ValveMixB, HIGH);
  digitalWrite(ValveOut, HIGH);
  digitalWrite(MotorMixCampuran, HIGH);

  pinMode(SensorFlowA, INPUT);
  pinMode(SensorFlowB, INPUT);


  attachInterrupt(digitalPinToInterrupt(SensorFlowB), pulseCounterB, RISING);

  display.clearDisplay();
  display.setTextSize(1.7);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("AN-MIX");
  display.print("Menyiapkan...");
  display.display();
  delay(5000);
}


void loop() {
  CtrlStep();
}

void setting() {
  if (digitalRead(4) == LOW) {
    Button = true;
  } if (digitalRead(4) == HIGH && Button) {
    Setting = true;
    Button = false;
  }
  while (Setting) {
    staticMenu();
    menuCheck();
  }
}

void ResetFlow() {
  FlowA.Count = 0;
  FlowB.Count = 0;
}

void pulseCounterA() {
  FlowA.Count++;
}

void pulseCounterB() {
  FlowB.Count++;
}

void doEncoder() {
  if (digitalRead(encoder0PinA) == HIGH) {
    if (digitalRead(encoder0PinB) == LOW && ppmCount > 0) {
      ppmCount--;
      menuCount--;
      pulse--;
    }
    else {
      ppmCount++;
      menuCount++;
      pulse++;
    }
  }
  else
  {
    if (digitalRead(encoder0PinB) == LOW ) {
      ppmCount++;
      menuCount++;
      pulse++;
    }
    else {
      if (ppmCount > 0) {
        ppmCount--;
        menuCount--;
        pulse--;
      }

    }
  }
}
