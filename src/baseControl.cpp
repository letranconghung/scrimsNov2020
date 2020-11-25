/** Base movement tasks and functions */
#include "main.h"
/** default values of kP and kD */
#define DEFAULT_KP 0.3
#define DEFAULT_KD 0.15
#define DEFAULT_TURN_KP 0.4
#define DEFAULT_TURN_KD 0.1
/** declare motors */
Motor BL (BLPort);
Motor BR (BRPort);
/**
 * targetEncdL & targetEncdR are target values for the 2 side encoders.
 * They are used to link movement functions to baseControl task.
 * Movement unctions will change the values of these 2 variables and the task will respond correspondingly.
 */
double targetEncdL=0,targetEncdR=0;
/**
 * targetPowerL & targetPowerR are target values for the power for side motors.
 * They are used to link baseControl to baseMotorControl.
 * Note that these values are only target and not final values, and subjected to calibration, capping and bounding by baseMotorControl
 */
double targetPowerL=0,targetPowerR=0;
/** PD loop constants */
double kP,kD;
/**
 * Move straight.
 * @param dis
 * distance in inches
 *
 * @param kp
 * kP
 *
 * @param kd
 * kD
 */
void baseMove(double dis, double kp, double kd){
  /** convert dis in inches to encoder degrees */
  targetEncdL += dis/inPerDeg;
  targetEncdR += dis/inPerDeg;
  /** assign custom values to kP and kD */
  kP = kp;
  kD = kd;
}
/**
 * Move straight using default values of kP and kD.
 * @param dis
 * distance in inches
 */
void baseMove(double dis){
  baseMove(dis, DEFAULT_KP, DEFAULT_KD);
}
/**
 * Move straight towards a coordinate.
 * @param x
 * x-coordinate of the target
 *
 * @param y
 * y-coordinate of the target
 *
 * @param kp
 * propotional constant
 *
 * @param kd
 * kD
 */
void baseMove(double x, double y, double kp, double kd){
	double errorX = x-position.x;
  double errorY = y-position.y;
  /** calculate Pythagorean distance */
	double distance = sqrt(errorX * errorX + errorY * errorY);
  /**
   * Refer to this link for visual aid: https://en.wikipedia.org/wiki/Atan2
   * In words, atan2(y, x) would return the angle wrt the x-axis.
   * Thus in this case, we use atan2(x, y) to return the angle wrt to the y-axis, which is the bearing.
   * The calculation of targAngle here solely serves the purpose of determining whether the robot should reverse or not, which will be implemented in the next code portion.
   */
	double targAngle = atan2(errorX,errorY);
  /**
   * In reality, |targAngle| should be approximately equal to |position.angle| already, since we would have already done a baseTurn to (x, y).
   * halfPI is just a value not too small nor too big to differentiate whether targAngle & position.angle are opposites of each other or not.
   * If reverse = 1, the robot should move forward, else reverse.
   */
	int reverse = 1;
  if(fabs(targAngle-position.angle) >= halfPI) reverse = -1;
  /** convert dis in inches to encoder degrees */
  targetEncdL += distance/inPerDeg*reverse;
  targetEncdR += distance/inPerDeg*reverse;
  /** assign custom values to kP and kD */
  kP = kp;
  kD = kd;
}
/**
 * Move straight towards a coordinate using default kP and kD values.
 * @param x
 * x-coordinate of the target
 *
 * @param y
 * y-coordinate of the target
 */
void baseMove(double x, double y){
  baseMove(x, y, DEFAULT_KP, DEFAULT_KD);
}
/**
 * Turn to an absolute bearing.
 * @param angleDeg
 * bearing (absolute angle) in degrees
 *
 * @param kp
 * kP
 *
 * @param kd
 * kD
 */
void baseTurn(double angleDeg, double kp, double kd){
	double error = angleDeg*toRad - position.angle;
  /** refer to Odometry Documentation for mathematical proof */
	double diff = error*baseWidth/inPerDeg;
	targetEncdL += diff/2;
	targetEncdR += -diff/2;
  /** assign custom values to kP and kD */
	kP = kp;
	kD = kd;
}
/**
 * Turn to an absolute bearing using default turn kP and kD values.
 * @param angleDeg
 * bearing (absolute angle) in degrees
 */
void baseTurn(double angleDeg){
  baseTurn(angleDeg, DEFAULT_TURN_KP, DEFAULT_TURN_KD);
}
/**
 * Turn to a coordinate.
 * @param x
 * x-coordinate of the target
 *
 * @param y
 * y-coordinate of the target
 *
 * @param kp
 * kP
 *
 * @param kd
 * kD
 *
 * @param reverse
 * true: backward movement
 * false: forward movement
 */
void baseTurn(double x, double y, double kp, double kd, bool reverse = false){
  /** same concept as above in baseMove(x, y, kp, kd) */
	double targAngle = atan2((x-position.x),(y-position.y));
	if(reverse) targAngle += PI;
  /** prevent turns that span over PI rad (which we can just turn the other way) */
	if(targAngle-position.angle > PI) targAngle -= twoPI;
	if(targAngle-position.angle < -PI) targAngle += twoPI;
  /** refer to Odometry Documentation for mathematical proof */
  double diff = (targAngle - position.angle)*baseWidth/inPerDeg;
  targetEncdL += diff/2;
  targetEncdR += -diff/2;
  /** assign custom values for kP and kD */
  kP = kp;
  kD = kd;
}
/**
 * Turn to a coordinate using default turn kP and kD values.
 * @param x
 * x-coordinate of the target
 *
 * @param y
 * y-coordinate of the target
 *
 * @param reverse
 * true: backward movement
 * false: forward movement
 */
