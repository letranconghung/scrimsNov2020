/** VisionObject and VisionCoordinates functions */
#include "main.h"
/** support printing */
const std::vector<std::string> sigIdToString = {"No object", "Red ball", "Blue ball", "Green flag"};
/** Default initialization of an object of class VisionCoordinates. */
VisionCoordinates::VisionCoordinates(){
  this -> setVisionCoords(0, 0, 0, 0, 0);
}
/**
 * Initialization of an object of class VisionCoordinates given (sig, x, y, w, h).
 * @param sig
 * vision signature
 *
 * @param x
 * on-screen x-coordinate
 *
 * @param y
 * on-screen y-coordinate
 *
 * @param w
 * on-screen width
 *
 * @param h
 * on-screen height
 */
VisionCoordinates::VisionCoordinates(int sig, int x, int y, int w, int h){
  this -> setVisionCoords(sig, x, y, w, h);
}
/**
 * Set VisionCoordinates.
 * @param sig
 * to-be-set vision signature
 *
 * @param x
 * to-be-set on-screen x-coordinate
 *
 * @param y
 * to-be-set on-screen y-coordinate
 *
 * @param w
 * to-be-set on-screen width
 *
 * @param h
 * to-be-set on-screen height
 */
void VisionCoordinates::setVisionCoords(int sig, int x, int y, int w, int h){
  this -> sig = sig;
  this -> x = x;
  this -> y = y;
  this -> w = w;
  this -> h = h;
}
/**
 * Import output from the Vision Sensor and handle meaningless readings.
 * @param visObj
 * vision object (output from the vision sensor)
 */
void VisionCoordinates::getFromObject(pros::vision_object_s_t visObj){
  /** handle invalid meaningless objects */
  if(visObj.signature <= 7){
    this->setVisionCoords(visObj.signature, visObj.x_middle_coord, visObj.y_middle_coord, visObj.width, visObj.height);
  }else{
    this->setVisionCoords(0, 0, 0, 0, 0);
  }
}
/** Print VisionCoordinates to the terminal. */
void VisionCoordinates::print(){
    printf("sig: %s \t x: %d \t y: %d \t w: %d \t h: %d \n", sigIdToString[this->sig].c_str(), this->x, this->y, this->w, this->h);
}
/** Print VisionCoordinates to the master controller. */
void VisionCoordinates::printMaster(){
    Controller master(E_CONTROLLER_MASTER);
    master.print(2,0,"%d %d %d %d", this->x, this->y, this->w, this->h);
}
/** Default initialization of an object of class VisionObject. */
VisionObject::VisionObject(){
  this -> curr.setVisionCoords(0, 0, 0, 0, 0);
  this -> targ.setVisionCoords(0, 0, 0, 0, 0);
}
/**
 * Initialization of an object of class VisionObject, given the current VisionCoordinates and target VisionCoordinates.
 * @param c
 * current VisionCoordinates
 *
 * @param t
 * target VisionCoordinates
 */
VisionObject::VisionObject(VisionCoordinates c, VisionCoordinates t){
  this -> curr = c;
  this -> targ = t;
}
