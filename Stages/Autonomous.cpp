#include "../Rowdy15/Rowdy15.h"

void RowdyFifteen::AutonomousInit()
{
	/*
	 * Suppress warnings about not updating often enough.
	 */
	myRobot.SetSafetyEnabled(false);

	if(ds->GetDigitalIn(1)) {
		b->Printf(b->kUser_Line5, 1, "Running 1-ball Autonomous.");
		b->UpdateLCD();

		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(0.2);

		leftWing.Set(0);
		rightWing.Set(0);
		Wait(0.4);

		leftWing.Set(-0.6);
		rightWing.Set(0.6);
		leftRollers.Set(1.0);
		rightRollers.Set(-1.0);
		Wait(0.2);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		Wait(0.4);
		
		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		Wait(ds->GetAnalogIn(1));

		myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		Wait(0.1);

		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(0.6);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		aSolenoid.Set(true);
		bSolenoid.Set(true);
		Wait(1.0);

		aSolenoid.Set(false);
		bSolenoid.Set(false);
		Wait(1.0);
	} else if(ds->GetDigitalIn(2)) {
		b->Printf(b->kUser_Line5, 1, "Running 2-ball Autonomous.");
		b->UpdateLCD();

		//Move the wings down
		leftWing.Set(0.6);
		rightWing.Set(-0.6);
		Wait(0.7);

		//Stop Moving Wings and Intake Balls
		leftWing.Set(0.0);
		rightWing.Set(0.0);
		leftRollers.Set(1.0);
		rightRollers.Set(-1.0);
		Wait(0.9);

		//Stop Intake and Drive
		leftRollers.Set(0.0);
		rightRollers.Set(0.0);
		Wait(0.1);
		myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		Wait(ds->GetAnalogIn(1));

		//Stop drive and Rollers In/Out Left + Right Wing Up
		myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		Wait(0.3);

		leftRollers.Set(-0.7);
		rightRollers.Set(-1.0);
		rightWing.Set(0.8);
		Wait(0.7);

		leftRollers.Set(0.0);
		Wait(0.2);

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
		Wait(0.6);

		leftWing.Set(0.0);
		rightWing.Set(0.0);
		aSolenoid.Set(true);
		bSolenoid.Set(true);
		Wait(0.2);

		aSolenoid.Set(false);
		bSolenoid.Set(false);
	} else if(!ds->GetDigitalIn(1) && !ds->GetDigitalIn(2) && !ds->GetDigitalIn(3)) {
		b->Printf(b->kUser_Line5, 1, "Running no-ball Autonomous.");
		b->UpdateLCD();

		/*
		 * Only drive forward.
		 */
		myRobot.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		Wait(ds->GetAnalogIn(1));
		
		myRobot.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
	}

	SetJoystickButtonValueRegisters();

	UpdateOutputs();
}

void RowdyFifteen::AutonomousPeriodic()
{
	SetJoystickButtonValueRegisters();
	UpdateOutputs();
}
