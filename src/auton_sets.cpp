/** Autonomous routines */
#include "main.h"
/** programming skills run */
void skills(){

}
/** Autonomous routine for blue left spawn. */
void blueLeft(){
}
/** Autonomous routine for blue right spawn. */
void blueRight(){
  rollersMove(127);
  timerBase(110, 80, 1000);
  shoot(127);
  load(127);
  shoot(127);
  load(127);
  shoot(127);
  eject(80);
  allMove(0, 127, 127, 127, 127);
  delay(2000);
  allMove(127, 127, 127, 127, 127);
  delay(200);
  allMove(0, 127, 127, 127, 127);
  delay(2000);
  allMove(0, 0, 0, 0, 0);
  rollersMove(-127);
  timerBase(-50, -50, 800);
  rollersMove(0);
}
/** Autonomous routine for red left spawn. */
void redLeft(){
}
/** Autonomous routine for red right spawn. */
void redRight(){
}
