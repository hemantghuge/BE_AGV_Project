byte angle = 0;
int ang;

byte sign = 0;
int sig;

void setup(){
Serial3.begin(9600);
Serial.begin(9600);
Serial3.setTimeout(10);
}

void loop(){
if (Serial3.available()) {
String angle = Serial3.readString();
int ang = angle.toInt();
delay(20);
String sign = Serial3.readString();
int sig = sign.toInt();
Serial.println("character received: ");
Serial.println(ang);
Serial.println(sig);

}
}
