#include "../Rowdy15/Rowdy15.h"

/*
 * Runs during test mode
 */
void RowdyFifteen::TestInit()
{
	SetJoystickButtonValueRegisters();

	myRobot.SetSafetyEnabled(false);

	UpdateOutputs();
}

/*
 * This function gets called periodically in Test mode
 */
void RowdyFifteen::TestPeriodic()
{
	SetJoystickButtonValueRegisters();

	/*
	 * Get the trigger values.
	 */
	bool l_t = l_values[0x0];
	bool r_t = r_values[0x0];

	/*
	 * If either of the triggers is pressed, apply
	 * a weighting factor to all of the Joystick values.
	 */
	if(l_t) {
		l_x *= master_trigger_power_factor;
		l_y *= master_trigger_power_factor;
	}

	if(r_t) {
		r_x *= master_trigger_power_factor;
		r_y *= master_trigger_power_factor;
	}

	/*
	 * Write the values for the rollers
	 */
	if(eb_values[0x1]) {
		leftRollers.Set(1.0);
	} else if(eb_values[0x0]) {
		leftRollers.Set(-1.0);
	} else {
		leftRollers.Set(0.0);
	}

	if(ea_values[0x8]) {
		rightRollers.Set(1.0);
	} else if(ea_values[0x9]) {
		rightRollers.Set(-1.0);
	} else {
		rightRollers.Set(0.0);
	}

	if(ea_values[0xa]) {
		rightRollers.Set(1.0);
		leftRollers.Set(-1.0);
	} else if(ea_values[0xb]) {
		rightRollers.Set(-1.0);
		leftRollers.Set(1.0);
	}

	/*
	 * Write the values for the wings
	 */
	if(ea_values[0x6] || ea_values[0x4]) {
		leftWing.Set(-1.0);
	} else if(ea_values[0x7] || ea_values[0x5]) {
		leftWing.Set(1.0);
	} else {
		leftWing.Set(0.0);
	}

	if(ea_values[0x2] || ea_values[0x4]) {
		rightWing.Set(1.0);
	} else if(ea_values[0x3] || ea_values[0x5]) {
		rightWing.Set(-1.0);
	} else {
		rightWing.Set(0.0);
	}

	if(ea_values[0x0]) {
		aSolenoid.Set(true);
		bSolenoid.Set(true);
	} else {
		aSolenoid.Set(false);
		bSolenoid.Set(false);
	}

	/*
	 * Input the values into the drive function.
	 */
	myRobot.MecanumDrive_Cartesian(l_x, l_y, r_x);

	UpdateOutputs();
}
