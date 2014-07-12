#include "Rowdy15.h"

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

