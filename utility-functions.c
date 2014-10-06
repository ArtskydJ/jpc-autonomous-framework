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


//This function returns the value between min and max.
int rangeLimit(int min, int value, int max) {
	if (min > max) { //https://en.wikipedia.org/wiki/XOR_swap_algorithm
		min = min ^ max;
		max = max ^ min;
		min = min ^ max;
	}
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

//This function keeps the main loop time of the code constant. It does this by
//waiting after each loop iteration until x milliseconds have passed since it
//started the loop iteration. (E.g. Start loop, do stuff, wait, repeat.)
void constantLoopTime() {
	while (TIMER_LOOP < LOOP_TIME_MS) {
		//noop
	}
	TIMER_LOOP = 0;
}

//This function returns a corrected value for potentiometers that were installed backwards.
int potReverse(int potentiometer) {
	return rangeLimit(0, 4095 - potentiometer, 4095);
}

//This function checks how far the dial is turned compared to how many options
//number allowed. E.g. potPosition(100) returns a number between 0 and 99,
//proportional to how far the dial is turned.
int potPosition(int numOfOptions, int potentiometer) {
	numOfOptions =  rangeLimit(0, numOfOptions, 4095);
	potentiometer =  rangeLimit(0, potentiometer, 4095);
	return rangeLimit(0, (float)numOfOptions*potentiometer/4096, numOfOptions-1);
}
//This function calcs a new speed with target, previous, and slew values. Examples:
//slew(100, 0, 5) -> 5
//slew(100, 15, 5) -> 20
//slew(-100, -80, 5) -> -85
int slew(int target, int now, int rate) {
	int result = now + rangeLimit(-rate, target-now, rate)
	return rangeLimit(REV, result, FWD);
}

//This function turns 2 motor values into 1 coded value
//This allows a function to return 1 coded value, instead of 2 motor speeds.
//Returning more than 1 value from a function can be difficult. (W/o pointers.)
unsigned int encode(int l, int r) {
	ubyte tempL = rangeLimit(0, l + FWD, 0xFF); //0xFF = 255
	ubyte tempR = rangeLimit(0, r + FWD, 0xFF);
	return (tempL<<8) + tempR;
}

unsigned int encode2(int lr) {
	return encode(lr, lr);
}

unsigned int encodeNegative2(int lr) {
	return encode(lr, -lr);
}

unsigned int encodeL(int l) {
	return encode(l, 0);
}

unsigned int encodeR(int r) {
	return encode(0, r);
}

//This function turns the second coded value back to the original value
int decodeR(unsigned int n) {
	return rangeLimit(REV, (n & 0xFF) - FWD, FWD);
}

//This function turns the first coded value back to the original value
int decodeL(unsigned int n) {
	return decodeR(n >> 8); //bitshifting magic
}

//This function turns two buttons (up/down or left/right) into a motor speed
int buttonsToSpeed(TVexJoysticks forwardBtn, TVexJoysticks reverseBtn, int holdPower = 0) {
	int result = holdPower;
	int btnStatus = vexRT[forwardBtn] - vexRT[reverseBtn];
	if (btnStatus == 1) result = FWD;
	if (btnStatus == -1) result = REV;
	return rangeLimit(REV, result, FWD);
}
