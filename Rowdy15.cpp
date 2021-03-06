#include "Rowdy15/Rowdy15.h"

RowdyFifteen::RowdyFifteen(void):
	myRobot(dt_pwm_front_left, dt_pwm_rear_left, dt_pwm_front_right, dt_pwm_rear_right),
	stickl(j_left),
	stickr(j_right),
	stickea(j_3),
	stickeb(j_4),
	leftWing(w_pwm_wing_left),
	rightWing(w_pwm_wing_right),
	leftRollers(w_pwm_roller_left),
	rightRollers(w_pwm_roller_right),
	aSolenoid(p_sol_shooter_a),
	bSolenoid(p_sol_shooter_b),
	compressor(5, p_rel_compressor)
{
	/*
	 * Configure the joysticks to have the correct channels.
	 */
	stickl.SetAxisChannel(Joystick::kXAxis, 1);
	stickl.SetAxisChannel(Joystick::kYAxis, 2);
	stickr.SetAxisChannel(Joystick::kXAxis, 1);
	stickr.SetAxisChannel(Joystick::kYAxis, 2);

	/*
	 * Get the raw axis values from the joysticks
	 */
	l_x_raw = stickl.GetX();
	l_y_raw = stickl.GetY();
	r_x_raw = stickr.GetX();
	r_y_raw = stickr.GetY();

	/*
	 * We shouldn't party right-the-bat.  Maybe on days when
	 * false is true, but not right off-the-bat.
	 */
	party_mode = false;
	
	/* The timer thing. #hacksy
	 * 
	 */
	timer = 0;

	/*
	 * Get an instance of the Driver Station stuff
	 */
	ds = DriverStation::GetInstance();
	b = DriverStationLCD::GetInstance();

	b->Clear();
	b->UpdateLCD();

	/*
	 * Invert the motors.
	 *
	 * This helps Mecanum_Cartesian to work properly.
	 */
	myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, false);
	myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
	myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, false);
	myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);

	/*
	 * Start the compressor, which will watch the digital sensor and turn
	 * on/off depending on if it is full. It will do this whenever IsEnabled()
	 * returns true.
	 */
	compressor.Start();

	/*
	 * Initializes the SmartDashboard.
	 */
	SmartDashboard::init();
}

void RowdyFifteen::FactorJoystickValues(float *result, float *raw, float null_zone, float factor)
{
	if(*raw <= +null_zone || *raw >= -null_zone) {
		*raw = 0.0;
	}

	*result = *raw * factor;
}

void RowdyFifteen::UpdateOutputs()
{
	UpdateSmartDashboard();
	LCDPrint();
}

/*
 * This function updates the SmartDashboard values
 */
void RowdyFifteen::UpdateSmartDashboard()
{
	SmartDashboard::PutBoolean("Compressor", (bool) compressor.GetPressureSwitchValue());

	//	SmartDashboard::PutBoolean("Blue Missile Switch", !eb_values[0x4]);
	//	SmartDashboard::PutBoolean("Orange Missile Switch", eb_values[0x5]);
	//	SmartDashboard::PutBoolean("Green Missile Switch", eb_values[0x6]);

	SmartDashboard::PutBoolean("Lower Power Button",ea_values[lower_power_button]);

	SmartDashboard::PutBoolean("Left Wing Up Button", ea_values[left_wing_up]);
	SmartDashboard::PutBoolean("Left Wing Down Button", ea_values[left_wing_down]);
	SmartDashboard::PutBoolean("Both Wings Up Button", ea_values[both_wings_up]);
	SmartDashboard::PutBoolean("Both Wings Down Button", ea_values[both_wings_down]);
	SmartDashboard::PutBoolean("Right Wing Up Button", ea_values[right_wing_up]);
	SmartDashboard::PutBoolean("Right Wing Down Button", ea_values[right_wing_down]);

	SmartDashboard::PutBoolean("Left Rollers Out Switch", ea_values[left_roller_out]);
	SmartDashboard::PutBoolean("Left Rollers In Switch", ea_values[left_roller_in]);
	SmartDashboard::PutBoolean("Both Rollers Out Switch", ea_values[both_roller_out]);
	SmartDashboard::PutBoolean("Both Rollers In Switch", ea_values[both_roller_in]);
	SmartDashboard::PutBoolean("Right Rollers Out Switch", ea_values[right_roller_out]);
	SmartDashboard::PutBoolean("Right Rollers In Switch", ea_values[right_roller_in]);

	SmartDashboard::PutBoolean("Fire Button", ea_values[fire_button]);
}

