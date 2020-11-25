/** Other mechanical functions. */
#include "main.h"
#include "mech_lib.hpp"
Motor lRoller(lRollerPort);
Motor rRoller(rRollerPort);
Motor bouncer(bouncerPort);
Motor ejector(ejectorPort);
Motor shooter(shooterPort);
Motor froller(frollerPort);
ADIAnalogIn color(colorPort);
ADIDigitalIn limit(limitPort);
int colorThreshold = 2600;
void rollersMove(double pow){
  lRoller.move(pow);
  rRoller.move(pow);
}
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
bool loadMode = false, ejectMode = false, shootMode = false;
double intakePow = 0, outtakePow = 0, loadPow, ejectPow, shootPow, cyclePow;
void intake(double pow){
  intakePow = pow;
}
void outtake(double pow){
  outtakePow = pow;
}
void load(double pow){
  rollersMove(pow);
  frollerMove(pow);
  bouncerMove(pow);
  ejectorMove(pow);
  shooterMove(pow);
  waitLoad();
  rollersMove(0);
  frollerMove(0);
  bouncerMove(0);
  ejectorMove(0);
  shooterMove(0);
}
void eject(double pow){
  rollersMove(pow);
  bouncerMove(pow);
  ejectorMove(-pow);
  frollerMove(pow);
  shooterMove(-pow);
  waitEject();
  rollersMove(0);
  bouncerMove(0);
  ejectorMove(0);
  frollerMove(0);
  shooterMove(0);
  ejectMode = false;
}
void allMove(double rollersPow, double bouncerPow, double ejectorPow, double shooterPow, double frollerPow){
  rollersMove(rollersPow);
  bouncerMove(bouncerPow);
  ejectorMove(ejectorPow);
  shooterMove(shooterPow);
  frollerMove(frollerPow);
}
