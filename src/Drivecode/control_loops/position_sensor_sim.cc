#include "control_loops/position_sensor_sim.h"

#include <cmath>

namespace bot3 {
namespace control_loops {

void PositionSensorSimulator::Initialize(double start_position, double hall_effect_position) {
  hall_effect_position_ = hall_effect_position;
  start_position_ = start_position;
  cur_pos_ = start_position;
}

void PositionSensorSimulator::MoveTo(double new_pos) { cur_pos_ = new_pos; }

// TODO Kelly did this because elevator isn't what we're concerned about

//void PositionSensorSimulator::GetSensorValues(
//    control_loops::ElevatorQueue::Position* position) {
//  position->encoder = cur_pos_ - start_position_;
//  if (cur_pos_ <= hall_effect_position_) {
//    position->bottom_hall_effect = true;
//  } else {
//    position->bottom_hall_effect = false;
//  }
//}

}  // namespace control_loops
}  // namespace bot3
