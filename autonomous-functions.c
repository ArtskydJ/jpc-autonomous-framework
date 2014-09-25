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


static int thisStepNum = 0;
static int runningStepNum = 0;
static T_STATES autoStepState = NONE; //states: NONE, HIT, TIME_UP

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
		//runningStepNum++;
		//clear screen or beep or whatever you want
		//} else { //if not at the beginning
		nextStep();
	}
}

void endAuto() {
	thisStepNum++;
	if (runningStepNum == thisStepNum) {
		writeDebugStreamLine("</autonomous>");
		//clear screen or beep or whatever you want
		//run resetAuto now to loop autonomous routine
		runningStepNum++;
	}
	if (runningStepNum >= thisStepNum) {
		//whenever auton isn't running
	}
}

void auto(unsigned int driveLR, int driveStrafe, unsigned int liftLR, int intake, T_END_TYPES endType, int endTime) {
	//Not able to compute values live
	//Might need to be passed encoder values
	//Might not need the encoding stuff now
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
	return encode(0, 0);
}

unsigned int straight(int spd) { //Both sides same power
	return encode(spd, spd);
}

unsigned int powerL(int spd) { //Left side turn power
	return encode(spd, 0);
}

unsigned int powerR(int spd) { //Right side turn power
	return encode(0, spd);
}

unsigned int gyro2(int deg) { //Both sides turn gyro
	return encode(
		(deg * 10 - CURRENT_GYRO) * GYRO_P, //automatically range limited to REV through FWD
		(deg * 10 - CURRENT_GYRO) * -GYRO_P
	);
}

unsigned int enc(int dist) { //Both sides, one target, two encoders
	return encode(
		(dist - CURRENT_LEFT_ENC) * ENC_DRV_P, //automatically range limited to REV through FWD
		(dist - CURRENT_RIGHT_ENC) * ENC_DRV_P
	);
}

#if (EXTENDED_API)
	int encStrafe1(int n) { //Strafe to Left Encoder setpoint
		return (n + (CURRENT_LEFT_ENC + CURRENT_LEFT_ENC)/2) * ENC_STRF_P;
	}

	unsigned int speeds(int spdL, int spdR) { //Separate sides different power, alias: encode()
		return encode(spdL, spdR);
	}

	unsigned int gyroL(int deg) { //Left side turn gyro degrees
		return encode((deg*10 - CURRENT_GYRO)*GYRO_P*2, 0); //x2 b/c one side's not moving
	}

	unsigned int gyroR(int deg) { //Right side turn gyro
		return encode(0, (deg*10 - CURRENT_GYRO)*GYRO_P*-2); //x2 b/c one side's not moving
	}

	unsigned int enc2(int distL, int distR) { //Individual sides encoders
		return encode(
			(distL - CURRENT_LEFT_ENC) * ENC_DRV_P, //automatically range limited to REV through FWD
			(distR - CURRENT_RIGHT_ENC) * ENC_DRV_P
		);
	}

	unsigned int enc1Spd(int dist, int spd) { //Both sides, one target, two encoders, custom speed
		return encode(
			rangeLimit(-abs(spd), (dist-CURRENT_LEFT_ENC) * ENC_DRV_P, abs(spd)),
			rangeLimit(-abs(spd), (dist-CURRENT_RIGHT_ENC) * ENC_DRV_P, abs(spd))
		);
	}
#endif




/*
- add lift functions
	- preset heights
	- preset heights +/-
- create api, documentation
*/
