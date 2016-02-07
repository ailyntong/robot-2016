#ifndef AUTO_AUTO_FUNCTIONS_H_
#define AUTO_AUTO_FUNCTIONS_H_

#include "gyro/gyro_reader.h"
#include "unitscpp/unitscpp.h"
#include "muan/utils/timing_utils.h"
#include "muan/control/trapezoidal_motion_profile.h"
#include "auto_functions.h"
#include "frc1678/frc1678.h"

enum Position {
  // Shooting positions
  LOW_BAR,
  BATTER,
  WORKS_3,
  WORKS_4,
  // Other positions
  TUCK
};

namespace AutoFunction {
void SetUpAutoFunction();
void DeleteAutoFunction();
bool DriveStraight(CitrusRobot* robot, float dist);
bool PointTurn(CitrusRobot *robot, float angle);
bool Wait(CitrusRobot* robot, Time time);
bool Shoot(CitrusRobot* robot, Position infield);
bool RunIntake(CitrusRobot* robot);
bool SetArmPosition(CitrusRobot* robot, Position arm_position);
bool DropPinch(CitrusRobot* robot);
bool Align(CitrusRobot* robot);
bool StopDriving(CitrusRobot* robot);
}

#endif