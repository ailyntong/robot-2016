#include "object_detector.h"
#include <iostream>

ObjectTracker::ObjectTracker()
    : range(InRangeInstructions("PARAMS.txt")),
      detector(ShapeDetector(
          std::vector<Angle>({90 * deg, -90 * deg, -90 * deg, 90 * deg,
                              90 * deg, 90 * deg, 90 * deg, 90 * deg}))) {}

ObjectTracker::~ObjectTracker() {}

TrackerResults ObjectTracker::Update(cv::Mat& image) {
  TrackerResults retval;
  range.Thresh(image);

  // erode and dilate to remove noise
  int erodeWidth = image.cols * .003;
  cv::erode(image, image, cv::getStructuringElement(
                              cv::MORPH_ELLIPSE,
                              cv::Size(erodeWidth * 2 + 1, erodeWidth * 2 + 1),
                              cv::Point(erodeWidth / 2, erodeWidth / 2)));
  cv::dilate(image, image, cv::getStructuringElement(
                               cv::MORPH_ELLIPSE,
                               cv::Size(erodeWidth * 4 + 1, erodeWidth * 4 + 1),
                               cv::Point(erodeWidth, erodeWidth)));

  // get position of target
  detector.setData(image);

  if (detector.getScore() > 0) {
    // draw stuff on the image for debugging
    cv::cvtColor(image, image, CV_GRAY2BGR);
    cv::drawContours(
        image, std::vector<std::vector<cv::Point> >({detector.getPoints()}), 0,
        cv::Scalar(0, 255, 0), 3);
    cv::Rect boundRect = cv::boundingRect(detector.getPoints());
    // Scale from -FOV/2 to FOV/2
    retval.angle =
        FOV *
        ((boundRect.tl().x + boundRect.br().x) / (2.0 * image.cols) - 0.5);
    retval.is_found = true;
  } else {
    retval.angle = 0 * deg;
    retval.is_found = false;
  }
  return retval;
}

void ObjectTracker::SetThresholds(cv::Scalar low, cv::Scalar high) {
  range.Set(low, high);
}

const Angle ObjectTracker::FOV = 55 * deg;
const double ObjectTracker::minScore = 20;
