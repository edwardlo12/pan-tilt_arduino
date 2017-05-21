#include <Servo.h>
#include <math.h>
#define pi 3.1415926
#define deg2rad 3.14159/180.0
#define rad2deg 180.0/3.14159


Servo pan;
Servo tilt;

int pan_pin = 2;      //PAN控制腳位
int tilt_pin = 3;     //TILT控制腳位
int x_max = 640;      //原始圖資寬
int y_max = 480;      //原始圖資高
float x_z, y_z, x_org, y_org, x_edge, y_edge;
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

  Serial.println(String("") + "x_z=" + x_z + " y_z=" + y_z);

  x_edge = 180; //x軸最大邊緣值
  y_edge = y_z * tan(89 * deg2rad); //y軸最大邊緣值

  Serial.println(String("") + "x_edge=" + x_edge + " y_edge=" + y_edge);

  int trashold_x = 10;      //x軸制動值
  int trashold_y = 10;      //y軸制動值

  x_org = 90;  //初始化X原始位置
  y_org = 90;  //初始化Y原始位置

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.print("\n\n");

}

void loop() {
  while (Serial.available() > 0) {

    int inChar = Serial.read();//not using this

    if (inChar != 'n') {
      //Serial.print("incomingByte:" + incomingByte + "f\n");
      if (inChar != 10)
        incomingByte += (char)inChar;
      //Serial.print("incomingByte:" + incomingByte + " inChar:" + inChar + "b\n");
    }
    else {
      int x, y;

      Serial.println(String("") + "IN: x_org=" + x_org + " y_org=" + y_org);

      x = int(incomingByte[0] - '0') * 100 + int(incomingByte[1] - '0') * 10 + int(incomingByte[2] - '0') * 1;

      //if (incomingByte[0] == '-')
      //x = x * -1;

      y = int( incomingByte[4] - '0') * 100 + int(incomingByte[5] - '0') * 10 + int(incomingByte[6] - '0') * 1;

      //if (incomingByte[5] == '-')
      //y = y * -1;

      sprintf(buf, "x=%d  y=%d\n", x, y);     //確認x、y值是否正確(無法使用)
      Serial.print(buf);

      x = CV_X(x);
      y = CV_Y(y);

      sprintf(buf, "x=%d  y=%d\n", x, y);     //確認x、y值是否正確(無法使用)
      Serial.print(buf);

      if (x_org + atan(x / x_z)*rad2deg < 180 && x_org + atan(x / x_z)*rad2deg > 0 && abs(x) > trashold_x) { //pan控制區
        pan.write(x_org + atan(x / x_z)*rad2deg);
        x_org = x_org + atan(x / x_z) * rad2deg;
        Serial.println(String("") + "x_deg=" + atan(x / x_z)*rad2deg );
      }
      else if (abs(x) > trashold_x) {
        if (x > 0)
          pan.write(180), x_org = 180;
        else
          pan.write(0), x_org = 0;

        Serial.print("EDGE\n");

      }

      if (y_org + atan(y / y_z)*rad2deg < 180 && y_org + atan(y / y_z)*rad2deg > 0 && abs(y) > trashold_y) { //tilt控制區
        tilt.write(y_org + atan(y / y_z)*rad2deg);
        y_org = y_org + y;
        Serial.println(String("") + "y_deg=" + atan(y / y_z)*rad2deg);
      }
      else if (abs(y) > trashold_y) {
        if (y > 0)
          tilt.write(180), y_org = 180;
        else
          tilt.write(0), y_org = 0;

        Serial.print("EDGE\n");
      }

      Serial.println(String("") + "OUT: x_org=" + x_org + " y_org=" + y_org);
      Serial.print("\n\n");
      /*if (abs(x_org + x) < x_edge || abs(y_org + y) < y_edge)
        {
        if (abs(x) > trashold_x || abs(y) > trashold_y)
        {
          pan.write(90 + atan((x_org + x) / x_z)*rad2deg);
          tilt.write(90 + atan((y_org + y) / y_z)*rad2deg);
          x_org = x_org + x;
          y_org = y_org + y;
          Serial.println(String("") + "x_deg=" + atan((x_org + x) / x_z)*rad2deg  + " y_deg=" + atan((y_org + y) / y_z)*rad2deg);
        }
        }
        else
        {
        if (x > 0)
          pan.write(180), x_org = x_edge;
        else
          pan.write(0), x_org = x_edge * -1;

        if (y > 0)
          tilt.write(180), y_org = y_edge;
        else
          tilt.write(0), y_org = y_edge * -1;
        }*/
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

