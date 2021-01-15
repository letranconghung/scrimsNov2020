/** Odometry Task and functions */
#include "main.h"
/** to test odometry in opcontrol() when not in competition */
#define COMPETITION_MODE false
/** declare encoders */
ADIEncoder encoderL(encdL_port, encdL_port + 1);
ADIEncoder encoderR(encdR_port, encdR_port + 1);
/** encdL, encdR = value of respective encoders */
double encdL = 0, encdR = 0;
/** global variable: current position of the robot */
Coordinates position;
/**
 * Retrieve encoder values.
 * @param processed
 * whether the returned values should be in inches (default) or encoder degrees
 *
 * @return
 * encoder values packaged in a pair
 */
std::pair<double, double> getEncdVals(bool rawData = false){
  if(!rawData) return std::make_pair((double) encoderL.get_value()*inPerDeg, (double) encoderR.get_value()*inPerDeg);
  else return std::make_pair((double)encoderL.get_value(), (double)encoderR.get_value());
}
/** Update the robot's position using side encoders values. */
void baseOdometry(void * ignore){
  /** D loop variables */
  double prevEncdL = 0;
  double prevEncdR = 0;
  double prevAngle = 0;
  int count = 0;
  while(!COMPETITION_MODE || competition::is_autonomous()){
    /** refer to Odometry Documentation for mathematical proof */
    /** retrieve & update encoder values */
    encdL = getEncdVals().first;
    encdR = getEncdVals().second;
    double encdChangeL = (encdL-prevEncdL);
    double encdChangeR = (encdR-prevEncdR);
    double sumEncdChange = encdChangeL + encdChangeR;
    double deltaAngle = (encdChangeL - encdChangeR)/baseWidth;
    position.angle += deltaAngle;

    /** update x- and y-coordinates */
    if(deltaAngle == 0) {
      /** handle 0 as the formula involves division by deltaAngle */
			position.x += sumEncdChange/2*sin(position.angle);
			position.y += sumEncdChange/2*cos(position.angle);
		}
		else {
			double halfDeltaAngle = deltaAngle/2;
			position.x += (sumEncdChange/deltaAngle)*sin(halfDeltaAngle)*sin(prevAngle+halfDeltaAngle);
			position.y += (sumEncdChange/deltaAngle)*sin(halfDeltaAngle)*cos(prevAngle+halfDeltaAngle);
		}
    /** update prev variables */
		prevEncdL = encdL;
		prevEncdR = encdR;
		prevAngle = position.angle;
    /** debugging */
    if(!COMPETITION_MODE) position.printCoordsMaster();
    if((DEBUG_MODE == 1) && (count++ % 100 == 0)){
      position.printCoordsTerminal();
    }
    if(DEBUG_MODE == 4) printf("Encoder values %4.0f \t %4.0f\n",getEncdVals(true).first,getEncdVals(true).second);
    /** refresh rate of Task */
    Task::delay(5);
  }
}
