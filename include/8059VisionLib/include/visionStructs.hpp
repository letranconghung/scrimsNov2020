/** Header file for visionStructs.cpp */
#ifndef _8059_VISION_LIB_VISION_STRUCTS_HPP_
#define _8059_VISION_LIB_VISION_STRUCTS_HPP_
/** The class VisionCoordinates provides information on the relative position of field elements in terms of on-screen x, y, w, h as registered by the Vision Sensors. */
class VisionCoordinates{
public:
  /**
   * sig: Vision Sensor signature
   * (x, y, w, h): on-screen x-coordinate, y-coordinate, width, height
   */
  int sig, x, y, w, h;
  /** refer to visionStructs.cpp for function documentation */
  VisionCoordinates();
  VisionCoordinates(int sig, int x, int y, int w, int h);
  void setVisionCoords(int sig, int x, int y, int w, int h);
  void getFromObject(pros::vision_object_s_t visObj);
  void print();
  void printMaster();
};
/** The class VisionObject bundles target VisionCoordinates with current VisionCoordinates for better accessibility and neatness. */
class VisionObject{
public:
  /** curr, targ: current and target VisionCoordinates */
  VisionCoordinates curr, targ;
  /** refer to visionStructs.cpp for function documentation */
  VisionObject();
  VisionObject(VisionCoordinates c, VisionCoordinates t);
};
#endif
