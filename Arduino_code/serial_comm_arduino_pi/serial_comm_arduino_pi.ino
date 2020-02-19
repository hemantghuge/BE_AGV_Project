byte angle_str = 0;
int angle;
byte qr;

byte sign_str = 0;
int sign;

void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
  Serial3.setTimeout(10);
}

void loop() {
  if (Serial3.available()) {
    String angle_str = Serial3.readString();
    int angle = angle_str.toInt();
    delay(20);
    String sign_str = Serial3.readString();
    int sign = sign_str.toInt();
    delay(20);
    String qr = Serial3.readString();

    if (sign == 0) {
      angle = -angle;
    }
    else if (sign == 1) {
      angle = angle;
    }
    else {
      Serial.println("Sign Error");
    }


    Serial.println("character received: ");
    Serial.println(angle);
    Serial.println(qr);
  }
}
