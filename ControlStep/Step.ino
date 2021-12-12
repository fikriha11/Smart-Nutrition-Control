void CtrlStep() {
  switch (Step) {
    case 0:
      setting();
      BacaSensorTandonUtama();
      break;
    case 1 :
      IsiAirBaku();
      Step = 2;
      EEPROM.write(addrStep, Step);
      break;
    case 2 :
      TambahNutrisi();
      Step = 3;
      EEPROM.write(addrStep, Step);
      break;
    case 3:
      AdukCampuran();
      Step = 4;
      EEPROM.write(addrStep, Step);
      break;
    case 4:
      DistribusiNutrisi();
      Step = 0;
      EEPROM.write(addrStep, Step);
      break;
    default :
      break;
  }
}


void BacaSensorTandonUtama() {
  ppmSetting();
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


void TambahNutrisi() {
  attachInterrupt(digitalPinToInterrupt(SensorFlowA), pulseCounterA, RISING);
  showPhrase("TAMBAH NUTRISI A", 19, 10);
  digitalWrite(ValveMixA, LOW);
  digitalWrite(ValveMixB, LOW);
  EEPROM.write(addrFlowA, FlowA.Count);
  if (FlowA.Count >= ppmCount) {
    digitalWrite(ValveMixA, HIGH);
    digitalWrite(ValveMixB, HIGH);
    detachInterrupt(digitalPinToInterrupt(SensorFlowA));
    FlowA.Count = 0;
    EEPROM.write(addrFlowA, FlowA.Count);
    delay(3000);
  }
}

void AdukCampuran() {
  showPhrase("PENGADUKAN", 33, 10);
  MotorMix(MotorMixCampuran, ON);
  delay(60000);
  MotorMix(MotorMixCampuran, OFF);
  delay(2000);
}

void DistribusiNutrisi() {
  showPhrase("PENGELUARAN", 33, 10);
  digitalWrite(ValveOut, LOW);
  if (digitalRead(sw_mixLow) == HIGH) {
    delay(180000);
    digitalWrite(ValveOut, HIGH);
    delay(2000);
  }
}
