#include "../Rowdy15/Rowdy15.h"

/*
 * Runs during test mode
 */
void RowdyFifteen::TestInit()
{
	SetJoystickButtonValueRegisters();

	myRobot.SetSafetyEnabled(false);

	LCDPrint();
	UpdateSmartDashboard();
}

/*
 * This function gets called periodically in Test mode
 */
void RowdyFifteen::TestPeriodic()
{
	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}
