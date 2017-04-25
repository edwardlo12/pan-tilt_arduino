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

void setup() {

  Serial.begin(19200);
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

  x_org = 0;  //初始化X原始位置
  y_org = 0;  //初始化Y原始位置

}

void loop() {

  int x, y, posite;

  

  x = Serial.read();
  y = Serial.read();
  /*x = posite & 0xFF00;
  x = x >> (0xFF-1);
  y = posite & 0x00FF;*/
  
  //x=320,y=240;
  
  //sprintf(buf, "x=%f  y=%f\n", x, y);     //確認x、y值是否正確
  //Serial.print(buf);

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



}
