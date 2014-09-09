#ifndef ROWDY15_H
#define ROWDY15_H

#include "WPILib.h"

#include <math.h>

#include "Math/RowdyMath.h"
#include "defines.h"
#include "enums.h"

class RowdyFifteen : public IterativeRobot
{
	RobotDrive myRobot;
	Joystick stickl;
	Joystick stickr;
	Joystick stickea;
	Joystick stickeb;
	Talon leftWing;
	Talon rightWing;
	Talon leftRollers;
	Jaguar rightRollers;
	DriverStation *ds;
	DriverStationLCD *b;
	Solenoid aSolenoid;
	Solenoid bSolenoid;
	Compressor compressor;

	bool party_mode;

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

	void FactorJoystickValues(float *result, float *raw, float null_zone, float factor);

	void SetJoystickButtonValueRegister(Joystick *, vector<bool> *);
	void SetJoystickButtonValueRegisters();

	void UpdateSmartDashboard();
	void UpdateOutputs();
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
