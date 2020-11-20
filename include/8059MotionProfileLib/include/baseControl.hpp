/** Header file for baseControl.cpp */
#ifndef _8059_MOTION_PROFILE_LIB_BASE_CONTROL_HPP_
#define _8059_MOTION_PROFILE_LIB_BASE_CONTROL_HPP_
/**
 * DEBUG_MODE can be used to debug & test functions and tasks via the terminal.
 * 0: None
 * 1: Odometry (print Coordinates position)
 * 2: Encoders (print errorEncdL & errorEncdR)
 * 3: Power (print powerL & powerR)
 * 4: Raw encoder values (print raw encdL & encdR)
 */
#define DEBUG_MODE 0
/** maximum allowed power */
#define MAX_POW 90
/**
 * maximum power increment step
 * mathematically: |V - V previous| <= RAMPING_POW
 */
#define RAMPING_POW 6
/**
 * DISTANCE_LEEWAY is the distance (in inches) within which from the target the robot would register that it has arrived at the target.
 */
#define DISTANCE_LEEWAY 3
/** refer to baseControl.cpp for function documentation */
void baseMove(double dis, double kp, double kd);
void baseMove(double dis);
void baseMove(double x, double y, double kp, double kd);
void baseMove(double x, double y);
void baseTurn(double angleDeg, double kp, double kd);
void baseTurn(double angleDeg);
void baseTurn(double x, double y, double kp, double kd, bool reverse);
void baseTurnRelative(double angle, double kp, double kd);
void baseTurnRelative(double angle);
void waitBase(double cutoff);
void capBasePow(double cap);
void rmBaseCap();
void pauseBase(bool pause);
void timerBase(double powL, double powR, double time);
void powerBase(double powL, double powR);
void resetCoords(double x, double y, double angleDeg);
extern double targetEncdL, targetEncdR;
void baseControl(void * ignore);
void baseMotorControl(void * ignore);
#endif
