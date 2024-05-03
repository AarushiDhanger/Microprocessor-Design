#define RD_PIN 43
#define WE_PIN 45
#define ALE_PIN 49
#define MIO_PIN 51

static  uint8_t ad[8] = {38,40,42,44,46,48,50,52};
int cmdReg = 0x83;  
int ctrl = 0x82; //defines mode 0 - 1000 0010

int row;
int column;
char keypad[4][4] = {
  {'A', '3', '2', '1'},  
  {'B', '6', '5', '4'},
  {'C', '9', '8', '7'},
  {'D', '#', '0', '*'}
};

void setPins() {
  pinMode(RD_PIN, OUTPUT);
  pinMode(WE_PIN, OUTPUT);
  pinMode(ALE_PIN, OUTPUT);
  pinMode(MIO_PIN, OUTPUT);
    
  digitalWrite(RD_PIN, HIGH);
  delay(100);
  digitalWrite(WE_PIN, HIGH);
  delay(100);
  digitalWrite(ALE_PIN, LOW);
  delay(100);
  digitalWrite(MIO_PIN, LOW);
  delay(100);
}
void setup() {

  Serial.begin(9600);
  Serial.println("\nCommand Register: ");
  IOwrite(cmdReg, ctrl);
  Serial.println("\nPort A: "); 
  IOwrite(0x80, 0x55); 
  //55 - 0101 0101 <=real val (active low so read flipped)
  //AA - 1010 1010 
  Serial.println("\nPort B: ");
  byte data;
  Serial.println(IOread(0x81), HEX); 
}

void IOwrite(byte address, byte data){
  
  setPins();
  //set addr pins
  for(int i = 0; i < 8; i++){
    pinMode(ad[i], OUTPUT);
  }

  //setting addr
  for (int i = 0; i < 8; i++){
    digitalWrite(ad[i], bitRead(address, i));
  }

  //latching
  digitalWrite(ALE_PIN, HIGH); 
  delay(300);
  digitalWrite(ALE_PIN, LOW);
  delay(100);

  //writing 
  for (int i = 0; i < 8; i++){
    digitalWrite(ad[i], bitRead(data, i));
  }
  //start writing
  delay(100);
  digitalWrite(WE_PIN, LOW); 
  delay(100);
  digitalWrite(WE_PIN, HIGH);

  Serial.begin(9600);
  Serial.print("Address: 0x");
  Serial.print(address, HEX);
  Serial.print(" \nData: 0x");
  Serial.print(data, HEX);
  
  setPins(); 
}

byte IOread(byte address){
  //setting pins
  setPins();
  byte data;

  //set addr pin - OUTPUT
  for(int i = 0; i < 8; i++){
    pinMode(ad[i], OUTPUT);
  }

  //send add
  for (int i = 0; i < 8; i++){
    digitalWrite(ad[i], bitRead(address, i));
  }

  //latch addr
  digitalWrite(ALE_PIN, HIGH); 
  delay(300);
  digitalWrite(ALE_PIN, LOW);
  delay(100);

  //set addr data bus to INPUT  
  for(int i = 0; i < 8; i++){
    //confirms Port B isn't reading 
    //garbage values from arduino
    digitalWrite(ad[i], LOW); 
    //set to INPUT
    pinMode(ad[i], INPUT);
  }

  for (int i = 0; i < 8; i++){
    digitalWrite(ad[i], bitRead(data, i));
  }

  //reading
  digitalWrite(RD_PIN, LOW);
  delay(300);

  Serial.begin(9600);
  Serial.print("Address: 0x");
  Serial.print(address, HEX);
  Serial.print(" \nData: 0x");
  
  for(int i = 0; i < 8; i++){
    bitWrite(data, i, digitalRead(ad[i])); 
  }
  digitalWrite(RD_PIN, HIGH);
  return data;
  setPins();
  
}


void loop() {

}
