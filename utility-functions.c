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


//This function forces the target between min and max. Examples:
//rangeLimit(3, 1, 7) -> 3
//rangeLimit(3, 19, 7) -> 7
//rangeLimit(3, 5, 7) -> 5
int rangeLimit(int min, int value, int max) {
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}


//This function returns a corrected value for potentiometers that were installed backwards.
int potReverse(int potentiometer) {
	return (4095 - potentiometer);
}


//This function calcs a new motor value with target, previous, and slew values. Examples:
//slew(100, 0, 5) -> 5
//slew(100, 15, 5) -> 20
//slew(-100, -80, 5) -> -85
int slew(int target, int last, int rate) {
	return (last + rangeLimit(-rate, target-last, rate));
}

//This function turns 2 motor values into 1 coded value
//This allows a function to return 1 coded value, instead of 2 motor speeds.
//Returning more than 1 value from a function can be difficult. (W/o pointers.)
unsigned int encode(int l, int r) {
	ubyte tempL = rangeLimit(REV, l, FWD) + FWD;
	ubyte tempR = rangeLimit(REV, r, FWD) + FWD;
	return (tempL<<8) + tempR;
}

//This function turns the second coded value back to the original value
int decodeR(unsigned int n) {
	return rangeLimit(0, (n & 0xFF), (FULL * 2)) - FULL;
}

//This function turns the first coded value back to the original value
int decodeL(unsigned int n) {
	return decodeR(n >> 8); //bitshifting magic
}
