// Inclusion of libraries.
#include<math.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <L3GD20H.h>
#include <LiquidCrystal.h>

// GPIO pin declarations.
int PWM_F = 44;
int PWM_L = 46;
int PWM_R = 45;
int DR_F1 = 43;
int DR_F2 = 42;
int DR_L1 = 47;
int DR_L2 = 48;
int DR_R1 = 41;
int DR_R2 = 40;
int RS = 26;
int EN = 27;
int D4 = 28;
int D5 = 29;
int D6 = 30;
int D7 = 31;
int Green = 23;
int Red = 24;
int Blue = 25;
int normal = 9;
int slight = 7;
int extreme = 9;
int VL;
int Buzzer = 49;
int servo = 12;
int count = 1;

//variables for serial 3
byte angle_str = 0;
int angle;
byte sign_str = 0;
int sign;
String qr;
int x_pos;

// LCD pin declaration
LiquidCrystal lcd(26, 27, 28, 29, 30, 31);
// Object declarations
L3GD20H gyro;
// Variable Declarations
int16_t avx, avy, avz;
long int timer , tp;
float timestep, yaw;
float reading = 0;
char SER_IP = 'S';
float theta = 0 * M_PI / 180;
float V1, V2, V3;
float Vx, Vy;
float V;
float timeStep = 0.01;
float error = 0;
float PID = 0;
float P, I, D;
float preverror = 0;
float kp = 10;
float ki = 0.0001;
float kd = 20 ;
float setpoint = 0;
// Function declarations
void Clockwise();
void AntiClockwise();
void V1_Clockwise();
void V2_Clockwise();
void V3_Clockwise();
void V1_AntiClockwise();
void V2_AntiClockwise();
void V3_AntiClockwise();
void setup()
{
  /*
    Function Name:   pinMode(pin, mode)
    Input:    pin: the Arduino pin number to set the mode of.
              mode: INPUT, OUTPUT, or INPUT_PULLUP.
    Output:     Nothing
    Logic:    The code makes the digital "pin" available for either INPUT/INPUT_PULLUP or OUTPUT.
    Example Call:   pinMode(PWM_F, OUTPUT);
  */

  pinMode(PWM_F, OUTPUT);
  pinMode(DR_F1, OUTPUT);
  pinMode(DR_F2, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DR_L1, OUTPUT);
  pinMode(DR_L2, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(DR_R1, OUTPUT);
  pinMode(DR_R1, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(servo, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(Buzzer, HIGH);

  Wire.begin();
  Serial3.begin(9600); // Raspberry_pi_serial
  Serial3.setTimeout(10);
  Serial.begin(115200);
  Serial.println("Initializing I2C devices...");
  gyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(gyro.testConnection() ? "L3GD20H connection successful" : "L3GD20H connection failed");
  gyro.setEndianMode(not gyro.getEndianMode());
  Serial.print("EndianMode: ");
  Serial.print(gyro.getEndianMode());
  gyro.setEndianMode(not gyro.getEndianMode());
  Serial.print(" EndianMode: ");
  Serial.println(gyro.getEndianMode());
  gyro.setFullScale(2000);
  gyro.setOutputDataRate(100);
  gyro.setBlockDataUpdateEnabled(1);
  lcd.setCursor(0, 0);
  lcd.print("         AGV");
}


void loop() {

  timer = millis();
  timestep = (timer - tp) / 1000.0;
  tp = timer;
  gyro.getAngularVelocity(&avx, &avy, &avz);
  reading = (avz) * 0.07;
  if (abs(avz) < 25)reading = 0;
  yaw = yaw + reading * timestep;

  if (Serial3.available()) {
    String angle_str = Serial3.readString();
    angle = angle_str.toInt();
    delay(20);
    String sign_str = Serial3.readString();
    sign = sign_str.toInt();
    delay(20);
    qr = Serial3.readString();
    delay(20);
    String x_pos_str = Serial3.readString();
    x_pos = x_pos_str.toInt();

    if (sign == 0) {
      angle = -angle;
    }
    else if (sign == 1) {
      angle = angle;
    }
    else {
      Serial.println("Sign Error");
    }
  }

  Serial.println("character received: ");
  Serial.println(angle);
  Serial.println(qr);
  Serial.println(x_pos);


  theta = 90 * M_PI / 180;
  V = 50;
  if (angle < 20 && angle > -20)
  {
    setpoint = 0;
  }
  else if (angle > 20  && angle < 40 || angle < -20  && angle > -40)
  {
    setpoint = - 90;
  }
  else if (angle > 40  && angle < 80 || angle < -40  && angle > -80)
  {
    setpoint = -60;
  }
  //
  //  if (x_pos == 1) // Bot at extreme left
  //  {
  //    V1 = normal;
  //    V1_AntiClockwise();
  //    V2_Clockwise();
  //  }
  //
  //  else if (x_pos == 2) // Bot at slight left
  //  {
  //    V1 = normal;
  //    V1_AntiClockwise();
  //    V2_Clockwise();
  //  }
  //
  //  else if (x_pos == 3) // Bot at center
  //  {
  //    Forward();
  //  }
  //
  //  else if (x_pos == 4) // Bot at slight right
  //  {
  //    V1 = normal;
  //    V1_Clockwise();
  //    V3_AntiClockwise();
  //  }
  //
  //  else if (x_pos == 5) // Bot at extreme right
  //  {
  //    V1 = normal;
  //    V1_Clockwise();
  //    V3_AntiClockwise();
  //  }

  error = yaw - setpoint;


  Serial.println("Setpoint");
  Serial.println(setpoint);

  P = kp * error;
  I = I + (error * ki);
  D = kd * (error - preverror);
  PID = P + I + D;
  preverror = error;
  
  lcd.setCursor(0, 0);
  lcd.print(x_pos);
  lcd.print("   ANGLE   ");
  lcd.print(angle);
  lcd.print("       ");
  lcd.setCursor(0, 1);
  lcd.print(V);
  lcd.print("  ");
  lcd.print(V1);


  Vx = V * cos(theta);
  Vy = - V * sin(theta);

  V1 = Vx + PID;
  V2 = -(Vx * 0.5) + (Vy * 0.866) + PID;
  V3 = -Vx * 0.5 + Vy * 0.866 + PID;


  if (V1 >= 0)
  {
    V1_Clockwise();
  }
  else
  {
    V1_AntiClockwise();
  }
  if (V2 >= 0)
  {
    V2_Clockwise();
  }
  else
  {
    V2_AntiClockwise();
  }
  if (V3 >= 0)
  {
    V3_Clockwise();
  }
  else
  {
    V3_AntiClockwise();
  }


}
//
//void Clockwise()
//{
//  V1_Clockwise();
//  V2_Clockwise();
//  V3_Clockwise();
//  Serial.println("Clockwise");
//}
//
//
//void AntiClockwise()
//{
//  V1_AntiClockwise();
//  V2_AntiClockwise();
//  V3_AntiClockwise();
//  Serial.println("AntiClockwise");
//}

//void Forward()
//{
//
//  V2_Clockwise();
//  V3_AntiClockwise();
//  Serial.println("Forward");
//}
/*
    Function Name: V1_Clockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V1_Clockwise()
*/
void V1_Clockwise()
{
  analogWrite(PWM_F, abs(V1));
  digitalWrite(DR_F1, HIGH);
  digitalWrite(DR_F2, LOW);

}
/*
    Function Name: V2_Clockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V2_Clockwise()
*/
void V2_Clockwise()
{

  analogWrite(PWM_L, abs(V2));
  digitalWrite(DR_L1, HIGH);
  digitalWrite(DR_L2, LOW);


}
/*
    Function Name: V3_Clockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V3_Clockwise()
*/
void V3_Clockwise()
{

  analogWrite(PWM_R, abs(V3));
  digitalWrite(DR_R1, HIGH);
  digitalWrite(DR_R2, LOW);

}
/*
    Function Name: V1_AntiClockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V1_AntiClockwise();
*/

void V1_AntiClockwise()
{
  analogWrite(PWM_F, abs(V1));
  digitalWrite(DR_F1, LOW);
  digitalWrite(DR_F2, HIGH);
}
/*
    Function Name: V2_AntiClockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V2_AntiClockwise();
*/
void V2_AntiClockwise()
{

  analogWrite(PWM_L, abs(V2));
  digitalWrite(DR_L1, LOW);
  digitalWrite(DR_L2, HIGH);
}
/*
    Function Name: V3_AntiClockwise()
    Input: None
    Output: None
    Logic: For driving motor in Clockwise direction.
    Example Call: V3_AntiClockwise();
*/
void V3_AntiClockwise()
{
  analogWrite(PWM_R, abs(V3));
  digitalWrite(DR_R1, LOW);
  digitalWrite(DR_R2, HIGH);
}
