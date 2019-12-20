#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
double CELL_1 = A2;
double CELL_2 = A1;
double CELL_3 = A0;
double E1, E2, E3;
double V1, V2, V3;
void setup() {
  Serial.begin(115200);
  pinMode(CELL_1, INPUT);
  pinMode(CELL_2, INPUT);
  pinMode(CELL_3, INPUT);
  pinMode(13, OUTPUT);

  lcd.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back liglcd.clear();//Clean the screen
}

void loop() {
  digitalWrite(13, HIGH);

  V1 = analogRead(CELL_1);
  V2 = analogRead(CELL_2);
  V3 = analogRead(CELL_3);

  E1 = V1 * 0.005760368664-0.03;
  E2 = (V2 * 0.0107110917+0.32) - E1;
  E3 = (V3 * 0.01318167007+0.26) - (E1 + E2) ;

  Serial.print("V1 =  ");
  Serial.print(V1);
  Serial.print("   V2 =  ");
  Serial.print(V2);
  Serial.print("   V3 =  ");
  Serial.print(V3);


  Serial.print("   E1 =  ");
  Serial.print(E1);
  Serial.print("   E2 =  ");
  Serial.print(E2);
  Serial.print("   E3 =  ");
  Serial.print(E3);


  Serial.print("    E1 =  ");
  Serial.print(E1);
  Serial.print("  E1+ E2 =  ");
  Serial.print(E1 + E2);
  Serial.print("  E1+ E2+ E3 =  ");
  Serial.println(E1 + E2 + E3);
  lcd.setCursor(0, 0);

  
  Serial.print(E1);
  Serial.print("  ");
  Serial.print(E1 + E2);
  lcd.setCursor(0, 1);
  Serial.println(E1 + E2 + E3);

  delay(500);
}
