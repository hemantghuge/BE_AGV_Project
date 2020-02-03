
#include <TM1637Display.h>
#define CLK A5
#define DIO A4
double CELL_1 = A2;
double CELL_2 = A1;
double CELL_3 = A0;
double E1, E2, E3;
int ED1,ED2,ED3,ED_ALL,COUNT1=0,COUNT2=0;
double V1, V2, V3;
int time=1000; 
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
TM1637Display display(CLK, DIO);

const uint8_t SEG_CEL1[] = {
  SEG_A | SEG_F | SEG_E | SEG_D ,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G ,
  SEG_F | SEG_E | SEG_D ,
  SEG_B | SEG_C
};
const uint8_t SEG_CEL2[] = {
  SEG_A | SEG_F | SEG_E | SEG_D ,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G ,
  SEG_F | SEG_E | SEG_D ,
  SEG_A | SEG_B | SEG_G | SEG_E | SEG_D
};
const uint8_t SEG_CEL3[] = {
  SEG_A | SEG_F | SEG_E | SEG_D ,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G ,
  SEG_F | SEG_E | SEG_D,
  SEG_A | SEG_B | SEG_G | SEG_C | SEG_D
};
const uint8_t SEG_ALL[] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G ,
  SEG_F | SEG_E | SEG_D ,
  SEG_F | SEG_E | SEG_D ,
  SEG_G
  
};
void setup() {
  Serial.begin(115200);
  pinMode(CELL_1, INPUT);
  pinMode(CELL_2, INPUT);
  pinMode(CELL_3, INPUT);
  pinMode(13, OUTPUT);
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  display.setBrightness(0x0f);

}

void loop() {
  digitalWrite(13, HIGH);

  V1 = analogRead(CELL_1);
  V2 = analogRead(CELL_2);
  V3 = analogRead(CELL_3);

  E1 = V1 * 0.005760368664 + 0.48;
  E2 = (V2 * 0.0107110917 + 1.23) - E1;
  E3 = (V3 * 0.01318167007+1.7) - (E1 + E2) ;

  ED1=E1*100;
  ED2=E2*100;
  ED3=E3*100;
  ED_ALL=ED1+ED2+ED3;
  
  Serial.print("   E1 =  ");
  Serial.print(V1);
  Serial.print("   E2 =  ");
  Serial.print(V2);
  Serial.print("   E3 =  ");
  Serial.print(V3);
  Serial.print("   E1+ E2+ E3 =  ");
  Serial.println(E1 + E2 + E3);

   display.setSegments(SEG_CEL1);
   delay(time);
   display.showNumberDecEx(ED1, 0b11100000, false, 4, 0);
   delay(time);
   display.setSegments(SEG_CEL2);
   delay(time);
   display.showNumberDecEx(ED2, 0b11100000, false, 4, 0);
   delay(time);
   display.setSegments(SEG_CEL3);
   delay(time);
   display.showNumberDecEx(ED3, 0b11100000, false, 4, 0);
   delay(time);
   display.setSegments(SEG_ALL);
   delay(time);
   display.showNumberDecEx(ED_ALL, 0b11100000, false, 4, 0);
   delay(time);
//if (COUNT1 < 5)
//{
//  
// if (ED_ALL < 1130)
//   {
//    digitalWrite(13, LOW);
//    delay(300);
//    digitalWrite(13, HIGH);
//    delay(300);
//    }
//    COUNT1++;
//}
//if (COUNT2 < 5)
//{ 
//  
// if (ED_ALL >1190)
//   {
//    digitalWrite(13, LOW);
//    delay(1000);
//    digitalWrite(13, HIGH);
//    delay(1000);
//    }
//    COUNT2++;
//}
}
