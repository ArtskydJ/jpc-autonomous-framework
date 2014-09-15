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

//This is the file for constants that should not need to be modified.

//Autonomous motor power presets
#define FWD     127  //Drive direction
#define REV   (-127) //Drive direction
#define UP      127  //Lift
#define DOWN  (-127) //Lift
#define LEFT  (-127) //Turning
#define RIGHT   127  //Turning
#define IN    (-127) //Intake
#define OUT     127  //Intake
#define FULL    127  //Max speex
#define HALF    64   //Half of max speed
#define BRAKE   5           //Braking motor power

//Sensor values (these could be replaced with variables that are updated periodically.)
#define CURRENT_GYRO           SensorValue[GYRO]
#define CURRENT_LEFT_ENC       SensorValue[QUAD_LEFT]
#define CURRENT_RIGHT_ENC      SensorValue[QUAD_RIGHT]
#define CURRENT_LINE_FOLLOWER  SensorValue[LINE_FOLLOWER]

//Solenoid options
typedef enum {
	ON = 0,
	OFF,
	TOGGLE,
	NUM_OF_SOLENOID_OPTS
} T_SOLENOID_OPTS;

typedef enum {
	TIME_LIMIT = 0,
	DRIVE_MOTORS,
	LIFT_MOTORS,
	INTAKE_MOTORS,
	ALL_MOTORS,
	NUM_OF_END_TYPES
} T_END_TYPES;

//Function Prototypes
void applySlew(int *assign, int slewRate, int mtr, int target);
