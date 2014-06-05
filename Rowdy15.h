#include <math.h>
#include "WPILib.h"

#ifndef ROWDY15_H
#define ROWDY15_H
class RowdyFifteen : public IterativeRobot
{
private:
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
	void RowdyFifteen();
	void UpdateDashboard();
	void SetJoystickButtonValueRegister(Joystick *, vector<bool>);
}
#endif
