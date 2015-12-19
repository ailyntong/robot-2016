#ifndef Y2015_BOT3_CONTROL_LOOPS_DRIVETRAIN_H_
#define Y2015_BOT3_CONTROL_LOOPS_DRIVETRAIN_H_

#include <string.h>
#include "Eigen/Dense"

#include "drivetrain/polytope.h"

namespace drivetrain {
namespace control_loops {

// Constants
// TODO(jasmine): Figure out what these refer to and get actual constants.
constexpr double kDrivetrainTurnWidth = 0.63;
constexpr double kDrivetrainDoneDistance = 0.02;
constexpr double kDrivetrainEncoderRatio = 18.0 / 44.0;
constexpr double kDrivetrainHighGearRatio =
    kDrivetrainEncoderRatio * 18.0 / 60.0;
constexpr double kDrivetrainLowGearRatio = kDrivetrainHighGearRatio;
const bool kDrivetrainClutchTransmission = false;
// End constants

// Structs to carry information about the drivetrain.
struct DrivetrainGoal {
    double steering;
    double throttle;
    bool highgear;
    bool quickturn;
    bool control_loop_driving;
    double left_goal;
    double left_velocity_goal;
    double right_goal;
    double right_velocity_goal;
  };

struct DrivetrainPosition {
    double left_encoder;
    double right_encoder;
    bool left_shifter_high;
    bool right_shifter_high; // TODO (jasmine): do we need both of these?
  };

struct DrivetrainOutput {
    double left_voltage;
    double right_voltage;
    bool left_high; // These two are for shifting.
    bool right_high;
  };

struct DrivetrainStatus {
    double robot_speed;
    double filtered_left_position;
    double filtered_right_position;
    double filtered_left_velocity;
    double filtered_right_velocity;

    double uncapped_left_voltage;
    double uncapped_right_voltage;
    bool output_was_capped;

    bool is_done;
  };


class DrivetrainLoop {
 public:
  // TODO (Finn): Write a constructor to zero-initialize everything?

  // Returns true if all the counters etc in the sensor data have been reset.
  // This will return true only a single time per reset.
  bool WasReset() {
    if (reset_) {
      reset_ = false;
      return true;
    } else {
      return false;
    }
  }

  // Constructs and sends a message on the output queue which sets everything to
  // a safe state (generally motors off). For some subclasses, this will be a
  // bit different (ie pistons).
  // The implementation here creates a new Output message, calls Zero() on it,
  // and then sends it.
  // TODO (jasmine): Send everything to a safe state, usually 'motors off', without the 971 idiom.
  void ZeroOutputs();

  // Runs the loop forever.
  void Run() override;

  // Runs one cycle of the loop.
  void Iterate() override;

  // Executes one cycle of the control loop.
  void RunIteration(
      const DrivetrainGoal *goal,
      const DrivetrainPosition *position,
      DrivetrainOutput *output,
      DrivetrainStatus *status);

 private:
  bool reset_ = false;
};

}  // namespace control_loops
}  // namespace drivetrain

#endif  // Y2015_BOT3_CONTROL_LOOPS_DRIVETRAIN_H_