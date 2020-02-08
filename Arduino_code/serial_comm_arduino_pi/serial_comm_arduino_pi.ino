byte number = 0;

int num;
void setup(){
Serial3.begin(9600);
Serial.begin(9600);
Serial3.setTimeout(10);
}

void loop(){
if (Serial3.available()) {
String number = Serial3.readString();
int num = number.toInt();
Serial.println("character recieved: ");
Serial.println(number);
Serial.println(num);
}
}
