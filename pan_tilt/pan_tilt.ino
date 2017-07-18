#include <Servo.h>
#include <math.h>
#define pi 3.1415926
#define deg2rad 3.1415926/180.0
#define rad2deg 180.0/3.1415926


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
String incomingByte; // for incoming serial data
int led = 13;
int period;

void setup() {

  Serial.begin(9600);
  Serial.print("Starting...\n");
  while(Serial.read()>= 0){}
  pan.attach(pan_pin);
  tilt.attach(tilt_pin);

  pan.write(90);
  tilt.write(90);

  x_z = x_max / 2 / tan(37.5 * deg2rad);  //計算XZ平面Ｚ軸長度
  y_z = x_max / 2 / tan(23.5 * deg2rad);  //計算YZ平面Ｚ軸長度

  Serial.println(String("") + "x_z=" + x_z + " y_z=" + y_z);

  //x_edge = 180; //x軸最大邊緣值
  //y_edge = y_z * tan(89 * deg2rad); //y軸最大邊緣值

  Serial.println(String("") + "x_edge=" + x_edge + " y_edge=" + y_edge);

  int trashold_x = 20;      //x軸制動值
  int trashold_y = 20;      //y軸制動值

  x_org = 90;  //初始化X原始位置
  y_org = 90;  //初始化Y原始位置

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.print("\n\n");

}

void loop() {
  while (Serial.available() > 0) {
    delay(1);
    
    incomingByte=Serial.readString();
      String x_char, y_char;
      int x,y;

      Serial.println(String("") + "IN: x_org=" + x_org + " y_org=" + y_org);
      period=incomingByte.indexOf(',');
      x_char=incomingByte.substring(0,period);
      x=x_char.toInt();
      y_char = incomingByte.substring(period+1);
      y=y_char.toInt();
      
      sprintf(buf, "x=%d  y=%d\n", x, y);     //確認x、y值是否正確(無法使用)
      Serial.print(buf);

      x = CV_X(x);
      y = CV_Y(y);

      sprintf(buf, "x=%d  y=%d\n", x, y);     //確認x、y值是否正確(無法使用)
      Serial.print(buf);

      if (x_org - atan(x / x_z)*rad2deg < 180 && x_org - atan(x / x_z)*rad2deg > 0 && abs(x) > trashold_x) { //pan控制區
        pan.write(x_org - atan(x / x_z)*rad2deg);
        x_org = x_org - atan(x / x_z) * rad2deg;
        Serial.println(String("") + "x_deg=" + atan(x / x_z)*rad2deg );
      }
      else if (abs(x) > trashold_x) {
        if (x > 0)
          pan.write(180);//, x_org = 180;
        else
          pan.write(0);//, x_org = 0;

        Serial.print("EDGE\n");

      }
      else if (abs(x) < trashold_x) {
          pan.write(x_org);
      }
      
      if (y_org + atan(y / y_z)*rad2deg < 180 && y_org + atan(y / y_z)*rad2deg > 0 && abs(y) > trashold_y) { //tilt控制區
        tilt.write(y_org + atan(y / y_z)*rad2deg);
        y_org = y_org + atan(y / y_z) * rad2deg;
        Serial.println(String("") + "y_deg=" + atan(y / y_z)*rad2deg);
      }
      else if (abs(y) > trashold_y) {
        if (y > 0)
          tilt.write(180);//, y_org = 180;
        else
          tilt.write(0);//, y_org = 0;

        Serial.print("EDGE\n");
      }
      else if(abs(y) < trashold_y){
         tilt.write(y_org);//, y_org = 0;
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
    

    digitalWrite(led, HIGH);
    delay(1);
  }
  digitalWrite(led, LOW);
  delay(1);
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
