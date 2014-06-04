#include <math.h>
#include "WPILib.h"

enum RobotDriveTrainPorts {
	dt_pwm_rear_right = 1,
	dt_pwm_rear_left = 2,
	dt_pwm_front_right = 3,
	dt_pwm_front_left = 4
};

enum RobotWingPorts {
	w_pwm_roller_left = 5,
	w_pwm_roller_right = 6,
	w_pwm_wing_left = 9,
	w_pwm_wing_right = 10
};

enum RobotMiscellaneousPorts {
	m_rel_aim_confirm_led = 2
};

enum RobotPneumaticsPorts {
	p_sol_shooter_a = 7,
	p_sol_shooter_b = 8,
	p_rel_compressor = 1
};

enum RobotJoystickPorts {
	j_1 = 1,
	j_2 = 2,
	j_3 = 3,
	j_4 = 4,
	j_left = j_1,
	j_right = j_2
};

enum Attack3Buttons {
	atk3_btn_1 = 0x1,
	atk3_btn_2 = 0x2,
	atk3_btn_3 = 0x3,
	atk3_btn_4 = 0x4,
	atk3_btn_5 = 0x5,
	atk3_btn_6 = 0x6,
	atk3_btn_7 = 0x7,
	atk3_btn_8 = 0x8,
	atk3_btn_9 = 0x9,
	atk3_btn_10 = 0xa,
	atk3_btn_11 = 0xb,
	atk3_btn_12 = 0xc,
	atk3_btn_trigger = atk3_btn_1,
	atk3_btn_thumb_back = atk3_btn_2,
	atk3_btn_thumb_front = atk3_btn_3,
	atk3_btn_thumb_left = atk3_btn_4,
	atk3_btn_thumb_right = atk3_btn_5
};

#define master_js_null_zone 0.06
#define left_js_null_zone master_js_null_zone
#define right_js_null_zone master_js_null_zone

#define master_power_factor 0.8
#define master_trigger_power_factor 0.25

#define dual_triggers_multiply_again false

#define use_mecanum true

class RowdyFifteen : public IterativeRobot
{
	RobotDrive myRobot;
	Joystick stickl;
	Joystick stickr;
	Joystick stickea;
	Joystick stickeb;
	Talon leftWing;
	Talon rightWing;
	Jaguar leftRollers;
	Jaguar rightRollers;
	DriverStation *ds;
	DriverStationLCD *b;
	Solenoid aSolenoid;
	Solenoid bSolenoid;
	Compressor compressor;

	vector<bool> l_values;
	vector<bool> r_values;
	vector<bool> ea_values;
	vector<bool> eb_values;
	float l_x_raw;
	float l_y_raw;
	float r_x_raw;
	float r_y_raw;
	float l_x;
	float l_y;
	float r_x;
	float r_y;
	float missile_switch_speed_multiplier;

	float drive_speed_ain_value;

public:
	RowdyFifteen():
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

		compressor.Start();

