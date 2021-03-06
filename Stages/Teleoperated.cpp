#include "../Rowdy15/Rowdy15.h"

void RowdyFifteen::TeleopInit()
{
	myRobot.SetSafetyEnabled(false);

	UpdateOutputs();
}

void RowdyFifteen::TeleopPeriodic()
{
	/*
	 * Grab values from all of the joysticks as the raw values.
	 */
	l_x_raw = stickl.GetX();
	l_y_raw = stickl.GetY();
	r_x_raw = stickr.GetX();
	r_y_raw = stickr.GetY();

	drive_speed_ain_value = ds->GetAnalogIn(2);
	missile_switch_speed_multiplier = 1.0;

/*	if(ds->GetDigitalIn(8)) {
		if(eb_values[0x4]) {
			missile_switch_speed_multiplier *= 0.5;
		}

		if(eb_values[0x5]) {
			missile_switch_speed_multiplier *= 0.5;
		}

		if(eb_values[0x6]) {
			missile_switch_speed_multiplier *= 0.5;
		}
	}*/

	/*
	 * Apply weighting factors and alleviate the garbage that the joysticks
	 * output when resting (the phantom values).
	 */
	l_x = master_power_factor * RowdyMath::FloatNullZonify(l_x_raw, left_js_null_zone) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
	l_y = master_power_factor * RowdyMath::FloatNullZonify(l_y_raw, left_js_null_zone) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
	r_x = master_power_factor * RowdyMath::FloatNullZonify(r_x_raw, right_js_null_zone) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
	r_y = master_power_factor * RowdyMath::FloatNullZonify(r_y_raw, right_js_null_zone) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;

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
	 * This is an intentional duplicate of the above code. If both triggers are pulled,
	 * it would be nice if the values would go down by an additional power factor.
	 *
	 * So, if both triggers are pulled, then both left and right joysticks will be
	 * multiplied by 0.125 overall.
	 */
	if((l_t && r_t) && dual_triggers_multiply_again) {
		l_x *= master_trigger_power_factor;
		l_y *= master_trigger_power_factor;
		r_x *= master_trigger_power_factor;
		r_y *= master_trigger_power_factor;
	}

	/*
	 * Write the values for the rollers
	 */
	if(ea_values[left_roller_in]) {
		leftRollers.Set(1.0);
	} else if(ea_values[left_roller_out]) {
		leftRollers.Set(-1.0);
	} else {
		leftRollers.Set(0.0);
	}

	if(eb_values[right_roller_in]) {
		rightRollers.Set(-1.0);
	}
	else if(eb_values[right_roller_out]) {
		rightRollers.Set(1.0);
	}
	else {
		rightRollers.Set(0.0);
	}

	if(ea_values[both_roller_out]) {
		rightRollers.Set(1.0);
		leftRollers.Set(-1.0);
	} else if(ea_values[both_roller_in]) {
		rightRollers.Set(-1.0);
		leftRollers.Set(1.0);
	}

	/*
	 * Write the values for the wings
	 */

	if((ea_values[left_wing_up] || ea_values[both_wings_up]) && timer < 2000) {
		timer++;
		leftWing.Set(-0.6);
	} else if((ea_values[left_wing_down] || ea_values[both_wings_down]) && timer > -2000) {
		timer--;
		leftWing.Set(0.6);
	} else {
		if(!ea_values[right_wing_up] && !ea_values[right_wing_down])
		timer = 0;
		leftWing.Set(0.0);
	}

	if((ea_values[right_wing_up] || ea_values[both_wings_up]) && timer < 2000) {
		if(!ea_values[both_wings_up])
			timer++;
		rightWing.Set(0.6);
	} else if((ea_values[right_wing_down] || ea_values[both_wings_down]) && timer > -2000) {
		if(!ea_values[both_wings_down])
			timer--;
		rightWing.Set(-0.6);
	} else {
		if(!ea_values[left_wing_down] && !ea_values[left_wing_up])
			timer = 0;
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
