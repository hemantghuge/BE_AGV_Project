byte number = 0;

void setup(){
Serial3.begin(9600);
Serial.begin(9600);
}

void loop(){
if (Serial3.available()) {
String number = Serial3.readString();
int num = number.toInt();
Serial.print("character recieved: ");
Serial.println(number);
Serial.println(num);
}
}
