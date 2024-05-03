//define pins
int ALE_PIN = 49;
int MIO_PIN = 51;
int WRITE_PIN = 45;
int READ_PIN = 43;

int ad_pins[8] = {38,40,42,44,46,48,50,52};

int portA = 0x80;
int portB = 0x81;
int cmdReg = 0x83;//command register
int ctrl = 0x82;//control address for Mode 0 op


void setup(){ 
  Serial.begin(9600);
  IOWrite(cmdReg, ctrl);
  Serial.println("Pressed Keys:");
}

void setPins(){
  pinMode(READ_PIN, OUTPUT);
  pinMode(WRITE_PIN, OUTPUT);
  pinMode(ALE_PIN, OUTPUT);
  pinMode(MIO_PIN, OUTPUT);
    
  digitalWrite(READ_PIN, HIGH); delay(100);
  digitalWrite(WRITE_PIN, HIGH); delay(100);
  digitalWrite(ALE_PIN, LOW); delay(100);
  digitalWrite(MIO_PIN, LOW); delay(100);
}

void IOWrite(byte address, byte data){

  setPins();
  //set addr pins
  for(int i = 0; i < 8; i++) {
    pinMode(ad_pins[i], OUTPUT); 
  }
  //setting addr
  for(int i = 0; i < 8; i++){
    if(bitRead(address, i) == 1){
      digitalWrite(ad_pins[i], HIGH);
    }
    else if (bitRead(address, i) == 0){
      digitalWrite(ad_pins[i], LOW);
    }
  }
  //latch
  digitalWrite(ALE_PIN, HIGH);
  digitalWrite(ALE_PIN, LOW);  
  
  //writing
  for(int i = 0; i < 8; i++){
    if(bitRead(data, i) == 1){
      digitalWrite(ad_pins[i], HIGH);
    }
    else if (bitRead(data, i) == 0){
      digitalWrite(ad_pins[i], LOW);
    }
  }

  //set write pins
  digitalWrite(WRITE_PIN, LOW);
  digitalWrite(WRITE_PIN, HIGH);

  setPins(); //reset
}

byte IORead(byte address){
  //setting pins
  setPins();
  byte data;
  
  //set addr pin - OUTPUT
  for(int i = 0; i < 8; i++){
    pinMode(ad_pins[i], OUTPUT); //set address pins to input
  }

  //send addr
  for(int i=0; i<8; i++){
    if(bitRead(address, i) == 1){
      digitalWrite(ad_pins[i], HIGH);
    }
    else if(bitRead(address, i) == 0){
      digitalWrite(ad_pins[i], LOW);
    }
  }
  
  //latch addr
  digitalWrite(ALE_PIN, HIGH);
  digitalWrite(ALE_PIN, LOW);

  //set addr bus to INPUT
  for(int i=0; i<8; i++){
    pinMode(ad_pins[i], INPUT);
  }

  delay(100);

  //reading
  digitalWrite(READ_PIN, LOW);
  for(int i = 0; i < 8; i++){
    bitWrite(data, i, digitalRead(ad_pins[i]));
  }

  digitalWrite(READ_PIN, HIGH);
  return data;  

  setPins(); //reset
}


void keypad(byte data){
  IOWrite(portA, data);
  byte col = IORead(portB);
  byte row = IORead(portA);

  switch(row){
    case 1:
      switch(col){
        case 1:
          Serial.println("1");
          break;
        case 2:
          Serial.println("2");
          break;
        case 4:
          Serial.println("3");
          break;
        case 8:
          Serial.println("A");
          break;
      }
      break;
    case 2:
      switch(col){
        case 1:
          Serial.println("4");
          break;
        case 2:
          Serial.println("5");
          break;
        case 4:
          Serial.println("6");
          break;
        case 8:
          Serial.println("B");
          break;
      }
      break;
    case 4:
      switch(col){
        case 1:
          Serial.println("7");
          break;
        case 2:
          Serial.println("8");
          break;
        case 4:
          Serial.println("9");
          break;
        case 8:
          Serial.println("C");
          break;
      }
      break;
    case 8:
      switch(col){
        case 1:
          Serial.println("*");
          break;
        case 2:
          Serial.println("0");
          break;
        case 4:
          Serial.println("#");
          break;
        case 8:
          Serial.println("D");
          break;
      }
      break;
  }
}


void loop() {
  keypad(0x01);//ROW 1
  keypad(0x02);//ROW 2
  keypad(0x04);//ROW 3
  keypad(0x08);//ROW 4
}