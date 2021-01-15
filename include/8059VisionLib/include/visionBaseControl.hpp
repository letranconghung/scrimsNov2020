/** Header file for visualBaseControl.cpp */
#ifndef _8059_VISION_LIB_VISION_BASE_CONTROL_HPP_
#define _8059_VISION_LIB_VISION_BASE_CONTROL_HPP_
#define VISION_COMPETITION_MODE false
/**
 * VISION_DEBUG_MODE can be used to debug & test functions and tasks via the terminal.
 * 0: None
 * 1: Object Odometry
 * 2: error, targetPower, power
 * 3: Selected odometry
 */
#define VISION_DEBUG_MODE 0
/**
 * W_LEEWAY and X_LEEWAY are the distances (in Vision Sensor units) within which from the target
 * the robot would register that it has arrived at the target.
 */
#define W_LEEWAY 5
#define X_LEEWAY 5
/** maximum allowed power */
#define VISION_MAX_POW 100
/**
 * maximum power increment step
 * mathematically: |V - V previous| <= RAMPING_POW
 */
#define VISION_RAMPING_POW 8
/** refer to visionBaseControl.cpp for function documentation */
void visionBaseMove(int sig, double xkp, double xkd, double wkp, double wkd);
void visionBaseMove(int sig);
void visionWaitBase(double cutoff);
void visionBaseControl(void * ignore);
#endif
