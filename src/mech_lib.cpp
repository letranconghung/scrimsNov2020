/** Other mechanical functions. */
#include "main.h"
#include "mech_lib.hpp"
Motor bouncer(bouncerPort);
Motor ejector(ejectorPort);
Motor shooter(shooterPort);
Motor froller(frollerPort);
ADIAnalogIn color(colorPort);
ADIDigitalIn limit(limitPort);
int colorThreshold = 2700;
void bouncerMove(double pow){
  bouncer.move(pow);
}
void ejectorMove(double pow){
  ejector.move(pow);
}
void shooterMove(double pow){
  shooter.move(pow);
}
void frollerMove(double pow){
  froller.move(pow);
}
void waitLoad(){
  while (color.get_value()>colorThreshold) delay(5);
}
void waitEject(){
  while(!limit.get_value()) delay(5); //wait till limit is pressed
  while(limit.get_value()) delay(5); // wait till limit is released
}
void load(double pow){
  frollerMove(pow);
  bouncerMove(pow);
  ejectorMove(pow);
  waitLoad();
  frollerMove(0);
  bouncerMove(0);
  ejectorMove(0);
}
void eject(double pow){
  bouncerMove(pow);
  ejectorMove(-pow);
  frollerMove(pow);
  waitEject();
  bouncerMove(0);
  ejectorMove(0);
  frollerMove(0);
}
