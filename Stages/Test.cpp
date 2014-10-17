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
	if(ea_values[left_roller_out]) {
		leftRollers.Set(1.0);
	} else if(ea_values[left_roller_in]) {
		leftRollers.Set(-1.0);
	} else {
		leftRollers.Set(0.0);
	}

	if(eb_values[right_roller_in]) {
		rightRollers.Set(1.0);
	} else if(eb_values[right_roller_out]) {
		rightRollers.Set(-1.0);
	} else {
		rightRollers.Set(0.0);
	}

	if(ea_values[both_roller_in]) {
		rightRollers.Set(1.0);
		leftRollers.Set(-1.0);
	} else if(ea_values[both_roller_out]) {
		rightRollers.Set(-1.0);
		leftRollers.Set(1.0);
	}

	/*
	 * Write the values for the wings
	 */
	if(ea_values[left_wing_up] || ea_values[both_wings_up]) {
		leftWing.Set(-1.0);
	} else if(ea_values[left_wing_down] || ea_values[both_wings_down]) {
		leftWing.Set(1.0);
	} else {
		leftWing.Set(0.0);
	}

	if(ea_values[right_wing_up] || ea_values[both_wings_up]) {
		rightWing.Set(1.0);
	} else if(ea_values[right_wing_down] || ea_values[both_wings_down]) {
		rightWing.Set(-1.0);
	} else {
		rightWing.Set(0.0);
	}

	if(ea_values[fire_button]) {
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