/*
 * This function updates the values in `value_registry' by grabbing the button
 * inputs from `joystick'
 */
void RowdyFifteen::SetJoystickButtonValueRegister(Joystick *joystick, vector<bool> *value_registry)
{
	/*
	 * Clear the registry vector.
	 */
	value_registry->clear();

		
	for(uint32_t i = 0; i < 12; i += 1) {
		/*
		 * Push the value for each channel (1-12)
		 */
		value_registry->push_back(joystick->GetRawButton(i + 1));
	}
}

void RowdyFifteen::SetJoystickButtonValueRegisters()
{
	SetJoystickButtonValueRegister( &stickl,  &l_values);
	SetJoystickButtonValueRegister( &stickr,  &r_values);
	SetJoystickButtonValueRegister(&stickea, &ea_values);
	SetJoystickButtonValueRegister(&stickeb, &eb_values);
}

void RowdyFifteen::LCDPrint()
{
	b->Clear();
	b->Printf(b->kUser_Line1, 1, "l %s%s%s%s%s%s%s%s%s%s%s%s",
	          (l_values[0x0] ? "1" : ""),
	          (l_values[0x1] ? "2" : ""),
	          (l_values[0x2] ? "3" : ""),
	          (l_values[0x3] ? "4" : ""),
	          (l_values[0x4] ? "5" : ""),
	          (l_values[0x5] ? "6" : ""),
	          (l_values[0x6] ? "7" : ""),
	          (l_values[0x7] ? "8" : ""),
	          (l_values[0x8] ? "9" : ""),
	          (l_values[0x9] ? "a" : ""),
	          (l_values[0xa] ? "b" : ""),
	          (l_values[0xb] ? "c" : ""));
	b->Printf(b->kUser_Line2, 1, "r %s%s%s%s%s%s%s%s%s%s%s%s",
	          (r_values[0x0] ? "1" : ""),
	          (r_values[0x1] ? "2" : ""),
	          (r_values[0x2] ? "3" : ""),
	          (r_values[0x3] ? "4" : ""),
	          (r_values[0x4] ? "5" : ""),
	          (r_values[0x5] ? "6" : ""),
	          (r_values[0x6] ? "7" : ""),
	          (r_values[0x7] ? "8" : ""),
	          (r_values[0x8] ? "9" : ""),
	          (r_values[0x9] ? "a" : ""),
	          (r_values[0xa] ? "b" : ""),
	          (r_values[0xb] ? "c" : ""));
	b->Printf(b->kUser_Line3, 1, "a %s%s%s%s%s%s%s%s%s%s%s%s",
	          (ea_values[0x0] ? "1" : ""),
	          (ea_values[0x1] ? "2" : ""),
	          (ea_values[0x2] ? "3" : ""),
	          (ea_values[0x3] ? "4" : ""),
	          (ea_values[0x4] ? "5" : ""),
	          (ea_values[0x5] ? "6" : ""),
	          (ea_values[0x6] ? "7" : ""),
	          (ea_values[0x7] ? "8" : ""),
	          (ea_values[0x8] ? "9" : ""),
	          (ea_values[0x9] ? "a" : ""),
	          (ea_values[0xa] ? "b" : ""),
	          (ea_values[0xb] ? "c" : ""));
	b->Printf(b->kUser_Line4, 1, "b %s%s%s%s%s%s%s%s%s%s%s%s",
	          (eb_values[0x0] ? "1" : ""),
	          (eb_values[0x1] ? "2" : ""),
	          (eb_values[0x2] ? "3" : ""),
	          (eb_values[0x3] ? "4" : ""),
	          (eb_values[0x4] ? "5" : ""),
	          (eb_values[0x5] ? "6" : ""),
	          (eb_values[0x6] ? "7" : ""),
	          (eb_values[0x7] ? "8" : ""),
	          (eb_values[0x8] ? "9" : ""),
	          (eb_values[0x9] ? "a" : ""),
	          (eb_values[0xa] ? "b" : ""),
	          (eb_values[0xb] ? "c" : ""));
	b->Printf(b->kUser_Line5, 1, "joyl (%f, %f)", l_x, l_y);
	b->Printf(b->kUser_Line6, 1, "joyr (%f, %f)", r_x, r_y);
	b->UpdateLCD();
}

START_ROBOT_CLASS(RowdyFifteen);
