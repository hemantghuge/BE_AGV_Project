// Parallel communication between Arduino Mega and Raspberry Pi

// ra = raspi to arduino mega data transmission
int ra1 = A4;
int ra2 = A5;
int ra3 = A6;
int ra4 = A7;

int ra5 = A8;
int ra6 = A9;
int ra7 = A10;
int ra8 = A11;

// ar = arduino mega to raspi data transmission 
int ar1 = 32;
int ar2 = 33;
int ar3 = 34;
int ar4 = 35;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Enabling raspi to arduino mega pins as OUTPUT
  pinMode(ra1, INPUT);
  pinMode(ra2, INPUT);
  pinMode(ra3, INPUT);
  pinMode(ra4, INPUT);

  pinMode(ra5, INPUT);
  pinMode(ra6, INPUT);
  pinMode(ra7, INPUT);
  pinMode(ra8, INPUT);

  // Enabling arduino mega to raspi pins as INPUT
  pinMode(ar1, OUTPUT);
  pinMode(ar2, OUTPUT);
  pinMode(ar3, OUTPUT);
  pinMode(ar4, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {

  int signal_value = HIGH;
  // read the input pin:
  byte ra1State = digitalRead(ra1);
  byte ra2State = digitalRead(ra2);
  byte ra3State = digitalRead(ra3);
  byte ra4State = digitalRead(ra4);

  byte ra5State = digitalRead(ra5);
  byte ra6State = digitalRead(ra6);
  byte ra7State = digitalRead(ra7);
  byte ra8State = digitalRead(ra8);

  digitalWrite(ar1, signal_value);
  digitalWrite(ar2, signal_value);
  digitalWrite(ar3, signal_value);
  digitalWrite(ar4, signal_value);
    
  // print out the state of the pin:
  Serial.print(" ra1State = ");
  Serial.print(ra1State);
  Serial.print(" ra2State = ");
  Serial.print(ra2State);
  Serial.print(" ra3State = ");
  Serial.print(ra3State);
  Serial.print(" ra4State = ");
  Serial.print(ra4State);

  Serial.print(" ra5State =");
  Serial.print(ra5State);
  Serial.print(" ra6State = ");
  Serial.print(ra6State);
  Serial.print(" ra7State = ");
  Serial.print(ra7State);
  Serial.print(" ra8State = ");
  Serial.println(ra8State);
  delay(1);        // delay in between reads for stability
}
