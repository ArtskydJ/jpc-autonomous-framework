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

//This is the file for definitions that should be modified for each robot

//Sensor values (these could be replaced with variables that are updated periodically.)
#define CURRENT_GYRO           SensorValue[GYRO]
#define CURRENT_LEFT_ENC       SensorValue[QUAD_LEFT]
#define CURRENT_RIGHT_ENC      SensorValue[QUAD_RIGHT]
#define CURRENT_LIFT_HEIGHT    SensorValue[POT_LIFT]
#define CURRENT_LINE_FOLLOWER  SensorValue[LINE_FOLLOWER]

//Proportional Constants
#define LIFT_P      (float)0.45
#define LINE_P      (float)0.022
#define WALL_P      (float)0.1
#define ENC_DRV_P   (float)0.3
#define ENC_STRF_P  (float)2.0
#define US_STRF_P   (float)8.0
#define GYRO_P      (float)0.45
#define GYRO_STRF_P (float)1.0

//Slew rates; smaller = more gradual
#define DRIVE_SLEW_RATE 5   //3/8 sec  (if LOOP_TIME_MS==15, because 15 * 127 / 5  = ~380ms = ~3/8sec)
#define LIFT_SLEW_RATE 5    //3/8 sec  (if LOOP_TIME_MS==15, because 15 * 127 / 5  = ~380ms = ~3/8sec)
#define INTAKE_SLEW_RATE 10 //3/16 sec (if LOOP_TIME_MS==15, because 15 * 127 / 10 = ~190ms = ~3/16sec)

//Other
#define LOOP_TIME_MS 15     //17 ms interval between motor updates I think
#define LINE_TARGET 2000    //Line follower target
#define FOLLOW 100          //Line following target speed
#define STRAFE              //Comment out to disable strafing
//#define H_DRIVE           //Comment out to use mecanum or x-drive instead of an h-drive

//Preset lift heights
typedef enum {
	GROUND = 430,
	LOW_POST = 1605,
	MED_POST = 2750,
	HIGH_POST = 3920
} T_PRESETS;
