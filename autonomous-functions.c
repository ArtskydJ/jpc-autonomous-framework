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

//Variables and stuff
typedef enum {
	NONE = 0,
	HIT,
	TIME_UP,
	NUM_OF_STATES
} T_STATES;

static int thisStepNum = 0;
static int runningStepNum = 0;
static T_STATES autoStepState = NONE; //states: NONE, HIT, TIME_UP


//Functions (obviously)
void resetAuto() {
	writeDebugStreamLine("<autonomous>");
	thisStepNum = 0;
	runningStepNum = 0;
	autoStepState = NONE;
	TIMER_STEP = 0;
	TIMER_END_STEP = 0;
}

void nextStep() {
	writeDebugStreamLine("Step %d | %1.1f sec", runningStepNum, (float)TIMER_STEP/1000);
	runningStepNum++;
	autoStepState = NONE;
	TIMER_STEP = 0;
	TIMER_END_STEP = 0;
}

void startAuto() {
	thisStepNum = 0; //ALWAYS SET THIS TO 0!
	if (runningStepNum == 0) { //if at the beginning
		resetAuto();
		//clear screen or beep or whatever
		nextStep();
	}
}

bool endAuto() {
	thisStepNum++;
	if (runningStepNum == thisStepNum) {
		writeDebugStreamLine("</autonomous>");
		//clear screen or beep or whatever you want
		//running resetAuto() now would loop the autonomous routine
	}
	return (runningStepNum < thisStepNum);
}

void auto(unsigned int driveLR, int driveStrafe, unsigned int liftLR, int intake, T_END_TYPES endType, int endTime) {
	thisStepNum++;
	if (runningStepNum == thisStepNum) {
		int driveLeft = decodeL(driveLR);
		int driveRight = decodeR(driveLR);
		driveLeftRightStrafe(DRIVE_SLEW_RATE, driveLeft, driveRight, driveStrafe);
		liftSpeeds(LIFT_SLEW_RATE, liftLR);
		intakeSpeed(INTAKE_SLEW_RATE, intake);

		if (autoStepState == NONE) {
			bool driveDone = false;
			bool liftDone = false;
			bool intakeDone = false;
			if (abs(driveLeft) < MOTOR_ALLOW_ZONE
				&& abs(driveRight) < MOTOR_ALLOW_ZONE
				&& abs(driveStrafe) < MOTOR_ALLOW_ZONE) driveDone = true;
			if (abs(decodeL(liftLR)) < MOTOR_ALLOW_ZONE
				&& abs(decodeR(liftLR)) < MOTOR_ALLOW_ZONE) liftDone = true;
			if (abs(intake) < MOTOR_ALLOW_ZONE) intakeDone = true;

			switch(endType) { //check for condition being hit
				case TIME_LIMIT:    autoStepState = (TIMER_STEP >= endTime)? TIME_UP : NONE; break;
				case DRIVE_MOTORS:  autoStepState = driveDone? HIT : NONE; break;
				case LIFT_MOTORS:   autoStepState = liftDone? HIT : NONE; break;
				case INTAKE_MOTORS: autoStepState = intakeDone? HIT : NONE; break;
				case ALL_MOTORS:    autoStepState = (driveDone && liftDone && intakeDone)? HIT : NONE; break;
			}
			if (autoStepState == HIT) { //if just hit condition, not including TIME_LIMIT
				writeDebugStreamLine("BL mtr speed: %d", motor[DRIVE_BL1]);
			}
			TIMER_END_STEP = 0;
		}
		if (autoStepState == HIT) { //if condition was hit sometime
			autoStepState = (TIMER_END_STEP >= endTime)? TIME_UP : HIT;
		}
		if (autoStepState == TIME_UP) { //if done with step
			nextStep();
		}
	}
}

void solenoid(int sensor, T_SOLENOID_OPTS targetState) {
	if (targetState == OFF || targetState == ON) {
		SensorValue[sensor] = (bool)targetState;
	} else if (targetState == TOGGLE) {
		SensorValue[sensor] = !SensorValue[sensor];
	}
}


//Yes, I copy & pasted the next 60ish lines from technapwn-toss-up (which I created)

unsigned int stopped() { //Don't move forward or reverse
	return encode2(0);
}

unsigned int straight(int spd) { //Both sides same power; AKA encode2()
	return encode2(spd);
}

unsigned int turn(int spd) { //Both sides same power; AKA encodeNegative2()
	return encodeNegative2(spd);
}

unsigned int speeds(int spdL, int spdR) {
	return encode(spdL, spdR); //Sides have separate speeds; AKA encode()
}

unsigned int speedL(int spd) { //Left side turn speed; AKA encodeL()
	return encodeL(spd);
}

unsigned int speedR(int spd) { //Right side turn speed; AKA encodeR()
	return encodeR(spd);
}

unsigned int gyro2(int deg) { //Both sides turn gyro
	return encodeNegative2(
		(deg * 10 - CURRENT_GYRO) * GYRO_P //auto range limited to REV thru FWD
	);
}

unsigned int enc(int dist) { //Both sides, one target, two encoders
	return encode(
		(dist - CURRENT_LEFT_ENC) * ENC_DRV_P, //automatically range limited...
		(dist - CURRENT_RIGHT_ENC) * ENC_DRV_P //...to REV through FWD
	);
}

//Advanced API below...
int encStrafe1(int n) { //Strafe to Left Encoder setpoint
	return (n + (CURRENT_LEFT_ENC + CURRENT_LEFT_ENC) / 2) * ENC_STRF_P;
}

int encStrafe2(int n) { //Strafe to Left Encoder setpoint
	return (n + (CURRENT_LEFT_ENC - CURRENT_RIGHT_ENC) / 2) * ENC_STRF_P;
} //Subtract b/c they're going opposite dirs

unsigned int gyroL(int deg) { //Left side turn gyro degrees
	return encodeL(
		(deg*10 - CURRENT_GYRO) * GYRO_P * 2 //x2 b/c one side isn't movin
	);
}

unsigned int gyroR(int deg) { //Right side turn gyro
	return encodeR(
		(deg*10 - CURRENT_GYRO) * GYRO_P * -2 //x2 b/c one side isn't movin
	);
}

unsigned int enc2(int distL, int distR) { //Individual sides encoders
	return encode(
		(distL - CURRENT_LEFT_ENC) * ENC_DRV_P, //automatically range limited to REV through FWD
		(distR - CURRENT_RIGHT_ENC) * ENC_DRV_P
	);
}

unsigned int enc1Spd(int dist, int spd) { //2 sides, 1 dist, 2 encs, custom spd
	return encode(
		rangeLimit(-abs(spd), (dist - CURRENT_LEFT_ENC) * ENC_DRV_P, abs(spd)),
		rangeLimit(-abs(spd), (dist - CURRENT_RIGHT_ENC) * ENC_DRV_P, abs(spd))
	);
}

//LIFTING
unsigned int liftPreset(T_PRESETS height) {
	return encode2(
		(height - CURRENT_LIFT_HEIGHT) * LIFT_P
	);
}
