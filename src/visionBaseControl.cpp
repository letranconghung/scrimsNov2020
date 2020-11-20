/** Vision base movement Task and functions */
#include "main.h"
#define VISION_COMPETITION_MODE false
/** default values of kP and kD */
#define DEFAULT_X_KP 0.5
#define DEFAULT_X_KD 0.1
#define DEFAULT_W_KP 1.5
#define DEFAULT_W_KD 0.2
int targSig;
double visionTargetPowerL = 0, visionTargetPowerR = 0;
double powerL = 0, powerR = 0;
double XkP = DEFAULT_X_KP, XkD = DEFAULT_X_KD, WkP = DEFAULT_W_KP, WkD = DEFAULT_W_KD;
/**
 * Set to be larger than LEEWAYs' values to avoid skipping while loops in visionWaitBase.
 * Potential cause: visionBaseControlTask takes too long to compute errors so there was 1 loop that ran with initialized values of errors, and therefore exited visionWaitBase prematurely.
 */
double errorX = 2*X_LEEWAY, errorW = 2*W_LEEWAY;
bool useVision = false;
/**
 * Base movement using vision.
 * @param sig
 * signature of desired object
 *
 * @param xkp
 * kP for x
 *
 * @param xkd
 * kD for x
 *
 * @param wkp
 * kP for w
 *
 * @param wkd
 * kD for w
 */
void visionBaseMove(int sig, double xkp, double xkd, double wkp, double wkd){
  useVision = true;
  targSig = sig;
  XkP = xkp;
  XkD = xkd;
  WkP = wkp;
  WkD = wkd;
}
/**
 * Base movement using vision using default kP & kD values.
 * @param sig
 * signature of desired object
 */
void visionBaseMove(int sig){
  useVision = true;
  targSig = sig;
  XkP = DEFAULT_X_KP;
  XkD = DEFAULT_X_KD;
  WkP = DEFAULT_W_KP;
  WkD = DEFAULT_W_KD;
}
/**
 * Delay function for vision base movements.
 * @param cutoff
 * cutoff time for the movement
 */
void visionWaitBase(double cutoff){
  Motor FL (FLPort);
  Motor BL (BLPort);
  Motor FR (FRPort);
  Motor BR (BRPort);
  Controller master (E_CONTROLLER_MASTER);
  double start = millis();
  /** delay(50) for objectOdometry to calculate new errors so that while loop doesn't end prematurely */
  delay(50);
  /** while errors are still not within predeteremined LEEWAYs, or time runs out */
  while((fabs(errorX) > X_LEEWAY || fabs(errorW) > W_LEEWAY) && (millis()-start) < cutoff){
    delay(20);
  }
  FL.move(0);
  BL.move(0);
  FR.move(0);
  BR.move(0);
  useVision = false;
}
/** Vision base movement Task. */
void visionBaseControl(void * ignore){
  Motor FL (FLPort);
  Motor BL (BLPort);
  Motor FR (FRPort);
  Motor BR (BRPort);
  Controller master(E_CONTROLLER_MASTER);
  int i = 0;
  double prevErrorX = 0, prevErrorW = 0;
  while(true){
    master.print(0, 0, "%d", true);
    if(useVision){
      VisionObject v;
      /** get the desired object */
      switch(targSig){
        case 1: v = redBall; break;
        case 2: v = blueBall; break;
        case 3: v = greenFlag; break;
      }
      /** PD loop */
      errorX = v.targ.x - v.curr.x;
      errorW = v.targ.w - v.curr.w;
      double deltaErrorX = errorX - prevErrorX;
      double deltaErrorW = errorW - prevErrorW;
      prevErrorX = errorX;
      prevErrorW = errorW;
      /** w values contribute to the move component, while x values contribute to the turn component. */
      double move = WkP*errorW + WkD*prevErrorW;
      double turn = XkP*errorX + XkD*prevErrorX;
      /** set left and right powers based on move and turn components */
      visionTargetPowerL = move - turn;
      visionTargetPowerR = move + turn;
      /** motor ramping */
      double deltaPowerL = visionTargetPowerL - powerL;
      powerL += abscap(deltaPowerL, VISION_RAMPING_POW);
      double deltaPowerR = visionTargetPowerR - powerR;
      powerR += abscap(deltaPowerR, VISION_RAMPING_POW);
      /** motor capping */
      powerL = abscap(powerL, VISION_MAX_POW);
      powerR = abscap(powerR, VISION_MAX_POW);
      FL.move(powerL);
      BL.move(powerL);
      FR.move(powerR);
      BR.move(powerR);
      /** debugging */
      if(VISION_DEBUG_MODE == 2 && (++i % 10 == 0)){
        master.print(0, 0, "PWR: %3.1f %3.1f",powerL, powerR);
        printf("-----------------------------------------------------------------\n");
        printf("ErrorX: %3.0f \t ErrorW: %3.0f\n",errorX,errorW);
        printf("move: %3.1f \t turn: %3.1f\t", move, turn);
        printf("TargetPower: %3.1f \t %3.1f\n",visionTargetPowerL,visionTargetPowerR);
        printf("Power: %3.1f \t %3.1f\n",powerL,powerR);
      }
      Task::delay(10);
    }
  }
}
