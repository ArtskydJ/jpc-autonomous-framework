//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       VEX Competition Control Include File
//
// This file provides control over a VEX Competition Match. It should be included in the user's
// program with the following line located near the start of the user's program
//        #include "VEX_Competition_Includes.h"
// The above statement will cause this program to be included in the user's program. There's no
// need to modify this program.
//
// The program displays status information on the new VEX LCD about the competition state. You don't
// need the LCD, the program will work fine whether or not the LCD is actually provisioned.
//
// The status information is still useful without the LCD. The ROBOTC IDE debugger has a "remote screen"
// that contains a copy of the status information on the LCD. YOu can use this to get a view of the
// status of your program. The remote screen is shown with the menu command
//   "Robot -> Debugger Windows -> VEX Remote Screen"
//
// The LCD is 2 lines x 16 characters. There are three display formats to look for:
//
//        State          Description
//
//    ----------------
//   |Disabled        |  The robot is idle. This occurs before both the autonomous and user
//   |Time mm:ss.s    |  control states. The time display is minutes and seconds it has been idle.
//    ----------------
//
//    ----------------
//   |Autonomous      |  The robot is running autonomous code.
//   |Time mm:ss.s    |  control states. The time display is minutes and seconds it has been running.
//    ----------------
//
//    ----------------
//   |User Control    |  The robot is running user control code.
//   |Time mm:ss.s    |  control states. The time display is minutes and seconds it has been running.
//    ----------------
//////////////////////////////////////////////////////////////////////////////////////////////////////


void allMotorsOff();
void allTasksStop();
void pre_auton();
task autonomous();
task usercontrol();

int nTimeXX = 0;
bool bStopTasksBetweenModes = true;

static void displayStatusAndTime();

task main()
{
	// Master CPU will not let competition start until powered on for at least 2-seconds
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDPos(0, 0);
	displayNextLCDString("Startup");
	wait1Msec(2000);


	pre_auton();

	//wait1Msec(500);


	while (true)
	{

		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDPos(0, 0);

		while (bIfiRobotDisabled)
		{
			displayLCDPos(0, 0);
			displayNextLCDString("Disabled");
			nTimeXX = 0;
			while (true)
			{
				displayStatusAndTime();
				if (!bIfiRobotDisabled)
					break;
				wait1Msec(25);

				displayStatusAndTime();
				if (!bIfiRobotDisabled)
					break;
				wait1Msec(25);

				displayStatusAndTime();
				if (!bIfiRobotDisabled)
					break;
				wait1Msec(25);

				displayStatusAndTime();
				if (!bIfiRobotDisabled)
					break;
				wait1Msec(25);
				++nTimeXX;
			}
		}

		nTimeXX = 0;
		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDPos(0, 0);
		if (bIfiAutonomousMode)
		{
			displayNextLCDString("Autonomous");
			StartTask(autonomous);

			// Waiting for autonomous phase to end
			while (bIfiAutonomousMode && !bIfiRobotDisabled)
			{
				if (!bVEXNETActive)
				{
					if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
						allMotorsOff();
				}
				wait1Msec(25);                             // Waiting for autonomous phase to end
			}
			allMotorsOff();
			if(bStopTasksBetweenModes)
			{
				allTasksStop();
			}
		}
		else
		{
			displayNextLCDString("User Control");
			StartTask(usercontrol);

			// Here we repeat loop waiting for user control to end and (optionally) start
			// of a new competition run
			while (!bIfiAutonomousMode && !bIfiRobotDisabled)
			{
				if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
					allMotorsOff();
				wait1Msec(25);
			}
			allMotorsOff();
			if(bStopTasksBetweenModes)
			{
				allTasksStop();
			}
		}
	}
}


void allMotorsOff()
{
	motor[port1] = 0;
	motor[port2] = 0;
	motor[port3] = 0;
	motor[port4] = 0;
	motor[port5] = 0;
	motor[port6] = 0;
	motor[port7] = 0;
	motor[port8] = 0;
#if defined(VEX2)
	motor[port9] = 0;
	motor[port10] = 0;
#endif
}

void allTasksStop()
{
	StopTask(1);
	StopTask(2);
	StopTask(3);
	StopTask(4);
#if defined(VEX2)
	StopTask(5);
	StopTask(6);
	StopTask(7);
	StopTask(8);
	StopTask(9);
	StopTask(10);
	StopTask(11);
	StopTask(12);
	StopTask(13);
	StopTask(14);
	StopTask(15);
	StopTask(16);
	StopTask(17);
	StopTask(18);
	StopTask(19);
#endif
}

static void displayStatusAndTime()
{
	displayLCDPos(1, 0);
	if (bIfiRobotDisabled)
		displayNextLCDString("Disable ");
	else
	{
		if (bIfiAutonomousMode)
			displayNextLCDString("Auton  ");
		else
			displayNextLCDString("Driver ");
	}
	displayNextLCDNumber(nTimeXX / 600, 2);
	displayNextLCDChar(':');
	displayNextLCDNumber((nTimeXX / 10) % 60, -2);
	displayNextLCDChar('.');
	displayNextLCDNumber(nTimeXX % 10, 1);
}


static void UserControlCodePlaceholderForTesting()
{
	// Following code is simply for initial debuggging.
	//
	// It can be safely removed in a real program	and removing it will slightly improve the
	// real-time performance of your robot.
	//
	displayStatusAndTime();
	wait1Msec(100);
	++nTimeXX;
}

static void AutonomousCodePlaceholderForTesting()
{
	// This is where you insert your autonomous code. Because we don't have any, we'll
	// simply display a running count of the time on the VEX LCD.

	while (true)
	{
		displayStatusAndTime();
		wait1Msec(100);
		++nTimeXX;
	}
}
