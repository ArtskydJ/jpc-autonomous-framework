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
void driveFlFrBlBr(int *assign, int slewRate, int frontLeft, int frontRight, int backLeft, int backRight) {
	applySlew(assign, slewRate, DRIVE_FL, frontLeft);
	applySlew(assign, slewRate, DRIVE_FR, frontRight);
	applySlew(assign, slewRate, DRIVE_BL1, backLeft);
	applySlew(assign, slewRate, DRIVE_BL2, backLeft);
	applySlew(assign, slewRate, DRIVE_BR1, backRight);
	applySlew(assign, slewRate, DRIVE_BR2, backRight);
}

void liftLR(int *assign, int slewRate, unsigned int both) {
	applySlew(assign, slewRate, LIFT_L, decodeL(both));
	applySlew(assign, slewRate, LIFT_R, decodeR(both));
}

void intakeLR(int *assign, int slewRate, int target) {
	applySlew(assign, slewRate, INTK_L, target);
	applySlew(assign, slewRate, INTK_R, target);
}
///////////////////////////////// MODIFY ABOVE /////////////////////////////////

void applySlew(int *assign, int slewRate, int mtr, int target) { //has a function prototype
	*assign[mtr] = slew(target, *assign[wheel], slewRate);
}

void driveLRS(int *assign, int slewRate, int left, int right, int strafe) {
	driveFlFrBlBr(
		assign,
		slewRate,
		left  + strafe,
		right - strafe,
		left  - strafe,
		right + strafe
	);
}

void driveFTS(int *assign, int slewRate, int forward, int turn, int strafe) {
	driveFlFrBlBr(
		assign,
		slewRate,
		forward + turn + strafe,
		forward + turn - strafe,
		forward - turn - strafe,
		forward - turn + strafe
	);
}
