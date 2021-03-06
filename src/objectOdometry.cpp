/**
 * Vision task that constantly updates the largest (and therefore closest) red ball, blue ball and green flag
 */
#include "main.h"
const VisionCoordinates TARG_BALL{0, 150, 85, 104, 57};
const VisionCoordinates TARG_FLAG{0, 149, 68, 60, 70};
/** VisionObject objects that bundle current ball locations with target ball locations */
VisionObject redBall(VisionCoordinates(), TARG_BALL);
VisionObject blueBall(VisionCoordinates(), TARG_BALL);
VisionObject greenFlag(VisionCoordinates(), TARG_FLAG);
Vision visFlag(visFlagport);
Vision visBall(visBallport);
void objectOdometry(void * ignore){
  int i = 0;
  while(!VISION_COMPETITION_MODE || competition::is_autonomous()){
    /** import output from Vision Sensor */
    /** changed to visFlag for xMode */
    redBall.curr.getFromObject(visFlag.get_by_sig(0, SIG_RED_BALL));
    blueBall.curr.getFromObject(visFlag.get_by_sig(0, SIG_BLUE_BALL));
    greenFlag.curr.getFromObject(visFlag.get_by_sig(0, SIG_GREEN_FLAG));
    ++i;
    /** debugging */
    if(VISION_DEBUG_MODE == 1 && (i % 50 == 0)){
      printf("---------------------------------------------------------\n");
      redBall.curr.print();
      blueBall.curr.print();
      greenFlag.curr.print();
    }
    if(VISION_DEBUG_MODE == 3 && (i % 10 == 0)){
      redBall.curr.printMaster();
      redBall.curr.print();
    }
    Task::delay(10);
  }
}
