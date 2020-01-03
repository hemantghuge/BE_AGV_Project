#include<math.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <L3GD20H.h>
#include <LiquidCrystal.h>
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
int VL = 0;
int Buzzer = 49;
LiquidCrystal lcd(26, 27, 28, 29, 30, 31);
float reading = 0;
char SER_IP = 'S';
// specific I2C address may be passed here
L3GD20H gyro;

int16_t avx, avy, avz;
bool blinkState = false;
float delta = 0, threshold = 0;
long int timer , tp;
float timestep, yaw;
void Clockwise();
void AntiClockwise();
void V1_Clockwise();
void V2_Clockwise();
void V3_Clockwise();
void V1_AntiClockwise();
void V2_AntiClockwise();
void V3_AntiClockwise();

float theta = 0 * M_PI / 180;
float V1, V2, V3;
float Vx, Vy;
float V = 80;
float timeStep = 0.01;
float error = 0;
float PID = 0;
float P, I, D;
float preverror = 0;
float kp = 15;
float ki = 0.001;
float kd = 161 ;
float setpoint = 0;
int count;
void setup()
{
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
  lcd.begin(16, 2);
  digitalWrite(Buzzer, HIGH);
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(9600);
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
  Serial.println(yaw);
  if (SER_IP == 'X')
  {
    setpoint = (setpoint + 10);
  }

  if (Serial1.available() > 0)
  {
    SER_IP = Serial1.read();
  }
  if (SER_IP == 'F')
  {
    theta = 90 * M_PI / 180;
  }
  if (SER_IP == 'B')
  {
    theta = 270 * M_PI / 180;
  }
  if (SER_IP == 'L')
  {
    theta = 180 * M_PI / 180;
  }
  if (SER_IP == 'R')
  {
    theta = 0 * M_PI / 180;
  }
  if (SER_IP == 'G')
  {
    theta = 135 * M_PI / 180;
  }
  if (SER_IP == 'I')
  {
    theta = 45 * M_PI / 180;
  }
  if (SER_IP == 'H')
  {
    theta = 225 * M_PI / 180;
  }
  if (SER_IP == 'J')
  {
    theta = 315 * M_PI / 180;
  }

  if (SER_IP >= '1' && SER_IP <= '9')
  {
    VL = SER_IP ;
  }
  if (SER_IP == 'W')
  {
    digitalWrite(Green, HIGH);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, HIGH);
  }
  if (SER_IP == 'w')
  {

    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, LOW);

  }
  if (SER_IP == 'U')
  {

    digitalWrite(Red, HIGH);

  }
  if (SER_IP == 'u')
  {

    digitalWrite(Red, LOW);

  }
  if (SER_IP == 'V')
  {
    digitalWrite(Buzzer, LOW);
  }
  if (SER_IP == 'v')
  {
    digitalWrite(Buzzer, HIGH);
  }
  V = map(VL , 49, 58, 0, 255);
  if (V == -1388)
  {
    V = 0;
  }
  Serial.println(V);
  error = yaw - setpoint;
  P = kp * error;
  I = I + (error * ki);
  D = kd * (error - preverror);
  PID = P + I + D;
  preverror = error;
  lcd.setCursor(0, 0);
  lcd.print(V1 + PID);
  lcd.print("  ");
  lcd.print(yaw);
  lcd.setCursor(0, 1);
  lcd.print(V2 + PID);
  lcd.print("  ");
  lcd.print(V3 + PID);

  if (SER_IP == 'S')
  {
    V = 0;
  }
  Vx = V * cos(theta);
  Vy = - V * sin(theta);

  V1 = Vx + PID;
  V2 = -(Vx * 0.5 + Vy * 0.866) + PID ;
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
//  digitalWrite(Green, HIGH);
//    digitalWrite(Red, HIGH);
//   digitalWrite(Blue, HIGH);
}

void V1_Clockwise()
{
  analogWrite(PWM_F, abs(V1));
  digitalWrite(DR_F1, HIGH);
  digitalWrite(DR_F2, LOW);

}
void V2_Clockwise()
{

  analogWrite(PWM_L, abs(V2));
  digitalWrite(DR_L1, HIGH);
  digitalWrite(DR_L2, LOW);


}
void V3_Clockwise()
{

  analogWrite(PWM_R, abs(V3));
  digitalWrite(DR_R1, HIGH);
  digitalWrite(DR_R2, LOW);

}
void V1_AntiClockwise()
{
  analogWrite(PWM_F, abs(V1));
  digitalWrite(DR_F1, LOW);
  digitalWrite(DR_F2, HIGH);
}
void V2_AntiClockwise()
{

  analogWrite(PWM_L, abs(V2));
  digitalWrite(DR_L1, LOW);
  digitalWrite(DR_L2, HIGH);

}
void V3_AntiClockwise()
{
  analogWrite(PWM_R, abs(V3));
  digitalWrite(DR_R1, LOW);
  digitalWrite(DR_R2, HIGH);
}
//void AntiClockwise()
//{
//  analogWrite(PWM_F, abs(V1) );
//  digitalWrite(DR_F1, LOW);
//  digitalWrite(DR_F2, HIGH);
//  analogWrite(PWM_L, abs(V2));
//  digitalWrite(DR_L1, LOW);
//  digitalWrite(DR_L2, HIGH);
//  analogWrite(PWM_R, abs(V3) );
//  digitalWrite(DR_R1, LOW);
//  digitalWrite(DR_R2, HIGH);
//}
