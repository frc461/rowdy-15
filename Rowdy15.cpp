#include <math.h>
#include "WPILib.h"
#include "Rowdy15.h"

#define master_js_null_zone 0.06
#define left_js_null_zone master_js_null_zone
#define right_js_null_zone master_js_null_zone

#define master_power_factor 0.8
#define master_trigger_power_factor 0.25

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
	compressor(5, 3)
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

/*
 * This function updates the SmartDashboard values
 */
void RowdyFifteen::UpdateSmartDashboard()
{
	SmartDashboard::PutBoolean("Compressor", (bool) compressor.GetPressureSwitchValue());

	SmartDashboard::PutBoolean("Blue Missile Switch", eb_values[0x4]);
	SmartDashboard::PutBoolean("Orange Missile Switch", eb_values[0x5]);
	SmartDashboard::PutBoolean("Green Missile Switch", eb_values[0x6]);

	SmartDashboard::PutBoolean("Left Wing Up Button", ea_values[0x6]);
	SmartDashboard::PutBoolean("Left Wing Down Button", ea_values[0x7]);
	SmartDashboard::PutBoolean("Both Wings Up Button", ea_values[0x4]);
	SmartDashboard::PutBoolean("Both Wings Down Button", ea_values[0x5]);
	SmartDashboard::PutBoolean("Right Wing Up Button", ea_values[0x2]);
	SmartDashboard::PutBoolean("Right Wing Down Button", ea_values[0x3]);

	SmartDashboard::PutBoolean("Left Rollers Out Switch", eb_values[0x0]);
	SmartDashboard::PutBoolean("Left Rollers In Switch", eb_values[0x1]);
	SmartDashboard::PutBoolean("Both Rollers Out Switch", ea_values[0xa]);
	SmartDashboard::PutBoolean("Both Rollers In Switch", ea_values[0xb]);
	SmartDashboard::PutBoolean("Right Rollers Out Switch", ea_values[0x8]);
	SmartDashboard::PutBoolean("Right Rollers In Switch", ea_values[0x9]);

	SmartDashboard::PutBoolean("Fire Button", ea_values[0x0]);
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
	b->UpdateLCD();
}

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

void RowdyFifteen::TeleopInit()
{
	myRobot.SetSafetyEnabled(false);

	LCDPrint();
	UpdateSmartDashboard();
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

	if(ds->GetDigitalIn(8)) {
		if(eb_values[0x4]) {
			missile_switch_speed_multiplier *= 0.5;
		}

		if(eb_values[0x5]) {
			missile_switch_speed_multiplier *= 0.5;
		}

		if(eb_values[0x6]) {
			missile_switch_speed_multiplier *= 0.5;
		}
	}

	/*
	 * Apply weighting factors and alleviate the garbage that the joysticks
	 * output when resting (the phantom values).
	 */
	FactorJoystickValues(&l_x, &l_x_raw,  left_js_null_zone, master_power_factor * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier);
	FactorJoystickValues(&l_y, &l_y_raw,  left_js_null_zone, master_power_factor * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier);
	FactorJoystickValues(&r_x, &r_x_raw, right_js_null_zone, master_power_factor * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier);
	FactorJoystickValues(&r_y, &r_y_raw, right_js_null_zone, master_power_factor * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier);

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
	 * Write the values for the roller motors.
	 */
	if(eb_values[0x0] || ea_values[0xa]) {
		leftRollers.Set(-1.0);
	} else if(eb_values[0x1] || ea_values[0xb]) {
		leftRollers.Set(1.0);
	} else {
		leftRollers.Set(0.0);
	}

	if(ea_values[0x8] || ea_values[0xa]) {
		rightRollers.Set(1.0);
	} else if(ea_values[0x9] || ea_values[0xb]) {
		rightRollers.Set(-1.0);
	} else {
		rightRollers.Set(0.0);
	}

	/*
	 * Write the values for the wings
	 */
	if(ea_values[0x6] || ea_values[0x4]) {
		leftWing.Set(-0.6);
	} else if(ea_values[0x7] || ea_values[0x5]) {
		leftWing.Set(0.6);
	} else {
		leftWing.Set(0.0);
	}

	if(ea_values[0x2] || ea_values[0x4]) {
		rightWing.Set(0.6);
	} else if(ea_values[0x3] || ea_values[0x5]) {
		rightWing.Set(-0.6);
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

	LCDPrint();
	UpdateSmartDashboard();
}

void RowdyFifteen::AutonomousInit()
{
	/*
	 * Supress warnings about not updating often enough.
	 */
	myRobot.SetSafetyEnabled(false);

	if(ds->GetDigitalIn(1)) {
		b->Printf(b->kUser_Line3, 1, "/!\\ A_1");
		b->UpdateLCD();

		Wait(0.0);

		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(1.0);

		leftWing.Set(0);
		rightWing.Set(0);
		Wait(0.5);

		leftWing.Set(-0.6);
		rightWing.Set(0.6);
		leftRollers.Set(1.0);
		rightRollers.Set(-1.0);
		Wait(1.0);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		Wait(ds->GetAnalogIn(1));

		myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		Wait(0.1);

		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(1.0);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		aSolenoid.Set(true);
		bSolenoid.Set(true);
		Wait(1.0);

		aSolenoid.Set(false);
		bSolenoid.Set(false);
		Wait(1.0);
	} else if(ds->GetDigitalIn(2)) {
		//Move the wings down
		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(0.7);

		//Stop Moving Wings and Intake Balls
		leftWing.Set(0.0);
		rightWing.Set(0.0);
		leftRollers.Set(1.0);
		rightRollers.Set(-1.0);
		Wait(0.8);

		//Stop Intake and Drive
		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		Wait(0.2);
		myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		Wait(ds->GetAnalogIn(1));

		//Stop drive and Rollers In/Out Left + Right Wing Up
		myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		Wait(0.3);

		leftRollers.Set(-0.7);
		rightRollers.Set(-1.0);
		rightWing.Set(0.8);
		Wait(0.4);

		leftRollers.Set(0.0);
		Wait(0.5);

		//Stop Rollers and Keep Moving Wings
		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		rightWing.Set(-0.8);
		Wait(0.9);

		//Stop Wing Down + FIRE!!!!
		rightWing.Set(0.0);
		leftWing.Set(0.0);
		Wait(0.2);

		aSolenoid.Set(true);
		bSolenoid.Set(true);
		Wait(0.2);

		//STOP FIRING!!!
		aSolenoid.Set(false);
		bSolenoid.Set(false);
		Wait(1.0);

		leftRollers.Set(1.0);
		rightRollers.Set(-1.0);
		Wait(0.1);

		leftWing.Set(-0.6);
		rightWing.Set(0.6);
		Wait(0.8);

		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(0.8);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		aSolenoid.Set(true);
		bSolenoid.Set(true);
		Wait(0.2);

		aSolenoid.Set(false);
		bSolenoid.Set(false);
	} else if(!ds->GetDigitalIn(1) && !ds->GetDigitalIn(2) && !ds->GetDigitalIn(3)) {
	}

	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}

void RowdyFifteen::AutonomousPeriodic()
{
	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}

void RowdyFifteen::PracticeInit()
{
	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}

void RowdyFifteen::PracticePeriodic()
{
	SetJoystickButtonValueRegisters();

	LCDPrint();
	UpdateSmartDashboard();
}

/*
 * Runs during test mode
 */
void RowdyFifteen::TestInit()
{
	SetJoystickButtonValueRegisters();

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

START_ROBOT_CLASS(RowdyFifteen);
