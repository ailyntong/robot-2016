#ifndef BOT3_CONTROL_LOOPS_DRIVETRAIN_POLYDRIVETRAIN_CIM_PLANT_H_
#define BOT3_CONTROL_LOOPS_DRIVETRAIN_POLYDRIVETRAIN_CIM_PLANT_H_

#include "Drivecode/control_loops/state_feedback_loop.h"

namespace bot3 {
namespace control_loops {

StateFeedbackPlantCoefficients<1, 1, 1> MakeCIMPlantCoefficients();

StateFeedbackController<1, 1, 1> MakeCIMController();

StateFeedbackPlant<1, 1, 1> MakeCIMPlant();

StateFeedbackLoop<1, 1, 1> MakeCIMLoop();

}  // namespace control_loops
}  // namespace bot3

#endif  // BOT3_CONTROL_LOOPS_DRIVETRAIN_POLYDRIVETRAIN_CIM_PLANT_H_
