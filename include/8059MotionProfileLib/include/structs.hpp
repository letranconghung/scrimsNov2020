/** Header file for structs.cpp */
#ifndef _8059_MOTION_PROFILE_LIB_STRUCTS_HPP_
#define _8059_MOTION_PROFILE_LIB_STRUCTS_HPP_
/**
 * The class Coordinates provides information on the absolute position (x, y) and bearing (angle) of the robot on a virtual Cartesian plane (the field).
 * It also supports printing and mathematical positional functions for debugging purposes.
 */
class Coordinates{
public:
  /**
   * (x, y): coordinates on the Cartesian plane
   * angle: bearing (bounded from 0 to twoPI)
   * angleDeg: bearing in degrees (for ease of measurement and input in real life)
   */
  double x, y, angle, angleDeg;
  /** refer to structs.cpp for function documentation */
  Coordinates();
  Coordinates(double x, double y);
  Coordinates(double x, double y, double angleDeg);
  void setCoords(double x, double y, double angleDeg);
  void printCoordsTerminal();
  void printCoordsMaster();
};
#endif
