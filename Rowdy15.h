#include <math.h>
#include "WPILib.h"

#ifndef ROWDY15_H
#define ROWDY15_H

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
	RowdyFifteen();
	
	void UpdateSmartDashboard();
	void SetJoystickButtonValueRegister(Joystick *, vector<bool> *);
	void SetJoystickButtonValueRegisters();
	void LCDPrint();

	void DisabledInit();
	void DisabledPeriodic();
	
	void TeleopInit();
	void TeleopPeriodic();
	
	void AutonomousInit();
	void AutonomousPeriodic();

	void PracticeInit();
	void PracticePeriodic();
	
	void TestInit();
	void TestPeriodic();
};
#endif
