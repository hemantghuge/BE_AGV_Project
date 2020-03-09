void setup() {

  Serial.begin(9600);
  Serial3.begin(115200);
  Serial3.setTimeout(3);

}

void loop()
{
  if (Serial3.available()) {
    Serial.println("Data received");
    String readString = Serial3.readString();

    // Split the readString by a pre-defined delimiter in a simple way. '%'(percentage) is defined as the delimiter in this project.
    int delimiter, delimiter_1, delimiter_2, delimiter_3;
    delimiter = readString.indexOf("%");
    delimiter_1 = readString.indexOf("%", delimiter + 1);
    delimiter_2 = readString.indexOf("%", delimiter_1 + 1);
    delimiter_3 = readString.indexOf("%", delimiter_2 + 1);

    // Define variables to be executed on the code later by collecting information from the readString as substrings.
    String angle_str = readString.substring(delimiter + 1, delimiter_1);
    String qr_str = readString.substring(delimiter_1 + 1, delimiter_2);
    String x_pos_str = readString.substring(delimiter_2 + 1, delimiter_3);

    int angle = angle_str.toInt();
    int qr = qr_str.toInt();
    int x_pos = x_pos_str.toInt();
    Serial.println(angle);
    Serial.println(qr);
    Serial.println(x_pos);
  }
}
