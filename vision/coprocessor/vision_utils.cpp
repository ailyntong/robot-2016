#include "vision_utils.h"
#include "opencv2/opencv.hpp"
#include <cmath>
#include <iostream>

Angle vision::AngleBetweenPoints(cv::Point p1, cv::Point center, cv::Point p2) {
  return std::acos(
             // dot product
             ((p1.x - center.x) * (p2.x - center.x) +
              (p1.y - center.y) * (p2.y - center.y)) /
             // length of the sides
             std::sqrt(((p1.x - center.x) * (p1.x - center.x) +
                        (p1.y - center.y) * (p1.y - center.y)) *
                       ((p2.x - center.x) * (p2.x - center.x) +
                        (p2.y - center.y) * (p2.y - center.y)))) *
         // flip sign if cross product is negative
         (((p1.x - center.x) * (p2.y - center.y) -
           (p2.x - center.x) * (p1.y - center.y)) < 0
              ? -1
              : 1) *
         rad;
}

int vision::getColorNumber(std::string colorName) {
  for (unsigned int i = 0; i < colorspaces.size(); i++) {
    if (colorNames[i] == colorName) return colorspaces[i];
  }
  return -1;
}

std::string vision::getColorName(int colorNumber) {
  for (unsigned int i = 0; i < colorspaces.size(); i++) {
    if (colorspaces[i] == colorNumber) return colorNames[i];
  }
  return "";
}
