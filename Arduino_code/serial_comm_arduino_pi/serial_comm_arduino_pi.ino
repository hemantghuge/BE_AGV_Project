byte number = 0;

void setup(){
Serial3.begin(9600);
Serial.begin(9600);
}

void loop(){
if (Serial3.available()) {
number = Serial3.read();
Serial.print("character recieved: ");
Serial.println(number);
}
}
