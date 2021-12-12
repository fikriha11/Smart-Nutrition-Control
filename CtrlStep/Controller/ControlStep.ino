void CtrlStep() {
  switch (Step) {
    case 0:
      ppmSetting();
      BacaSensorTandonUtama();
      break;
    case 1 :
      IsiAirBaku();
      Step = 2;
      EEPROM.write(addrStep, Step);
      break;
    case 2 :
      TambahPupukA();
      Step = 3;
      EEPROM.write(addrStep, Step);
      break;
    case 3:
      TambahPupukB();
      Step = 4;
      EEPROM.write(addrStep, Step);
      break;
    case 4:
      AdukCampuran();
      Step = 5;
      EEPROM.write(addrStep, Step);
      break;
    case 5:
      DistribusiNutrisi();
      Step = 0;
      EEPROM.write(addrStep, Step);
      break;
    default :
      break;
  }
}


void BacaSensorTandonUtama() {
  showPhrase("STAND BY", 25, 10);
  if (digitalRead(sw_airbaku) == HIGH) {
    Step = 1;
    EEPROM.write(addrStep, Step);
  }
}

void IsiAirBaku() {
  showPhrase("ISI AIR BAKU", 25, 10);
  if (digitalRead(sw_mixHigh) == HIGH) {
    delay(500);
    Pompa(ON);
    MotorMix(MotorMixA, ON);
    MotorMix(MotorMixB, ON);
  } else {
    delay(500);
    Pompa(OFF);
    MotorMix(MotorMixA, OFF);
    MotorMix(MotorMixB, OFF);
    delay(2000);
    IsiTandonCampuran = false;
    PenambahanABmix = true;
    PupukA = true;
    ResetFlow();
  }
}


void TambahPupukA() {
  attachInterrupt(digitalPinToInterrupt(SensorFlowA), pulseCounterA, RISING);
  showPhrase("TAMBAH NUTRISI A", 19, 10);
  digitalWrite(ValveMixA, LOW);
  EEPROM.write(addrFlowA, FlowA.Count);
  if (FlowA.Count >= ppmCount) {
    digitalWrite(ValveMixA, HIGH);
    detachInterrupt(digitalPinToInterrupt(SensorFlowA));
    FlowA.Count = 0;
    EEPROM.write(addrFlowA, FlowA.Count);
    delay(1000);
  }
}

void TambahPupukB() {
  attachInterrupt(digitalPinToInterrupt(SensorFlowB), pulseCounterB, RISING);
  showPhrase("TAMBAH NUTRISI B", 19, 10);
  digitalWrite(ValveMixB, LOW);
  EEPROM.write(addrFlowB, FlowB.Count);
  if (FlowB.Count >= ppmCount) {
    digitalWrite(ValveMixB, HIGH);
    detachInterrupt(digitalPinToInterrupt(SensorFlowB));
    FlowB.Count = 0;
    EEPROM.write(addrFlowB, FlowB.Count);
    delay(1000);
  }
}

void AdukCampuran() {
  showPhrase("PENGADUKAN", 33, 10);
  Serial.println("MIXING");
  MotorMix(MotorMixCampuran, ON);
  delay(20000);
  MotorMix(MotorMixCampuran, OFF);
  delay(2000);
}

void DistribusiNutrisi() {
  showPhrase("PENGELUARAN", 33, 10);
  Serial.println("PENGELUARAN");
  digitalWrite(ValveOut, LOW);
  if (digitalRead(sw_mixLow) == HIGH) {
    delay(30000);
    digitalWrite(ValveOut, HIGH);
    Serial.print("Selesai");
    Distribusi = false;
    ProsesMixing = false;
    BacaSensor = true;
  }
}
