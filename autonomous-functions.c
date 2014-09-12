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

void auto(unsigned int driveLR, int driveStrafe, unsigned int liftLR, int intake) {
	int driveLeft = decodeL(driveLR);
	int driveRight = decodeR(driveLR);
	driveLRS(DRIVE_SLEW_RATE, driveLeft, driveRight, driveStrafe);
	liftLR(LIFT_SLEW_RATE, liftLR);
	intakeLR(INTAKE_SLEW_RATE, intake);
}

void solenoid(int sensor, T_SOLENOID_OPTS targetState) {
	if (targetState == OFF || targetState == ON) {
		SensorValue[sensor] = (bool)targetState;
	} else if (targetState == TOGGLE) {
		SensorValue[sensor] = !SensorValue[sensor];
	}
}





/*
SETTINGS
rate limiter (slew)
max time duration, toggle (0 for off?)
min time duration, toggle (0 for off?)
motors-to-outputs or whatever
mecanum toggle?


DRIVE
encoder distance straight
speed straight
gyro turn


LIFT
speed
preset heights
preset heights +/-


INTAKE
speed


END STEP
time
slow motors

*/
