/** Coordinates functions */
#include "main.h"
Controller master(E_CONTROLLER_MASTER);
/** Default initialization of an object of class Coordinates. */
Coordinates::Coordinates(){
  this -> x = 0;
  this -> y = 0;
  this -> angle = 0;
}
/**
 * Initialization of an object of class Coordinates given (x, y).
 * @param x
 * x-coordinate
 *
 * @param y
 * y-coordinate
 */
Coordinates::Coordinates(double x, double y){
  this -> x = x;
  this -> y = y;
  this -> angle = 0;
}
/**
 * Initialization of an object of class Coordinates given (x, y, angleDeg).
 * @param x
 * x-coordinate
 *
 * @param y
 * y-coordinate
 *
 * @param angleDeg
 * bearing in degrees
 */
Coordinates::Coordinates(double x, double y, double angleDeg){
  this -> x = x;
  this -> y = y;
  this -> angle = boundRad(angleDeg*toRad);
}
/**
 * Set Coordinates.
 * @param x
 * to-be-set x-coordinate
 *
 * @param y
 * to-be-set y-coordinate
 *
 * @param angleDeg
 * to-be-set angleDeg
 */
void Coordinates::setCoords(double x, double y, double angleDeg){
  this -> x = x;
  this -> y = y;
  this -> angle = angleDeg*toRad;
}
/** Print Coordinates to the terminal. */
void Coordinates::printCoordsTerminal(){
  printf("x: %.2f, y: %.2f, angle: %.2f\n",this->x, this->y, this->angle*toDeg);
}
/** Print Coordinates to the master controller. */
void Coordinates::printCoordsMaster(){
  master.print(2,0,"%2.1f %2.1f %3f",this->x,this->y,this->angle*toDeg);
}
