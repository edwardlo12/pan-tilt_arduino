#include <Servo.h>
#include <math.h>
#define pi 3.1415926
#define deg2rad pi/180
#define rad2deg 180/pi


Servo pan;
Servo tilt;

int pan_pin = 2;      //PAN控制腳位
int tilt_pin = 3;     //TILT控制腳位
int x_max = 640;      //原始圖資寬
int y_max = 480;      //原始圖資高
double x_z, y_z, x_org, y_org, x_edge, y_edge;
int trashold_x;
int trashold_y;
char buf[32];
String incomingByte = ""; // for incoming serial data
int led = 13;

void setup() {

  Serial.begin(9600);
  Serial.print("Starting...\n");

  pan.attach(pan_pin);
  tilt.attach(tilt_pin);

  pan.write(90);
  tilt.write(90);

  x_z = tan(37.5 * deg2rad) * x_max / 2;  //計算XZ平面Ｚ軸長度
  y_z = tan(23.5 * deg2rad) * y_max / 2;  //計算YZ平面Ｚ軸長度

  x_edge = x_z / tan(89 * deg2rad); //x軸最大邊緣值
  y_edge = y_z / tan(89 * deg2rad); //y軸最大邊緣值

  int trashold_x = 10;      //x軸制動值
  int trashold_y = 10;      //y軸制動值

  x_org = 320;  //初始化X原始位置
  y_org = 240;  //初始化Y原始位置

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop() {
  while (Serial.available() > 0) {

    int inChar = Serial.read();//not using this

    if (inChar != 'n') {
      Serial.print("incomingByte:" + incomingByte + "f\n");
      if (inChar != 10)
        incomingByte += (char)inChar;
      Serial.print("incomingByte:" + incomingByte + " inChar:" + inChar + "b\n");
    }
    else {
      int x, y;

      x = int(incomingByte[0] - '0') * 100 + int(incomingByte[1] - '0') * 10 + int(incomingByte[2] - '0') * 1;
      y = int( incomingByte[4] - '0') * 100 + int(incomingByte[5] - '0') * 10 + int(incomingByte[6] - '0') * 1;

      sprintf(buf, "x=%d  y=%d\n", x, y);     //確認x、y值是否正確(無法使用)
      Serial.print(buf);

      x = CV_X(x);
      y = CV_Y(y);
      if (abs(x_org + x) < x_edge || abs(y_org + y) < y_edge)
      {
        if (abs(x) > trashold_x || abs(y) > trashold_y)
        {
          pan.write(90 + atan(x_z / (double)(x_org + x))*rad2deg);
          tilt.write(90 + atan(y_z / (double)(y_org + y))*rad2deg);
        }
      }
      else
      {
        if (x > 0)
          pan.write(180);
        else
          pan.write(0);

        if (y > 0)
          tilt.write(180);
        else
          tilt.write(0);
      }
      incomingByte = "";
    }

    digitalWrite(led, HIGH);
    delay(500);
  }
  digitalWrite(led, LOW);
  delay(500);
  //x=320,y=240;



}

int CV_X(int x)
{
  return x - x_max / 2;
}

int CV_Y(int y)
{
  return y - y_max / 2;
}

