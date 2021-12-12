void NoStep() {
  if (BacaSensor  && Start) {
    ppmSetting();
    Serial.println("STAND BY");
    BacaSensorTandonUtama();
  }
  while (ProsesMixing) {
    /******* Isi Air Tandon Campuran ******/
    showPhrase("ISI AIR BAKU", 25, 10);
    Serial.println("ISI AIR BAKU");
    if (IsiTandonCampuran) {
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

    /******* Proses Penambahan Pupuk ABmix ******/
    while (PenambahanABmix) {
      readFlow();
      int target = EEPROM.read(addrPPM);
      if (PupukA) {
        showPhrase("TAMBAH NUTRISI A", 19, 10);
        Serial.println("Nutrisi A");
        digitalWrite(ValveMixA, LOW);
        if (FlowA.Count >= target) {
          digitalWrite(ValveMixA, HIGH);  // 500 ml
          delay(1000);
          State = true;
          PupukA = false;
          PupukB = true;
        }
      } else if (PupukB) {
        showPhrase("TAMBAH NUTRISI B", 19, 10);
        Serial.println("Nutrisi B");
        digitalWrite(ValveMixB, LOW);
        if (FlowB.Count >= target) {
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
      showPhrase("PENGADUKAN", 33, 10);
      Serial.println("MIXING");
      MotorMix(MotorMixCampuran, ON);
      delay(20000);
      MotorMix(MotorMixCampuran, OFF);
      delay(2000);
      State = true;
      Mixing = false;
      Distribusi = true;
    }

    /******* Proses Distribusi Pupuk ******/
    while (Distribusi) {
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
  }
}
