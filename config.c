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

//This is the file for definitions that should be modified for performance

#define FOLLOW  100         //Line following target speed
#define GYRO_P  2           //Gyro Proportional
#define LINE_P  1           //Line Follower Proportional
#define ENC_STRF_P 1        //Encoder Strafe Proportional
#define ENC_DRV_P 1         //Encoder Drive Proportional
#define LOOP_TIME_MS 15     //17 ms interval between motor updates I think
#define LINE_TARGET 2000    //Line follower target

//Slew rates; smaller = more gradual
#define DRIVE_SLEW_RATE 5   //3/8 sec  (if LOOP_TIME_MS==15, because 15 * 127 / 5  = ~380ms = ~3/8sec)
#define LIFT_SLEW_RATE 5    //3/8 sec  (if LOOP_TIME_MS==15, because 15 * 127 / 5  = ~380ms = ~3/8sec)
#define INTAKE_SLEW_RATE 10 //3/16 sec (if LOOP_TIME_MS==15, because 15 * 127 / 10 = ~190ms = ~3/16sec)
