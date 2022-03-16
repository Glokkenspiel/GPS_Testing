#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
gps GPS = gps(PORT20, 0.00, 0.00, mm, 180);
motor leftBaseMotorA = motor(PORT1, ratio18_1, false);
motor leftBaseMotorB = motor(PORT2, ratio18_1, false);
motor_group leftBase = motor_group(leftBaseMotorA, leftBaseMotorB);
motor rightBaseMotorA = motor(PORT3, ratio18_1, true);
motor rightBaseMotorB = motor(PORT4, ratio18_1, true);
motor_group rightBase = motor_group(rightBaseMotorA, rightBaseMotorB);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}