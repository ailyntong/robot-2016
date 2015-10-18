#ifndef BOT3_CONTROL_LOOPS_DRIVETRAIN_DRIVETRAIN_DOG_MOTOR_PLANT_H_
#define BOT3_CONTROL_LOOPS_DRIVETRAIN_DRIVETRAIN_DOG_MOTOR_PLANT_H_

#include "control_loops/state_feedback_loop.h"

namespace bot3 {
namespace control_loops {

StateFeedbackPlantCoefficients<4, 2, 2> MakeDrivetrainLowLowPlantCoefficients();

StateFeedbackController<4, 2, 2> MakeDrivetrainLowLowController();

StateFeedbackPlantCoefficients<4, 2, 2> MakeDrivetrainLowHighPlantCoefficients();

StateFeedbackController<4, 2, 2> MakeDrivetrainLowHighController();

StateFeedbackPlantCoefficients<4, 2, 2> MakeDrivetrainHighLowPlantCoefficients();

StateFeedbackController<4, 2, 2> MakeDrivetrainHighLowController();

StateFeedbackPlantCoefficients<4, 2, 2> MakeDrivetrainHighHighPlantCoefficients();

StateFeedbackController<4, 2, 2> MakeDrivetrainHighHighController();

StateFeedbackPlant<4, 2, 2> MakeDrivetrainPlant();

StateFeedbackLoop<4, 2, 2> MakeDrivetrainLoop();

}  // namespace control_loops
}  // namespace bot3

#endif  // BOT3_CONTROL_LOOPS_DRIVETRAIN_DRIVETRAIN_DOG_MOTOR_PLANT_H_
