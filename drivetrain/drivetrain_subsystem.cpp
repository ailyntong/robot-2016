#include "drivetrain_subsystem.h"

using mutex_lock = std::lock_guard<std::mutex>;

// this code is an ugly hack but thats ok because the world is an ugly hack.

DrivetrainSubsystem::DrivetrainSubsystem()
    : muan::Updateable(200 * hz),
      angle_controller_(60 * 0.6 * V / rad,
                        ((2 * (60 * 0.6)) / 0.2) * V / rad / s,
                        (((60 * 0.6) * 0.2) / 8) * V / rad * s),
      // angle_controller_(60*V/rad, 0*V/rad/s, 0*V/rad*s),
      distance_controller_(3 * V / m, 0 * V / m / s, 0 * V / m * s),
      event_log_("drivetrain_subsystem"),
      csv_log_("drivetrain_subsystem", {"enc_left", "enc_right", "pwm_left",
                                        "pwm_right", "gyro_angle", "gear"}) {
  event_log_.Write("Initializing drivetrain subsystem components...", "INIT",
                   CODE_STAMP);
  drive_ = std::make_unique<RobotDrive>(RobotPorts::drive_left,
                                        RobotPorts::drive_right);

  drive_loop_ = std::make_unique<DrivetrainLoop>();

  left_encoder_ = std::make_unique<Encoder>(RobotPorts::left_encoder_a,
                                            RobotPorts::left_encoder_b);
  right_encoder_ = std::make_unique<Encoder>(RobotPorts::right_encoder_a,
                                             RobotPorts::right_encoder_b);

  shifting_ = std::make_unique<DoubleSolenoid>(RobotPorts::shift_a,
                                               RobotPorts::shift_b);

  gyro_reader_ = std::make_unique<GyroReader>();

  event_log_.Write("Done initializing drivetrain subsystem components", "INIT",
                   CODE_STAMP);
}

DrivetrainSubsystem::~DrivetrainSubsystem() {}

void DrivetrainSubsystem::Start() {
  event_log_.Write("Starting drivetrain subsystem...", "INIT", CODE_STAMP);
  drive_->SetSafetyEnabled(false);
  gyro_reader_->Start();
  is_operator_controlled_ = true;
  t = 0 * s;
  Updateable::Start();  // This needs to be called last so that Update(Time)
                        // doesn't get called until after everything is
                        // initialized
  event_log_.Write("Drivetrain subsystem started", "INIT", CODE_STAMP);
}

void DrivetrainSubsystem::Update(Time dt) {
  DrivetrainPosition pos;
  DrivetrainOutput out;
  DrivetrainStatus status;

  SetDrivePosition(&pos);

  {
    mutex_lock lock(mu_);
    if (is_operator_controlled_) {
      drive_loop_->RunIteration(&current_goal_, &pos, &out, &status);
      // printf("ol: %f\tor: %f\n", out.left_voltage, out.right_voltage);
      // TODO(Wesley) Find out why this is giving 12V and 0V as output
    } else {
      // TODO(Wesley) Reset the PID controller if we went from tele to auto
      // TODO(Wesley) Add operator control to exit auto mode
      t += dt;

      // Feed forward term
      Voltage feed_forward_angle = GetAngleFFVoltage(
          angle_profile_->CalculateDerivative(),
          angle_profile_->CalculateSecondDerivative(), pos.right_shifter_high);

      Velocity max_robot_velocity = 15 * ft / s;
      Voltage feed_forward_distance =
          distance_profile_->CalculateDerivative(t) *
          (12 * V / max_robot_velocity);

      // PID Term
      Angle target_angle_ = angle_profile_->Calculate(t);
      Length target_distance_ = distance_profile_->Calculate(t);

      // The difference between the gyro at the start of the profile and now
      Angle angle_from_start = gyro_reader_->GetAngle() - gyro_offset_;

      Length distance_from_start =
          ((pos.left_encoder + pos.right_encoder) / 2) * m -
          encoder_offset_ * m;

      Voltage correction_angle =
          angle_controller_.Calculate(dt, target_angle_ - angle_from_start);
      Voltage correction_distance = distance_controller_.Calculate(
          dt, target_distance_ - distance_from_start);

      Voltage out_left = feed_forward_angle + correction_angle;
      Voltage out_right = -feed_forward_angle - correction_angle;

      out.left_voltage = out_left.to(V);
      out.right_voltage = out_right.to(V);

      last_angle_ = angle_from_start;

      bool profiles_finished_time = angle_profile_->finished(t);
      bool profile_finished_distance =
          target_distance_ >=
              target_distance_ - (distance_from_start - 2 * cm) &&
          target_distance_ <= target_distance_ + (distance_from_start + 2 * cm);
      bool profile_finished_angle =
          std::abs((angle_from_start - angle_profile_->Calculate(t)).to(deg)) <
          .2 * deg;

      if (profiles_finished_time && profile_finished_angle) {
        angle_profile_.release();
        distance_profile_.release();
        is_operator_controlled_ = true;
        angle_controller_.Reset();
        printf("[motion] Finished motion profiles: %f deg in %f sec :)\n",
               angle_from_start.to(deg), t.to(s));
      }
    }
  }

  // TODO (Finn): Also deal with shifting output and with logging
  // from the status.
  drive_->TankDrive(out.left_voltage / 12.0, out.right_voltage / 12.0, false);
  shifting_->Set(current_goal_.highgear ? DoubleSolenoid::Value::kForward
                                        : DoubleSolenoid::Value::kReverse);

  csv_log_["enc_left"] = std::to_string(pos.left_encoder);
  csv_log_["enc_right"] = std::to_string(pos.right_encoder);
  csv_log_["pwm_left"] = std::to_string(out.left_voltage);
  csv_log_["pwm_right"] = std::to_string(out.right_voltage);
  csv_log_["gyro_angle"] = std::to_string(pos.gyro_angle);
  csv_log_["gear"] = current_goal_.highgear ? "high" : "low";
  csv_log_.EndLine();  // Flush the current row of the log
}

