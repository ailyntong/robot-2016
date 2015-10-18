#include "controls/replay_control_loop.h"
#include "linux_code/init.h"

#include "control_loops/drivetrain/drivetrain.q"

// Reads one or more log files and sends out all the queue messages (in the
// correct order and at the correct time) to feed a "live" drivetrain process.

int main(int argc, char **argv) {
  if (argc <= 1) {
    fprintf(stderr, "Need at least one file to replay!\n");
    return EXIT_FAILURE;
  }

  ::aos::InitNRT();

  ::aos::controls::ControlLoopReplayer<::bot3::control_loops::DrivetrainQueue>
      replayer(&::bot3::control_loops::drivetrain_queue, "drivetrain");
  for (int i = 1; i < argc; ++i) {
    replayer.ProcessFile(argv[i]);
  }

  ::aos::Cleanup();
}
