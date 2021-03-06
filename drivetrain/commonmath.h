/*
Many thanks to 971 Spartan Robotics for their help with this project
*/
#ifndef MUAN_COMMON_MATH_H_
#define MUAN_COMMON_MATH_H_

namespace aos {

// Clips a value so that it is in [min, max]
static inline double Clip(double value, double min, double max) {
  if (value > max) {
    value = max;
  } else if (value < min) {
    value = min;
  }
  return value;
}

template <typename T>
static inline int sign(T val) {
  if (val > T(0)) {
    return 1;
  } else {
    return -1;
  }
}

}  // namespace MUAN

#endif  // MUAN_COMMON_MATH_H_
