#include "Rowdy15/Rowdy15.h"

/*
 * This function is called whenever the robot is disabled.
 */
void RowdyFifteen::DisabledInit()
{
	LCDPrint();
	UpdateSmartDashboard();
}

/*
 * This function is called periodically after DisabledInit has been called and
 * up until the robot is enabled again.
 */
void RowdyFifteen::DisabledPeriodic()
{
	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}