void DrivetrainSubsystem::SetDriveGoal(const DrivetrainGoal& goal) {
  mutex_lock lock(mu_);
  current_goal_ = goal;
}

void DrivetrainSubsystem::SetDrivePosition(
    DrivetrainPosition* drivetrain_position) {
  double click =
      3.14159 * .1016 / 360.0;  // Translating encoders into ground distances.
  drivetrain_position->left_encoder =
      left_encoder_->Get() * click;  // TODO (Ash): Get this from the encoders
                                     // in the right units and direction.
  drivetrain_position->right_encoder = -right_encoder_->Get() * click;
  drivetrain_position->gyro_angle = gyro_reader_->GetAngle().to(rad);
  drivetrain_position->left_shifter_high = current_goal_.highgear;
  drivetrain_position->right_shifter_high = current_goal_.highgear;
}

void DrivetrainSubsystem::FollowMotionProfile(
    std::unique_ptr<muan::MotionProfile<Length>> distance_profile,
    std::unique_ptr<muan::MotionProfile<Angle>> angle_profile) {
  distance_profile_ = std::move(distance_profile);
  angle_profile_ = std::move(angle_profile);
  is_operator_controlled_ = false;
  t = 0 * s;
  DrivetrainPosition pos;
  SetDrivePosition(&pos);  // Is this bad?
  encoder_offset_ = (pos.left_encoder + pos.right_encoder) / 2;
  gyro_offset_ = gyro_reader_->GetAngle();
  angle_controller_.Reset();
  distance_controller_.Reset();
}

bool DrivetrainSubsystem::IsProfileComplete() {
  return !(distance_profile_ && angle_profile_);
}

void DrivetrainSubsystem::CancelMotionProfile() {
  mutex_lock lock(mu_);
  distance_profile_.release();
  angle_profile_.release();
  is_operator_controlled_ = true;
}

Angle DrivetrainSubsystem::GetGyroAngle() { return gyro_reader_->GetAngle(); }

Voltage DrivetrainSubsystem::GetAngleFFVoltage(AngularVelocity velocity,
                                               AngularAcceleration acceleration,
                                               bool highgear) {
  // omega_dot = c1 * (V-c2*omega)
  // V = c2*omega + omega_dot / c1
  AngularVelocity max_robot_angular_velocity =
      240 * deg / s * (highgear ? 1 : .333);
  const auto c2 = 24 * V / max_robot_angular_velocity;
  const auto c1 = 100 / V * (rad / s / s);
  return c2 * velocity + acceleration / c1;
}

Voltage DrivetrainSubsystem::GetDistanceFFVoltage(Velocity velocity,
                                                  Acceleration acceleration) {}
