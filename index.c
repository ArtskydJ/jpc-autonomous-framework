#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1, POT_LIFT, sensorPotentiometer)
#pragma config(Sensor, in2, GYRO, sensorGyro)
#pragma config(Sensor, in3, POT_SELECTOR, sensorPotentiometer)
#pragma config(Sensor, in4, LINE_FOLLOWER, sensorPotentiometer)
#pragma config(Sensor, dgtl1, QUAD_LEFT, sensorQuadEncoder)
#pragma config(Sensor, dgtl3, ULTRA_RIGHT, sensorSONAR_cm)
#pragma config(Sensor, dgtl5, ULTRA_LEFT, sensorSONAR_cm)
#pragma config(Sensor, dgtl7, QUAD_RIGHT, sensorQuadEncoder)
#pragma config(Sensor, dgtl9, BRAKE, sensorDigitalOut)
#pragma config(Sensor, dgtl10, LOADER, sensorDigitalOut)
#pragma config(Sensor, dgtl11, TRANNY, sensorDigitalOut)
#pragma config(Sensor, dgtl12, CATAPULT, sensorDigitalOut)
#pragma config(Motor,  port1, DRIVE_BL1, tmotorVex393, openLoop)
#pragma config(Motor,  port2, DRIVE_BR2, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port3, LIFT_R, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port4, DRIVE_FR, tmotorVex393, openLoop)
#pragma config(Motor,  port5, INTK_L, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port6, DRIVE_FL, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port7, INTK_R, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port8, LIFT_L, tmotorVex393, openLoop)
#pragma config(Motor,  port9, DRIVE_BL2, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port10, DRIVE_BR1, tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard !!*//

/* * * * * * * * * * * * * * * *  The MIT License  * * * * * * * * * * * * * * * *\
|* Copyright (c) 2014, Joseph Dykstra                                            *|
|*                                                                               *|
|* Permission is hereby granted, free of charge,  to any person obtaining a copy *|
|* of this software and associated documentation files (the "Software"), to deal *|
|* in the Software without restriction,  including without limitation the rights *|
|* to use,  copy, modify,  merge, publish,  distribute, sublicense,  and/or sell *|
|* copies  of the  Software, and  to permit  persons  to  whom  the  Software is *|
|* furnished to do so, subject to the following conditions:                      *|
|* The above copyright  notice and this  permission notice shall  be included in *|
|* all copies or substantial portions of the Software.                           *|
|*                                                                               *|
|* THE SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR *|
|* IMPLIED,  INCLUDING BUT  NOT LIMITED  TO THE  WARRANTIES OF  MERCHANTABILITY, *|
|* FITNESS FOR A PARTICULAR  PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE *|
|* AUTHORS  OR  COPYRIGHT HOLDERS BE  LIABLE  FOR ANY  CLAIM,  DAMAGES OR  OTHER *|
|* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *|
|* OUT OF OR IN  CONNECTION WITH  THE SOFTWARE  OR THE USE OR  OTHER DEALINGS IN *|
|* THE SOFTWARE.                                                                 *|
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
|* This project is to give those who use the VEX competition template an easier, *|
|* more powerful, and cleaner way to write autonomous routines.                  *|
|*                                                                               *|
|* Diagram:                                                                      *|
|*                                   main()                                      *|
|*                            _________|__________                               *|
|*                           V                   |                               *|
|*                      autonomous()             V                               *|
|*                           V              usercontrol()                        *|
|*                         auto()                V                               *|
|*                           V               driveFTS()                          *|
|*                       driveLRS()              |                               *|
|*                           |_________ _________|                               *|
|*                                     |                                         *|
|*                                     V                                         *|
|*                               driveFlFrBlBr()                                 *|
|*                                     V                                         *|
|*                                 applySlew()                                   *|
|*                                     V                                         *|
|*                                   motor[]                                     *|
|*                                                                               *|
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//Competition Control and Duration Settings
#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)
#if(0) //change this to 1
	#include "Vex_Competition_Includes.c"
#else
	#include "run-auto.c"
#endif

//JPC Autonomous Framework
#include "definitions.c"
#include "config.c"
#include "utility-functions.c"
#include "motor-functions.c"
#include "autonomous-functions.c"

void pre_auton() {
	bLCDBacklight = true;
}

task autonomous() {
	bool runningAuto = true;
	while (runningAuto) { //Weird as it looks; this loop must exist for the autonomous to work.
		startAuto();
		//___Drive_L&R________Strafe___Lift_L&R______Intake___End_Type_____Time_____Notes
		auto(straight(FWD),   0,       stopped(),    FWD,     TIME_LIMIT,  1000); //Forward
		auto(straight(REV),   0,       stopped(),    0,       TIME_LIMIT,  1000); //Reversed
		auto(stopped(),       LEFT,    straight(60), 0,       TIME_LIMIT,  1000); //Turn left with left wheels
		auto(turn(-64),      0,       stopped(),    0,       TIME_LIMIT,  1000); //Zero-turn half speed
		auto(straight(BRAKE), 0,       stopped(),    0,       TIME_LIMIT,  200);  //Brake
		auto(straight(-BRAKE),0,       stopped(),    0,       TIME_LIMIT,  200);  //Brake
		runningAuto = endAuto();
		constantLoopTime(); //Important for slew
	}
}

task usercontrol() {
	while (true) {
		driveForwardTurnStrafe(DRIVE_SLEW_RATE, vexRT(Ch3), vexRT(Ch4), vexRT(Ch1));
		liftSpeeds(LIFT_SLEW_RATE, straight( buttonsToSpeed(Btn5U, Btn5D) ) );
		intakeSpeed(INTAKE_SLEW_RATE, buttonsToSpeed(Btn6U, Btn6D) );
		constantLoopTime(); //Important for slew
	}
}
