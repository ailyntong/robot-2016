#include "auto_functions.h"
#include "muan/unitscpp/unitscpp.h"
#include "arm/arm_subsystem.h"

#include <stdio.h>

void AutoFunction::SetUpAutoFunction() {
  // Timer wait_timer = new Timer();
}

void AutoFunction::DeleteAutoFunction() {
  // delete wait_timer;
}

// Test for LemonScript
bool newDriveState = true;
bool AutoFunction::DriveStraight(CitrusRobot* robot, float dist) {
  if (newDriveState) {
    robot->subsystems_.drive.DriveDistance(dist * ft);
    newDriveState = false;
  }

  if (robot->subsystems_.drive.IsProfileComplete()) {
    newDriveState = true;
    return true;
  } else {
    return false;
  }
}

bool AutoFunction::SetWedge(CitrusRobot* robot, bool up) {
  robot->is_wedge_deployed_ = !up;
  return true;
}

bool newDriveAtAngleState = true;
bool AutoFunction::DriveStraightAtAngle(CitrusRobot* robot, float dist,
                                        float angle) {
  if (newDriveAtAngleState) {
    robot->subsystems_.drive.DriveDistanceAtAngle(dist * ft, angle * deg);
    newDriveAtAngleState = false;
  }

  if (robot->subsystems_.drive.IsProfileComplete()) {
    newDriveAtAngleState = true;
    return true;
  } else {
    return false;
  }
}

bool newTurnState = true;
bool AutoFunction::PointTurn(CitrusRobot* robot, float angle) {
  if (newTurnState) {
    robot->subsystems_.drive.PointTurn(angle * deg);
    newTurnState = false;
  }

  if (robot->subsystems_.drive.IsProfileComplete()) {
    newTurnState = true;
    return true;
  } else {
    return false;
  }
}

bool newAbsTurnState = true;
bool AutoFunction::AbsolutePointTurn(CitrusRobot* robot, float angle) {
  if (newAbsTurnState) {
    robot->subsystems_.drive.AbsolutePointTurn(angle * deg);
    newAbsTurnState = false;
  }

  if (robot->subsystems_.drive.IsProfileComplete()) {
    newAbsTurnState = true;
    return true;
  } else {
    return false;
  }
}

bool AutoFunction::Shift(CitrusRobot* robot, bool highgear) {
  robot->subsystems_.drive.Shift(highgear);
  return true;
}

Time startTime = -1 * s;
bool AutoFunction::Wait(CitrusRobot* robot, float time) {
  if (startTime < 0 * s) {
    startTime = muan::now();
  }

  if (muan::now() - startTime > time * s) {
    startTime = -1 * s;
    return true;
  }

  return false;
}

Length start_dist;
bool newEncoderWaitState = true;
bool AutoFunction::EncoderWait(CitrusRobot* robot, float dist) {
  if (newEncoderWaitState) {
    newEncoderWaitState = false;
    start_dist = robot->subsystems_.drive.GetDistanceDriven();
    return false;
  }

  if (dist >= 0 &&
      robot->subsystems_.drive.GetDistanceDriven() - start_dist >= dist * ft) {
    newEncoderWaitState = true;
    return true;
  } else if (dist < 0 &&
             robot->subsystems_.drive.GetDistanceDriven() - start_dist <=
                 dist * ft) {
    newEncoderWaitState = true;
    return true;
  } else
    return false;
}

bool AutoFunction::Shoot(CitrusRobot* robot) {
  // Need to set arm position to LONG before calling this shoot
  // Wait for shooter to reach shooting speed
  if (robot->subsystems_.arm.IsDone()) {
    robot->subsystems_.arm.Shoot();
    return true;  // shooter->finished();
  }

  return false;
}

bool AutoFunction::RunIntake(CitrusRobot* robot, bool run) {
  robot->subsystems_.arm.SetIntake(run ? IntakeGoal::FORWARD_UNTIL : IntakeGoal::OFF);
  return true;
}

bool newArmPosition = true;
bool AutoFunction::SetArmPosition(CitrusRobot* robot, Position arm_position) {
  if (newArmPosition) {
    switch (arm_position) {
      case LONG:
        robot->subsystems_.arm.GoToLong();
        break;
      case TUCK:
        robot->subsystems_.arm.GoToTuck();
        break;
      case INTAKE:
        robot->subsystems_.arm.GoToIntake();
        break;
      case AUTO_SHOT:
        robot->subsystems_.arm.GoToAutoShot();
        break;
      case DEFENSE:
        robot->subsystems_.arm.GoToDefensive();
        break;
      case TUCK_SPIN:
        robot->subsystems_.arm.GoToTuckSpin();
        break;
      case INTAKE_SPIN:
        robot->subsystems_.arm.GoToIntakeSpin();
        break;
    }

    newArmPosition = false;
  }

  if (robot->subsystems_.arm.IsDone()) {
    newArmPosition = true;
    return true;
  } else {
    return false;
  }
}

bool AutoFunction::CheckArmCalibration(CitrusRobot* robot) {
  return robot->subsystems_.arm.IsCalibrated();
}

bool AutoFunction::DropPinch(CitrusRobot* robot) {
  return true;
}  // Why is this needed?

bool toStartVision = true;
bool AutoFunction::Align(CitrusRobot* robot) {
  if (toStartVision) {
    robot->vision_.Start();
    toStartVision = false;
  }

  if (robot->vision_.Update(
          true)) {  // (TODO) Ash: Modify when vision Update() is fixed.
    toStartVision = true;
    return true;
  }

  return false;
}

bool AutoFunction::StopDriving(CitrusRobot* robot) {
  // driveSystem->DriveStraight(0,0);
  return true;
}
