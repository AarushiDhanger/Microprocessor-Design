#define PIN_INT 2 
#define PIN_M_IO 51
#define PIN_ALE 49
#define PIN_WRITE 45
#define PIN_READ 43

static  uint8_t AD_PINS[8]={52,50,48,46,44,42,40,38};
#define AD_PINS_SIZE (sizeof (AD_PINS) / sizeof (AD_PINS[0]))

//set ALE & M_IO signals = active HIGH
#define ALE_DISABLE() digitalWrite(PIN_ALE, LOW);
#define ALE_ENABLE() digitalWrite(PIN_ALE, HIGH);
#define M_IO_DISABLE() digitalWrite(PIN_M_IO, LOW);
#define M_IO_ENABLE() digitalWrite(PIN_M_IO, HIGH);

//set RD & WR signals = active LOW
#define WRITE_DISABLE() digitalWrite(PIN_WRITE, HIGH);
#define WRITE_ENABLE() digitalWrite(PIN_WRITE, LOW);
#define READ_DISABLE() digitalWrite(PIN_READ, HIGH);
#define READ_ENABLE() digitalWrite(PIN_READ, LOW);

void memWrite(uint8_t addr, uint8_t data);
void memRead(uint8_t addr);
void write_byte(uint8_t b);
void read_byte(uint8_t b);
void address_latch(uint8_t addr);


void setup() 
{ 
  pinMode (38,INPUT); //AD0
  pinMode (40,INPUT); //AD1
  pinMode (42,INPUT); //AD2
  pinMode (44,INPUT); //AD3
  pinMode (46,INPUT); //AD4
  pinMode (48,INPUT); //AD5
  pinMode (50,INPUT); //AD6
  pinMode (52,INPUT); //AD7
  pinMode(PIN_READ, OUTPUT);
  pinMode(PIN_WRITE, OUTPUT);
  pinMode(PIN_ALE, OUTPUT);
  pinMode(PIN_M_IO, OUTPUT);
  Serial.begin(9600);




// SRAM1
Serial.println("Read SRAM1: Address 30");
Serial.println("Wanted Output: 0xAA = 10101010");
memWrite(0x30,0xAA); 
memRead(0x30);
delay(1000);
digitalWrite(PIN_READ, LOW);

// SRAM2
Serial.println("Read SRAM2: Address 30");
Serial.println("Wanted Output: 0xFF = 11111111");
memWrite(0x44,0xFF); 
memRead(0x44);
delay(1000);
digitalWrite(PIN_READ, LOW);

} 


void write_byte(uint8_t b)
{
  int i;
  for (i = 0; i < AD_PINS_SIZE; i++) 
  {
   pinMode(AD_PINS[i], OUTPUT);  
  }
  for (i = 0; i < AD_PINS_SIZE; i++)
  {
    digitalWrite(AD_PINS[i], ((b >> (AD_PINS_SIZE-1-i)) & 1));
  }
}

void read_byte(uint8_t b)
{
  int i;
  uint8_t d; // = 0b00000000;
  for (i = 0; i < AD_PINS_SIZE; i++)
  {
    pinMode(AD_PINS[i], INPUT);
    d = digitalRead(AD_PINS[i]);
    Serial.print(d);
  }
  Serial.print("\n");
}
   

void memWrite(uint8_t addr, uint8_t data)
{
  READ_DISABLE();
  M_IO_ENABLE();

  //latch address.
  address_latch(addr);

  //SRAM accepts data from bus
  WRITE_ENABLE();

  //put data onto data bus
  write_byte(data);

  delay(100);
  WRITE_DISABLE();  
}


 void memRead(uint8_t addr){
  WRITE_DISABLE();
  M_IO_ENABLE();

  address_latch(addr);

  READ_ENABLE();

  read_byte(addr);

  delay(100);
  READ_DISABLE();
}

void address_latch(uint8_t addr){
  // Prepare to latch an address
  ALE_ENABLE();
  // Place addr on data bus
  write_byte(addr); 
  delay(50);
  // Diable ALE, future WR to bus isn't latched
  ALE_DISABLE();
}

void loop() { 
 
} 