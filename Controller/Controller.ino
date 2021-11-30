
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

#define encoder0PinA  2
#define encoder0PinB  3

#define encoder0PinA  2
#define encoder0PinB  3

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

unsigned long ltime = millis();
unsigned long ltime1 = millis();
unsigned long VolumeA = 0;
unsigned long VolumeB = 0;

int clk;
int CursorPos = 0;
bool state = false;


byte menuCount = 0;
byte ppmCount = 0;
byte pulse = 0;

/* trial */
String SerialData;
String Data[10];
String SplitData;
int StringData;

/** SAVE EEPROM **/
bool Start = false;
bool Setting = false;
int PPM = 0;

struct SensorFlow {
  int Count;
} FlowA, FlowB;


void BacaSensorTandonUtama() {
  if (digitalRead(sw_airbaku) == HIGH) {
    ProsesMixing = true;
    IsiTandonCampuran = true;
    BacaSensor = false;
  }
}


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
  display.display();
  display.clearDisplay();

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

  attachInterrupt(digitalPinToInterrupt(SensorFlowA), pulseCounterA, RISING);
  attachInterrupt(digitalPinToInterrupt(SensorFlowB), pulseCounterB, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);
}


void loop() {
  setting();
  MainLoop();
}

void setting() {
  if (digitalRead(4) == LOW) {
    Start = false;
    Setting = true;
  }
  while (Setting) {
    menuCheck();
    staticMenu();
  }
}


void MainLoop() {
  if (BacaSensor) {
    showPhrase("STAND BY");
    BacaSensorTandonUtama();
  }
  while (ProsesMixing && Start) {
    /******* Isi Air Tandon Campuran ******/
    showPhrase("ISI AIR BAKU");
    if (IsiTandonCampuran) {
      if (digitalRead(sw_mixHigh) == HIGH) {
        Pompa(ON);
        MotorMix(MotorMixA, ON);
        MotorMix(MotorMixB, ON);
      } else {
        Pompa(OFF);
        MotorMix(MotorMixA, OFF);
        MotorMix(MotorMixB, OFF);
        IsiTandonCampuran = false;
        PenambahanABmix = true;
        PupukA = true;
        ResetFlow();
      }
    }

    /******* Proses Penambahan Pupuk ABmix ******/
    while (PenambahanABmix) {
      readFlow();
      if (PupukA) {
        showPhrase("Nutrisi A");
        digitalWrite(ValveMixA, LOW);
        while (State) {
          ResetFlow();
          delay(500);
          State = false;
        }
        if (FlowA.Count >= 178) {
          digitalWrite(ValveMixA, HIGH);  // 500 ml
          delay(500);
          State = true;
          PupukA = false;
          PupukB = true;
        }
      } else if (PupukB) {
        showPhrase("Nutrisi B");
        digitalWrite(ValveMixB, LOW);
        while (State) {
          ResetFlow();
          delay(1000);
          State = false;
        }
        if (FlowB.Count >= 178) {
          digitalWrite(ValveMixB, HIGH); // 500 ml
          delay(1000);
          PupukB = false;
          Mixing = true;
          PenambahanABmix = false;
        }
      }
    }

    /******* Proses Mixing tandon Campuran ******/
    while (Mixing) {
      showPhrase("MIXING");
      MotorMix(MotorMixCampuran, ON);
      delay(20000);
      MotorMix(MotorMixCampuran, OFF);
      delay(100);
      State = true;
      Mixing = false;
      Distribusi = true;
    }

    /******* Proses Distribusi Pupuk ******/
    while (Distribusi) {
      showPhrase("PENGELUARAN");
      digitalWrite(ValveOut, LOW);
      if (digitalRead(sw_mixLow) == HIGH) {
        digitalWrite(ValveOut, HIGH);
        delay(500);
        Serial.print("Selesai");
        Distribusi = false;
        ProsesMixing = false;
        BacaSensor = true;
      }
    }
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
