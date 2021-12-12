

void debug() {
  if (Serial.available()) {
    SerialData = Serial.readString();
    for (int a = 0; a < SerialData.length(); a++) {
      Data[a] = GetData(SerialData, ',', a);
    }
  }
}

String GetData(String data, char Spliter, int Number) {
  StringData = 0;
  SplitData = "";

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == Spliter) {
      StringData++;
    }
    else if (StringData == Number) {
      SplitData.concat(data[i]);
    }
    else if (StringData > Number) {
      return SplitData;
      break;
    }
  }
  return SplitData;
}


void ReadSwitch() {
  Serial.print("Air Baku: ");
  Serial.print(digitalRead(sw_airbaku));
  Serial.print('\t');
  Serial.print("Mix Low: ");
  Serial.print(digitalRead(sw_mixLow));
  Serial.print('\t');
  Serial.print("Mix High: ");
  Serial.println(digitalRead(sw_mixHigh));
}

void readFlow() {
  Serial.print("A: ");
  Serial.print(FlowA.Count);
  Serial.print('\t');
  Serial.print("B: ");
  Serial.println(FlowB.Count);
}

void readSwitchBtn() {
  Serial.println(digitalRead(SwitchBtn));
}