		SmartDashboard::init();
	}

	vector<bool> GetJoystickButtonValues(Joystick *joystick)
	{
		vector<bool> values;

		for(uint32_t i = 0; i < 12; i += 1) {
			values.push_back(joystick->GetRawButton(i + 1));
		}

		return values;
	}

	void SetJoystickButtonValueRegister(Joystick *joystick, vector<bool> *value_registry)
	{
		value_registry->clear();

		for(uint32_t i = 0; i < 12; i += 1) {
			value_registry->push_back(joystick->GetRawButton(i + 1));
		}
	}

	void UpdateSmartDashboard()
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
	 * Drive left & right motors for 2 seconds then stop
	 */
	void AutonomousInit()
	{
		/*
		 * Supress warnings about not updating often enough.
		 */
		myRobot.SetSafetyEnabled(false);

		b->Clear();
		b->Printf(b->kUser_Line1, 1, "Starting autonomous (init)!");
		b->Printf(b->kUser_Line2, 1, "%f loops/sec.", GetLoopsPerSec());
		b->UpdateLCD();

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
			Wait(1.0);
			
			//Stop Intake and Drive
			leftRollers.Set(0.0);
			rightRollers.Set(0.0);
			Wait(0.2);
			myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
			Wait(ds->GetAnalogIn(1));
			
			//Stop drive and Rollers In/Out Left + Right Wing Up
			myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
			Wait(0.1);
			leftRollers.Set(-0.1);
			rightRollers.Set(-1.0);
			rightWing.Set(1.0);
			leftWing.Set(-0.4);
			Wait(0.7);
			
			//Stop Rollers and Keep Moving Wings
			leftRollers.Set(0.0);
			rightRollers.Set(0.0);
			rightWing.Set(-1.0);
			leftWing.Set(0.4);
			Wait(0.6);
			
			//Stop Wing Down + FIRE!!!!
			rightWing.Set(0.0);
			leftWing.Set(0.0);
			Wait(0.2);
			
			aSolenoid.Set(true);
			bSolenoid.Set(true);
			Wait (1.0);
			
			
			//STOP FIRING!!!
			aSolenoid.Set(false);
			bSolenoid.Set(false);
			Wait(1.0);
			
			b->Printf(b->kUser_Line3, 1, "/!\\ A_2");
			b->Printf(b->kUser_Line4, 1, "Aw hell naw.");
			b->UpdateLCD();

			Wait(3.0);
		} else if(!ds->GetDigitalIn(1) && !ds->GetDigitalIn(2) && !ds->GetDigitalIn(3)) {
			b->Printf(b->kUser_Line3, 1, "/!\\ !A_*");
			b->Printf(b->kUser_Line4, 1, "Aw hell naw.");
			b->UpdateLCD();

			Wait(3.0);
		}

		b->Clear();
		b->Printf(b->kUser_Line1, 1, "Done with autonomous (init)!");
		b->Printf(b->kUser_Line2, 1, "%f loops/sec.", GetLoopsPerSec());
		b->UpdateLCD();
	}

	void AutonomousPeriodic()
	{
		UpdateSmartDashboard();
	}

	void TeleopInit()
	{
		myRobot.SetSafetyEnabled(false);
	}

	/*
	 * Runs the motors with mecanum.
	 *
	 * NOTE: Broken
	 */
	void TeleopPeriodic()
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
		l_x = master_power_factor * (((l_x_raw >  left_js_null_zone) || (l_x_raw < - left_js_null_zone)) ? l_x_raw : 0.0) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
		l_y = master_power_factor * (((l_y_raw >  left_js_null_zone) || (l_y_raw < - left_js_null_zone)) ? l_y_raw : 0.0) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
		r_x = master_power_factor * (((r_x_raw > right_js_null_zone) || (r_x_raw < -right_js_null_zone)) ? r_x_raw : 0.0) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;
		r_y = master_power_factor * (((r_y_raw > right_js_null_zone) || (r_y_raw < -right_js_null_zone)) ? r_y_raw : 0.0) * (drive_speed_ain_value / 5.0) * missile_switch_speed_multiplier;

		/*l_values = GetJoystickButtonValues(&stickl);
		r_values = GetJoystickButtonValues(&stickr);
		ea_values = GetJoystickButtonValues(&stickea);
		eb_values = GetJoystickButtonValues(&stickeb);*/

		SetJoystickButtonValueRegister( &stickl,  &l_values);
		SetJoystickButtonValueRegister( &stickr,  &r_values);
		SetJoystickButtonValueRegister(&stickea, &ea_values);
		SetJoystickButtonValueRegister(&stickeb, &eb_values);

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

		{
			/*
			 * Print out the Joystick values onto the User Messages screen.
			 */
			b->Clear();
			b->Printf(b->kUser_Line5, 1, "a %s%s%s%s%s%s%s%s%s%s%s%s",
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
			b->Printf(b->kUser_Line6, 1, "b %s%s%s%s%s%s%s%s%s%s%s%s",
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
		
		if(ea_values[0xb]) {
			rightRollers.Set(1.0);
			leftRollers.Set(-1.0);
		} else if(ea_values[0xa]) {
			rightRollers.Set(-1.0);
			leftRollers.Set(1.0);
		} else {
			rightRollers.Set(0.0);
			leftRollers.Set(0.0);
		}

		/*
		 * Write the values for the wings
		 */
		
//		0xa is in 0xb is out

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

		UpdateSmartDashboard();

		/*
		 * Input the values into the drive function.
		 */
		if(use_mecanum) {
			myRobot.MecanumDrive_Cartesian(l_x, l_y, r_x);
		} else {
			myRobot.TankDrive(-l_y, -r_y, false);
		}
	}

	/*
	 * Runs during test mode
	 */
	void TestInit()
	{
		/*
		 * Print out a cute little message to let people know that we're in test mode.
		 */
		b->Clear();
		b->Printf(b->kUser_Line1, 1, "TEST mode!");
		b->UpdateLCD();
	}

	void TestPeriodic()
	{
		UpdateSmartDashboard();
	}

	void DisabledInit()
	{
	}

	void DisabledPeriodic()
	{
		SetJoystickButtonValueRegister( &stickl,  &l_values);
		SetJoystickButtonValueRegister( &stickr,  &r_values);
		SetJoystickButtonValueRegister(&stickea, &ea_values);
		SetJoystickButtonValueRegister(&stickeb, &eb_values);

		b->Clear();
		b->Printf(b->kUser_Line1, 1, "Robot is disabled!");
		b->Printf(b->kUser_Line5, 1, "a %s%s%s%s%s%s%s%s%s%s%s%s",
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
		b->Printf(b->kUser_Line6, 1, "b %s%s%s%s%s%s%s%s%s%s%s%s",
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

		UpdateSmartDashboard();

		Wait(0.05);
	}
};

START_ROBOT_CLASS(RowdyFifteen);
