#define PIN_INT 2
#define PIN_M_IO 51
void setup() 
{
  // put your setup code here, to run once:
  
  //52 = AD7, 38 = AD0
  int ad[8] = {38, 40, 42, 44, 46, 48, 50, 52};
  //ALE pin
  pinMode(49, OUTPUT);
  for(int i = 0; i < 8; ++i){
    pinMode(ad[i], OUTPUT);
  }
  digitalWrite(49, HIGH);


  //binary 14 00001110
  digitalWrite(ad[7], LOW);
  digitalWrite(ad[6], LOW);
  digitalWrite(ad[5], LOW);  
  digitalWrite(ad[4], LOW);
  digitalWrite(ad[3], HIGH);
  digitalWrite(ad[2], HIGH);
  digitalWrite(ad[1], HIGH);
  digitalWrite(ad[0], LOW);

  //address and data hold 14
  delay(100);

  //switch ALE back to low (0)
  digitalWrite(49, LOW);
  
  //binary 18 00010010
  digitalWrite(ad[7], LOW);
  digitalWrite(ad[6], LOW);
  digitalWrite(ad[5], LOW);
  digitalWrite(ad[4], HIGH);
  digitalWrite(ad[3], LOW);
  digitalWrite(ad[2], LOW);
  digitalWrite(ad[1], HIGH);
  digitalWrite(ad[0], LOW);

  //address holds 14
  //data hold 18
}

void loop() {
  // put your main code here, to run repeatedly:
}