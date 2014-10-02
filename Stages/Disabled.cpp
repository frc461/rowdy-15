#include "../Rowdy15/Rowdy15.h"

/*
 * This function is called whenever the robot is disabled.
 */
void RowdyFifteen::DisabledInit()
{
	UpdateOutputs();
}

/*
 * This function is called periodically after DisabledInit has been called and
 * up until the robot is enabled again.
 */
void RowdyFifteen::DisabledPeriodic()
{
	SetJoystickButtonValueRegisters();

	if(party_mode) {
		for(uint32_t i = 1; i <= 8; i += 1) {
			ds->SetDigitalOut(rand() % 2);
		}
	}

	UpdateOutputs();
}
