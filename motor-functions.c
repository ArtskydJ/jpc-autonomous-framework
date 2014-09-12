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

//Modify this file with your motor names

///////////////////////////////// MODIFY BELOW /////////////////////////////////
void driveFlFrBlBr(int slewRate, int frontLeft, int frontRight, int backLeft, int backRight) {
	applySlew(slewRate, DRIVE_FL, frontLeft);
	applySlew(slewRate, DRIVE_FR, frontRight);
	applySlew(slewRate, DRIVE_BL1, backLeft);
	applySlew(slewRate, DRIVE_BL2, backLeft);
	applySlew(slewRate, DRIVE_BR1, backRight);
	applySlew(slewRate, DRIVE_BR2, backRight);
}

void liftLR(int slewRate, unsigned int both) {
	applySlew(slewRate, LIFT_L, decodeL(both));
	applySlew(slewRate, LIFT_R, decodeR(both));
}

void intakeLR(int slewRate, int target) {
	applySlew(slewRate, INTK_L, target);
	applySlew(slewRate, INTK_R, target);
}
///////////////////////////////// MODIFY ABOVE /////////////////////////////////

void applySlew(int slewRate, int mtr, int target) { //has a function prototype
	motor[mtr] = slew(target, motor[mtr], slewRate);
}

void driveLRS(int slewRate, int left, int right, int strafe) {
	driveFlFrBlBr(
		slewRate,
		left  + strafe,
		right - strafe,
		left  - strafe,
		right + strafe
	);
}

void driveFTS(int slewRate, int forward, int turn, int strafe) {
	driveFlFrBlBr(
		slewRate,
		forward + turn + strafe,
		forward + turn - strafe,
		forward - turn - strafe,
		forward - turn + strafe
	);
}
