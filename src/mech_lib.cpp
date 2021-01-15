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
int colorThreshold = 1200;
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
void waitShoot(){
  while (color.get_value()>colorThreshold) delay(5);
  while (color.get_value()<colorThreshold) delay(5);
}
void waitEject(){
  while(!limit.get_value()) delay(5); //wait till limit is pressed
  while(limit.get_value()) delay(5); // wait till limit is released
}
void load(double pow){
  allMove(pow, pow, pow, 0, 0);
  waitLoad();
  allMove(0, 0, 0, 0, 0);
}
void eject(double pow){
  allMove(pow,pow,-pow,0,0);
  waitEject();
  allMove(0, 0, 0, 0, 0);
}

void allMove(double rollersPow, double bouncerPow, double ejectorPow, double shooterPow, double frollerPow){
  rollersMove(rollersPow);
  bouncerMove(bouncerPow);
  ejectorMove(ejectorPow);
  shooterMove(shooterPow);
  frollerMove(frollerPow);
}
void moveTillLoad(double pow){
  Motor BL(BLPort);
  Motor BR(BRPort);
  BL.move(pow);
  BR.move(pow);
  load(127);
  BL.move(0);
  BR.move(0);
}
void blindIntake(double pow, double time){
  allMove(pow, pow, pow, 0, 0);
  delay(time);
  allMove(0, 0, 0, 0, 0);
}
void moveBlindIntake(double pow, double time, double extra){
  Motor BL(BLPort);
  Motor BR(BRPort);
  BL.move(pow);
  BR.move(pow);
  blindIntake(127, time);
  BL.move(0);
  BR.move(0);
  delay(500);
  BL.move(pow);
  BR.move(pow);
  delay(extra);
  BL.move(0);
  BR.move(0);
}
void shoot(double pow){
  allMove(0, 0, pow, pow, pow);
  delay(500);
  allMove(0, 0, 0, 0, 0);
}

void controllerPrinter(void *ignore){
  Controller master(E_CONTROLLER_MASTER);
  while(true){
    master.print(0, 0, "LETS GO HANSON");
    Task::delay(100);
  }
}
