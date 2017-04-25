#include <Servo.h>
#include <math.h>

Servo pan;
Servo tilt;

int pan_pin = 2;
int tilt_pin = 3;
int x_max = 640;
int y_max = 480;
double x_z, y_z;
char buf[32];

void setup() {

  Serial.begin(19200);
  Serial.print("Starting...\n");

  pan.attach(pan_pin);
  tilt.attach(tilt_pin);

  pan.write(90);
  tilt.write(90);

  x_z = tan(37.5) * x_max / 2;
  y_z = tan(23.5) * y_max / 2;

}

void loop() {

  int x, y, posite;

  int trashold_x = 10;
  int trashold_y = 10;

  posite = Serial.read();
  x = posite & 0xFF00;
  x = x >> 2;
  y = posite & 0x00FF;

  //sprintf(buf, "x=%f  y=%f\n", x, y);
  //Serial.print(buf);

  if (abs(x) < trashold_x || abs(y) < trashold_y)
  {
    pan.write(90 + atan(x_z / (double)x));
    tilt.write(90 + atan(y_z / (double)y));
  }



}
