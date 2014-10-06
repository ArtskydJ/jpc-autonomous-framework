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

//Modify this file with your motor names.

void applySlew(int slewRate, int mtr, int target) { //has a prototype, so it can
	motor[mtr] = slew(target, motor[mtr], slewRate); //be declared after it's used
}

///////////////////////////////// MODIFY BELOW /////////////////////////////////
//Drive: Front left, Front right, Back left, Back right
void driveFlFrBlBr(int slewRate, int frontLeft, int frontRight, int backLeft, int backRight) {
	applySlew(slewRate, DRIVE_FL, frontLeft); //MODIFY HERE...
	applySlew(slewRate, DRIVE_FR, frontRight);
	applySlew(slewRate, DRIVE_BL1, backLeft); //Instead of DRIVE_BL1, etc; put in your own motor names
	applySlew(slewRate, DRIVE_BL2, backLeft); //If you have a 4 motor drive, delete this line and the line below this
	applySlew(slewRate, DRIVE_BR1, backRight);
	applySlew(slewRate, DRIVE_BR2, backRight); //...THROUGH HERE
}
#ifdef STRAFE
	#ifdef H_DRIVE
		void driveHDrive(int slewRate, int left, int right, int strafe) {
			applySlew(slewRate, DRIVE_L1, left); //MODIFY HERE...
			applySlew(slewRate, DRIVE_L2, left);
			applySlew(slewRate, DRIVE_R1, right); //Instead of DRIVE_R1, etc; put in your own motor names
			applySlew(slewRate, DRIVE_R2, right);
			applySlew(slewRate, DRIVE_S1, strafe);
			applySlew(slewRate, DRIVE_S2, strafe); //...THROUGH HERE
		}
	#endif
#endif

//Lift: Left, Right
void liftSpeeds(int slewRate, unsigned int both) {
	applySlew(slewRate, LIFT_L, decodeL(both)); //MODIFY LIFT_L...
	applySlew(slewRate, LIFT_R, decodeR(both)); //...AND LIFT_R
}

//Intake
void intakeSpeed(int slewRate, int target) {
	applySlew(slewRate, INTK_L, target); //MODIFY INKT_L...
	applySlew(slewRate, INTK_R, target); //...AND INTK_R
}
///////////////////////////////// MODIFY ABOVE /////////////////////////////////

void driveLeftRightStrafe(int slewRate, int left, int right, int strafe=0) {
#ifdef STRAFE
	#ifdef H_DRIVE
		driveHDrive(slewRate, left, right, strafe); //H-Drive
	#else
		driveFlFrBlBr( //Mecanum or X-Drive
			slewRate,
			left  + strafe,
			right - strafe,
			left  - strafe,
			right + strafe
		);
	#endif
#else
	driveFlFrBlBr( //Tank drive
		slewRate,
		left,
		right,
		left,
		right
	);
#endif
}

void driveForwardTurnStrafe(int slewRate, int forward, int turn, int strafe) {
#ifdef STRAFE
	#ifdef H_DRIVE
		driveHDrive( //H-Drive
			slewRate,
			forward + turn,
			forward - turn,
			strafe
		);
	#else
		driveFlFrBlBr( //Mecanum or X-Drive
			slewRate,
			forward + turn + strafe,
			forward + turn - strafe,
			forward - turn - strafe,
			forward - turn + strafe
		);
	#endif
#else
	driveFlFrBlBr( //Tank drive
		slewRate,
		forward + turn,
		forward - turn,
		forward + turn,
		forward - turn,
	);
#endif
}
