int ad[8] = {38,40,42,44,46,48,50,52};
int RD = 43;
int WR = 45;
int ALE = 49;
int MIO = 51;
int commandRegister = 0xC3;
int controlWord = 0x82;
int portA = 0xC0;
int portB = 0xC1;
int d = 0xDE;

void latch() {
    digitalWrite(ALE, HIGH);
    digitalWrite(ALE, LOW);
}

void CS() {
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

void setPins() {
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(ALE, OUTPUT);
  pinMode(MIO, OUTPUT);
    
  digitalWrite(RD, HIGH);
  digitalWrite(WR, HIGH);
  digitalWrite(ALE, LOW);
  digitalWrite(MIO, LOW);
}

void IOWrite(byte addr, byte data) {
  setPins();
  for(int i = 0; i < 8; i++) {
    pinMode(ad[i], OUTPUT);
  }

  for(int i = 0; i < 8; i++) {
    if(bitRead(addr, i) == 1) {
      digitalWrite(ad[i], HIGH);
    }
    else if(bitRead(addr, i) == 0) {
      digitalWrite(ad[i], LOW);
    }
  }

  latch();

  for(int i = 0; i < 8; i++) {
      if(bitRead(data, i) == 1) {
        digitalWrite(ad[i], HIGH);
      }
      else if(bitRead(data, i) == 0) {
        digitalWrite(ad[i], LOW);
      }
    }

  CS();
}

byte IORead (byte addr) {
  byte data;
  setPins();
  
  for(int i = 0; i < 8; i++) {
    pinMode(ad[i], OUTPUT);
  }

  for(int i = 0; i < 8; i++) {
    if(bitRead(addr, i) == 1) {
      digitalWrite(ad[i], HIGH);
    }
    else if(bitRead(addr, i) == 0) {
      digitalWrite(ad[i], LOW);
    }
  }
  
  latch();

  for(int i = 0; i < 8; i++) {
    pinMode(ad[i], INPUT);
  }
  delay(100);
  
  digitalWrite(RD, LOW);
  for(int i = 7; i >= 0; i--) {
    bitWrite(data, i, digitalRead(ad[i]));
  }
  digitalWrite(RD, HIGH);
  
  return data;
}

void setup() { 
  Serial.begin(9600);
  IOWrite(commandRegister, controlWord); 
  IOWrite(portA, d);
  
}

void keypad(byte data){
  IOWrite(portA, data);
  byte column = IORead(portB);
  byte row = IORead(portA);
  
  if(column == 0 || row == 0){
    Serial.println("N/A");
  }
  else if(column == 1 && row == 1){
    Serial.println("1");
  } 
  else if(column == 2 && row == 1){
    Serial.println("2");
  }
  else if(column == 4 && row == 1){
    Serial.println("3");
  }
  else if(column == 8 && row == 1){
    Serial.println("A");
  }
  else if(column == 1 && row == 2){
    Serial.println("4");
  } 
  else if(column == 2 && row == 2){
    Serial.println("5");
  }
  else if(column == 4 && row == 2){
    Serial.println("6");
  }
  else if(column == 8 && row == 2){
    Serial.println("B");
  }
  else if(column == 1 && row == 4){
    Serial.println("7");
  } 
  else if(column == 2 && row == 4){
    Serial.println("8");
  }
  else if(column == 4 && row == 4){
    Serial.println("9");
  }
  else if(column == 8 && row == 4){
    Serial.println("C");
  }
  else if(column == 1 && row == 8){
    Serial.println("*");
  } 
  else if(column == 2 && row == 8){
    Serial.println("0");
  }
  else if(column == 4 && row == 8){
    Serial.println("#");
  }
  else if(column == 8 && row == 8){
    Serial.println("D");
  }
}

void loop() {
  Serial.println("Checking each row:");
  keypad(0x01);
  keypad(0x02);
  keypad(0x04);
  keypad(0x08);
}