void baseTurn(double x, double y, bool reverse = false){
  baseTurn(x, y, DEFAULT_TURN_KP, DEFAULT_TURN_KD, reverse);
}
/**
 * Turn a relative angle.
 * @param angle
 * relative angle in degrees
 *
 * @param kp
 * kP
 *
 * @param kd
 * kD
 */
void baseTurnRelative(double angle, double kp, double kd){
  /** refer to Odometry Documentation for mathematical proof */
  double diff = angle*toRad*baseWidth/inPerDeg;
  targetEncdL += diff/2;
  targetEncdR += -diff/2;
  /** assign custom kP and kD values */
  kP = kp;
  kD = kd;
}
void baseTurnRelative(double angle) {
  baseTurnRelative(angle, DEFAULT_TURN_KP, DEFAULT_TURN_KD);
}
/**
 * Introduce a cutoff to base movements to interfere with the task when it takes too long to reach a target (e.g. due to too small DISTANCE_LEEWAY or too small kP).
 * @param cutoff
 * cutoff duration in milliseconds
 */
void waitBase(double cutoff){
  /** start the timer */
	double start = millis();
  /** while the encoder values are not within DISTANCE_LEEWAY from the target encoder values yet, or time has not run out */
	while(fabs(targetEncdL - getEncdVals(true).first) > DISTANCE_LEEWAY && fabs(targetEncdR - getEncdVals(true).second) > DISTANCE_LEEWAY && (millis()-start) < cutoff) delay(20);
  /** stop the motors */
	BL.move(0);
	BR.move(0);
}
/** boolean flag for whether there is a cap on base motor powers */
bool basePowCapped = false;
/** value of power cap */
double absPowerCap;
/**
 * Cap base motor powers.
 * @param cap
 * power cap
 */
void capBasePow(double cap){
	basePowCapped = true;
	absPowerCap = cap;
}
/** Remove the base motor power cap. */
void rmBaseCap(){
	basePowCapped = false;
}
/** boolean flag for whether basePowerControl controls base motor powers (for timing movements) */
bool basePaused = false;
/**
 * Set the value of basePaused.
 * @param pause
 * to-be-set value of basePaused
 */
void pauseBase(bool pause = true){
  basePaused = pause;
}
/**
 * Movement by raw power and timing.
 * @param powL
 * left power
 *
 * @param powR
 * right power
 *
 * @param time
 * movement duration in ms
 */
void timerBase(double powL, double powR, double time){
  double start = millis();
  pauseBase();
  BL.move(powL);
  BR.move(powR);
  while(millis() - start < time) delay(20);
	BL.move(0);
	BR.move(0);
	pauseBase(false);
}
/**
 * Set power to the left and right motors separately.
 * @param powL
 * left power
 *
 * @param powR
 * right power
 */
void powerBase(double powL, double powR){
  double start = millis();
  pauseBase();
  BL.move(powL);
  BR.move(powR);
}
/**
 * Reset the robot's coordinates and tare all motors.
 * @param x
 * x-coordinate of position
 *
 * @param y
 * y-coordinate of position
 *
 * @param angleDeg
 * bearing of position in degrees
 */
void resetCoords(double x, double y, double angleDeg){
  /** set position */
  position.setCoords(x, y, angleDeg);
  /** tare all motors */
	BL.tare_position();
	BR.tare_position();
  /** reset target encoder values */
  targetEncdL = 0;
  targetEncdR = 0;
}
/** calculate target motor powers using a PD loop */
void baseControl(void * ignore){
  /** D loop variables */
  double prevErrorEncdL = 0, prevErrorEncdR = 0;
  while(competition::is_autonomous()){
    /** whether vision library is in use */
    if(useVision) {
      targetEncdL = getEncdVals(true).first;
      targetEncdR = getEncdVals(true).second;
    }
    double errorEncdL = targetEncdL - getEncdVals(true).first;
    double errorEncdR = targetEncdR - getEncdVals(true).second;
    /** PD loop */
    double deltaErrorEncdL = errorEncdL - prevErrorEncdL;
    double deltaErrorEncdR = errorEncdR - prevErrorEncdR;
    prevErrorEncdL = errorEncdL;
    prevErrorEncdR = errorEncdR;
    targetPowerL = kP*errorEncdL+kD*deltaErrorEncdL;
    targetPowerR = kP*errorEncdR+kD*deltaErrorEncdR;
    /** debugging */
		if(DEBUG_MODE == 2) printf("Error: %f %f\n",errorEncdL,errorEncdR);
    /** refresh rate of Task */
		Task::delay(20);
  }
}
/** Control and set the motor powers by imposing limits and maximum ramping. */
void baseMotorControl(void * ignore){
  /** to-be-set values for left and right motors */
  double powerL=0,powerR=0;
  while(competition::is_autonomous()){
    if(!useVision) {
      /** limit consecutive power increments to below RAMPING_POW */
      double deltaPowerL = targetPowerL - powerL;
      powerL += abscap(deltaPowerL, RAMPING_POW);
      double deltaPowerR = targetPowerR - powerR;
      powerR += abscap(deltaPowerR, RAMPING_POW);
      /** handle custom speed caps */
      if(basePowCapped){
        powerL = abscap(powerL, absPowerCap);
        powerR = abscap(powerR, absPowerCap);
      }
      else{
        powerL = abscap(powerL, MAX_POW);
        powerR = abscap(powerR, MAX_POW);
      }
      /** if baseMotorControl is not paused */
      if(!basePaused){
        BL.move(powerL);
        BR.move(powerR);
      }
      /** debugging */
      if(DEBUG_MODE == 3) printf("%4.0f \t %4.0f\n",powerL,powerR);
      /** refresh rate of Task */
      Task::delay(20);
    }
  }
}
