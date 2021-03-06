#ifndef AUTO_AUTO_FUNCTIONS_H_
#define AUTO_AUTO_FUNCTIONS_H_

#include "gyro/gyro_reader.h"
#include "muan/unitscpp/unitscpp.h"
#include "muan/utils/timing_utils.h"
#include "muan/control/trapezoidal_motion_profile.h"
#include "auto_functions.h"
#include "frc1678/frc1678.h"

enum Position {
  LONG = 0,
  TUCK,
  INTAKE,
  AUTO_SHOT,
  DEFENSE,
  TUCK_SPIN,
  INTAKE_SPIN,
  CLASS_A
};

enum IntakeStatus {
  OFF = 0,
  UNTIL,
  FOREVER
};

namespace AutoFunction {
void SetUpAutoFunction();
void DeleteAutoFunction();
bool SetWedge(CitrusRobot* robot, bool up);
bool DriveStraight(
    CitrusRobot* robot,
    float dist, bool highgear);  // TODO(Wesley) Change name to be the same as elsewhere
bool DriveSlowStraight(
    CitrusRobot* robot,
    float dist, bool highgear);  // TODO(Wesley) Change name to be the same as elsewhere
bool DriveYolo(
    CitrusRobot* robot,
    float dist, bool highgear);
bool DriveYoloAtAngle(
    CitrusRobot* robot,
    float dist, float angle, bool highgear);
bool DriveStraightAtAngle(CitrusRobot* robot, float dist, float angle, bool highgear);
bool PointTurn(CitrusRobot* robot, float angle);
bool AbsolutePointTurn(CitrusRobot* robot, float angle);
bool Shift(CitrusRobot* robot, bool highgear);
bool Wait(CitrusRobot* robot, float time);
bool EncoderWait(CitrusRobot* robot, float dist);
bool Shoot(CitrusRobot* robot);
bool RunIntake(CitrusRobot* robot, IntakeStatus run);
bool WaitForBall(CitrusRobot* robot);
bool SetArmPosition(CitrusRobot* robot, Position arm_position);
bool CheckArmCalibration(CitrusRobot* robot);
bool DropBall(CitrusRobot* robot);
bool Align(CitrusRobot* robot);
bool StopDriving(CitrusRobot* robot);
}

#endif
