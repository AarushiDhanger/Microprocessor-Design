int ALE_PIN = 49;
int MIO_PIN = 51;
int READ_PIN = 43;
int WRITE_PIN = 45;
int INT_PIN = 2; //reads interrupt signal

int ad_pins[8] = {38,40,42,44,46,48,50,52};

int portA = 0x80;
int portB = 0x81;
int ctrl = 0x82;
int cmdReg = 0x83;

void setup() { 
  Serial.begin(9600);
  IOWrite(cmdReg, ctrl); 
  IOWrite(0x80, 0xFF);
  
  //read the interrupt signal 
  pinMode(INT_PIN, INPUT_PULLUP);
    Serial.println("Pressed Keys:");
  attachInterrupt(digitalPinToInterrupt(INT_PIN), Interrupt, RISING);
}

void setPins() {
  pinMode(READ_PIN, OUTPUT);
  pinMode(WRITE_PIN, OUTPUT);
  pinMode(ALE_PIN, OUTPUT);
  pinMode(MIO_PIN, OUTPUT);

  digitalWrite(READ_PIN, HIGH);
  digitalWrite(WRITE_PIN, HIGH);
  digitalWrite(ALE_PIN, LOW);
  digitalWrite(MIO_PIN, LOW);
}

void IOWrite(byte addr, byte data) {
  setPins();
  //set addr pins
  for(int i = 0; i < 8; i++) {
    pinMode(ad_pins[i], OUTPUT);
  }
  //setting addr
  for(int i = 0; i < 8; i++) {
    if(bitRead(addr, i) == 1) {
      digitalWrite(ad_pins[i], HIGH);
    }
    else if(bitRead(addr, i) == 0) {
      digitalWrite(ad_pins[i], LOW);
    }
  }
  //latch  
  digitalWrite(ALE_PIN, HIGH);
  digitalWrite(ALE_PIN, LOW);

  //writing
  for(int i = 0; i < 8; i++) {
      if(bitRead(data, i) == 1) {
        digitalWrite(ad_pins[i], HIGH);
      }
      else if(bitRead(data, i) == 0) {
        digitalWrite(ad_pins[i], LOW);
      }
    }
  
  //set write pins
  digitalWrite(WRITE_PIN, LOW);
  digitalWrite(WRITE_PIN, HIGH);
}

byte IORead (byte addr) {
  //setting pins
  byte data;
  setPins();

  //set addr pin - OUTPUT
  for(int i = 0; i < 8; i++) {
    pinMode(ad_pins[i], OUTPUT);
  }

  //send addr
  for(int i = 0; i < 8; i++) {
    if(bitRead(addr, i) == 1) {
      digitalWrite(ad_pins[i], HIGH);
    }
    else if(bitRead(addr, i) == 0) {
      digitalWrite(ad_pins[i], LOW);
    }
  }

  //latch addr
  digitalWrite(ALE_PIN, HIGH);
  digitalWrite(ALE_PIN, LOW);

  //set addr bus to INPUT
  for(int i = 0; i < 8; i++) {
    pinMode(ad_pins[i], INPUT);
  }

  delay(100);

  //reading
  digitalWrite(READ_PIN, LOW);
  for(int i = 0; i < 8; i++) {
    bitWrite(data, i, digitalRead(ad_pins[i]));
  }

  digitalWrite(READ_PIN, HIGH);
  return data;
}

void Interrupt() {
  Serial.println("Interrupt detected");

  IOWrite(0x80,0x1); //scan col 1
  //checks if key pressed in col 1
  if(IORead(0x81) != 0x00){
    keypad(0x1);
  }
  
  IOWrite(0x80,0x2); //scan col 2
  //checks if key pressed in col 2
  if(IORead(0x81) != 0x00){
    keypad(0x2);
  }
  
  IOWrite(0x80,0x4); //scan col 3
  //checks if key pressed in col 3
  if(IORead(0x81) != 0x00){
    keypad(0x4);
  }

  IOWrite(0x80,0x8); //scan col 4
  //checks if key pressed in col 4
  if(IORead(0x81) != 0x00){
    keypad(0x8);
  }
  
  IOWrite(0x80, 0xFF); //sets all columns to low
}

void keypad(byte data){
  IOWrite(portA, data);
  byte col = IORead(portB);
  byte row = IORead(portA);
  
  switch (col) {
    case 0x1:
      switch (row) {
        case 0x1:
          Serial.println("1");
          break;
        case 0x2:
          Serial.println("4");
          break;
        case 0x4:
          Serial.println("7");
          break;
        case 0x8:
          Serial.println("*");
          break;
      }
      break;
      
    case 0x2:
      switch (row) {
        case 0x1:
          Serial.println("2");
          break;
        case 0x2:
          Serial.println("5");
          break;
        case 0x4:
          Serial.println("8");
          break;
        case 0x8:
          Serial.println("0");
          break;
      }
      break;
      
    case 0x4:
      switch (row) {
        case 0x1:
          Serial.println("3");
          break;
        case 0x2:
          Serial.println("6");
          break;
        case 0x4:
          Serial.println("9");
          break;
        case 0x8:
          Serial.println("#");
          break;
      }
      break;
      
    case 0x8:
      switch (row) {
        case 0x1:
          Serial.println("A");
          break;
        case 0x2:
          Serial.println("B");
          break;
        case 0x4:
          Serial.println("C");
          break;
        case 0x8:
          Serial.println("D");
          break;
      }
      break;
    
    default:
      break;
  }

}

void loop()
{
  Serial.println("No Interrupt detected");
  delay(1000);
}