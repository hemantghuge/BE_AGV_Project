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
float normal = 0;
float slight = 15;
float extreme = 17;
float VL = 0;
int Buzzer = 49;
int servo = 12;
int count = 1;

//variables for serial 3
float angle = 0;
String qr;
int x_pos = 0;

int last_angle;
int change_in_angle;
int t_angle;

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
float V = 80;
float timeStep = 0.01;
float error = 0;
float PID = 0;
float P, I, D;
float preverror = 0;

float kp_straight = 2;
float ki_straight = 0.00005;
float kd_straight = 2;

float kp_angle = 0.2;
float ki_angle = 0;
float kd_angle = 0.2;

float kp;
float ki;
float kd;

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
  Serial3.begin(115200); // Raspberry_pi_serial
  Serial3.setTimeout(3);
  Serial.begin(9600);
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

int Straight = 2;

void loop() {

  timer = millis();
  timestep = (timer - tp) / 1000.0;
  tp = timer;
  gyro.getAngularVelocity(&avx, &avy, &avz);
  reading = (avz) * 0.07;
  if (abs(avz) < 25)reading = 0;
  yaw = yaw + reading * timestep;
  //  Serial.print("YAW:");
  //  Serial.println(yaw);


  Serial.print("TIME:");
  Serial.println(timestep);

  //  if (SER_IP == 'X')
  //  {
  //    setpoint = (setpoint + 90);
  //  }

  if (Serial3.available()) {


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

    angle = angle_str.toFloat();
    qr = qr_str;
    x_pos = x_pos_str.toInt();
  }

  Serial.println("DATA RECEIVED");
  Serial.print("angle:");
  Serial.println(angle);
  Serial.print("qr:");
  Serial.println(qr);
  Serial.print("x_pos:");
  Serial.println(x_pos);
  Serial.println("DATA RECEIVED");

  change_in_angle = angle - last_angle;
  last_angle = angle;

  Serial.print("change:");
  Serial.println(change_in_angle);


  if (abs(angle) < 10 && (millis() - t_angle >= 1000))
  {
    t_angle = millis();
    kp = kp_straight;
    ki = ki_straight;
    kd = kd_straight;
    setpoint = setpoint - angle;
    Straight = 1;
    //change_in_angle = 0;

  }
  if (abs(angle) > 10 && (millis() - t_angle >= 1000))
  {
    t_angle = millis();
    kp = kp_angle;
    ki = ki_angle;
    kd = kd_angle;
    Straight = 0;
    setpoint = setpoint - angle;
    //change_in_angle = 0;
  }

  //  if (change_in_angle > 10 || change_in_angle < -10 && (millis() - t_angle >= 1000))
  //  {
  //    t_angle = millis();
  //    kp =
  //    setpoint = setpoint - angle;
  //    //change_in_angle = 0;
  //  }

  //  if (SER_IP == 'F')
  //  {
  //    theta = 90 * M_PI / 180;
  //    count = 1;
  //  }
  //  if (SER_IP == 'B')
  //  {
  //    theta = 270 * M_PI / 180;
  //    count = 1;
  //  }

  //  if (x_pos == 1) // Bot at extreme left
  //  {
  //    VL = extreme;
  //    theta = 180 * M_PI / 180; //shift bot right
  //  }
  //
  //  else if (x_pos == 2) // Bot at slight left
  //  {
  //    VL = slight;
  //    theta = 180 * M_PI / 180; //shift bot right
  //    Serial.println("X POS 2");
  //  }
  //
  //  else if (x_pos == 3) // Bot at center
  //  {
  //    VL = normal;
  //    theta = 90 * M_PI / 180;
  //  }
  //
  //  else if (x_pos == 4) // Bot at slight right
  //  {
  //    VL = slight;
  //    theta = 0 * M_PI / 180;
  //  }
  //
  //  else if (x_pos == 5) // Bot at extreme right
  //  {
  //    VL = extreme;
  //    theta = 0 * M_PI / 180;
  //  }

  //  if (SER_IP == 'L')
  //  {
  //    theta = 180 * M_PI / 180;
  //    count = 1;
  //  }
  //  if (SER_IP == 'R')
  //  {
  //    theta = 0 * M_PI / 180;
  //    count = 1;
  //  }
  //  if (SER_IP == 'G')
  //  {
  //    theta = 135 * M_PI / 180;
  //    count = 1;
  //  }
  //  if (SER_IP == 'I')
  //  {
  //    theta = 45 * M_PI / 180;
  //    count = 1;
  //  }
  //  if (SER_IP == 'H')
  //  {
  //    //    theta = 225 * M_PI / 180;
  //    //    count = 1;
  //    digitalWrite(servo, HIGH);
  //    delayMicroseconds(2300);
  //    digitalWrite(servo, LOW);
  //    delay(20);
  //
  //  }

  //  if (SER_IP == 'J')
  //  {
  //    //    theta = 315 * M_PI / 180;
  //    //    count = 1;
  //    digitalWrite(servo, HIGH);
  //    delayMicroseconds(1500);
  //    digitalWrite(servo, LOW);
  //    delay(20);
  //  }

  //  if (SER_IP >= '1' && SER_IP <= '9')
  //  {
  //    VL = SER_IP ;
  //    count = 1;
  //  }
  //  if (SER_IP == 'W')
  //  {
  //    digitalWrite(Green, HIGH);
  //    digitalWrite(Red, HIGH);
  //    digitalWrite(Blue, HIGH);
  //    count = 1;
  //  }

  //  if (SER_IP == 'w')
  //  {
  //
  //    digitalWrite(Green, LOW);
  //    digitalWrite(Red, LOW);
  //    digitalWrite(Blue, LOW);
  //    count = 1;
  //
  //  }

  //  if (SER_IP == 'U')
  //  {
  //
  //    digitalWrite(Red, HIGH);
  //    count = 1;
  //
  //  }

  //  if (SER_IP == 'u')
  //  {
  //
  //    digitalWrite(Red, LOW);
  //    count = 1;
  //
  //  }

  //  if (SER_IP == 'V')
  //  {
  //    digitalWrite(Buzzer, LOW);
  //    count = 1;
  //  }
  //  if (SER_IP == 'v')
  //  {
  //    digitalWrite(Buzzer, HIGH);
  //    count = 1;
  //  }

  theta = 90 * M_PI / 180;
  VL = 20;
  Serial.print("VL:");
  Serial.println(VL);
  float V = map(VL , 0, 100, 0, 255);
  if (V == -1388)
  {
    V = 0;
  }

  error = yaw - setpoint;
  P = kp * error;
  I = I + (error * ki);
  D = kd * (error - preverror);
  PID = P + I + D;
  preverror = error;
  lcd.setCursor(0, 0);
  lcd.print("C_i_A=");
  lcd.print(change_in_angle);
  lcd.print(" yaw=");
  lcd.print(yaw);
  lcd.setCursor(0, 1);
  lcd.print("set =");
  lcd.print(setpoint);
  lcd.print(" ang=");
  lcd.print(angle);

  Serial.print("V:");
  Serial.println(V);


  Vx = V * cos(theta);
  Vy = - V * sin(theta);

  V1 = Vx + PID;
  V2 = -(Vx * 0.5 + Vy * 0.866) + PID ;
  V3 = -Vx * 0.5 + Vy * 0.866 + PID;

  Serial.print("V1:");
  Serial.println(V1);

  Serial.print("V2:");
  Serial.println(V2);

  Serial.print("V3:");
  Serial.println(V3);
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
