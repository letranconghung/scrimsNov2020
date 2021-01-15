/** Header file for mech_lib.cpp */
#ifndef _MECH_LIB_HPP_
#define _MECH_LIB_HPP_
void rollersMove(double pow);
void bouncerMove(double pow);
void ejectorMove(double pow);
void shooterMove(double pow);
void frollerMove(double pow);
void load(double pow);
void eject(double pow);
void shoot(double pow);
void moveTillLoad(double pow);
void moveBlindIntake(double pow, double time, double extra);
void blindIntake(double pow, double time);
void allMove(double rollersPow, double bouncerPow, double ejectorPow, double shooterPow, double frollerPow);
void controllerPrinter(void *ignore);
#endif
