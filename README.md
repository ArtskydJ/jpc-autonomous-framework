jpc-autonomous-framework
========================

An autonomous framework for vex teams using [RobotC](http://www.robotc.net)
An entry for [jpearman's 2014 programming challenge](http://www.vexforum.com/showthread.php?t=81887).

- [How to Use](#how-to-use)
- [Basic Api](#basic-api)
	- [Motor Functions](#motor-functions)
	- [Utility Functions](#utility-functions)
	- [Autonomous Functions](#autonomous-functions)
- [Advanced Api](#advanced-api)
	- [Motor Functions](#motor-functions-1)
	- [Utility Functions](#utility-functions-1)
	- [Autonomous Functions](#autonomous-functions-1)
- [License](#license)

#How to use

1. Read this entire file. :D
2. Fork this project and clone your fork to your computer.
3. Open `index.c` and enter all your motors and sensors.
4. Open `config.c` and enter your configuration:
	- Enter your sensor config into the `#define`s. (Should be around line 30.)
	- Enter your lift presets into the `T_PRESETS` enum. (Should be around line 55.)
	- If you know what proportional, slew, and other constants you want, enter them now.
5. Open `motor-functions.c`. Change the motor names in the following functions:
	- In `driveFlFrBlBr()` change `DRIVE_FL` down to `DRIVE_BR2`.
	- In `liftSpeeds()` change `LIFT_L` and `LIFT_R`.
	- In `intakeSpeed()` change `INTK_L` and `INTK_R`.
	- If you *do not* have a mecanum or X-drive, you will want to change how strafing works:
		- If you have an H-drive, you will have to rewrite `driveFlFrBlBr()`, and modify `driveLeftRightStrafe()` and `driveForwardTurnStrafe()` accordingly. If you need help with this, please create an issue on github.
		- If you don't have strafing, just delete all references to `strafe` in `driveLeftRightStrafe()` and `driveForwardTurnStrafe()`. Again, if you need help, create an issue.

#Basic API

The basic API has all the functions you should need. If you want more, check out the [Advanced API](#advanced-api).

##Motor Functions

###void driveFlFrBlBr(int slewRate, int frontLeft, int frontRight, int backLeft, int backRight)

- `int slewRate` is the rate at which the motor approaches its target.
- `int frontLeft` is the target speed for the front left motor(s). Negative for reverse.
- `int frontRight` is the target speed for the front right motor(s). Negative for reverse.
- `int backLeft` is the target speed for the back left motor(s). Negative for reverse.
- `int backRight` is the target speed for the back right motor(s). Negative for reverse.

```c
driveFlFrBlBr(10, FWD, FWD, FWD, FWD);
```

###void driveLeftRightStrafe(int slewRate, int left, int right, int strafe)

- `int slewRate` is the rate at which the motor approaches its target.
- `int left` is the target speed for the left side. Negative for reverse.
- `int right` is the target speed for the left side. Negative for reverse.
- `int strafe` is the target strafing speed. Negative for left.

```c
driveLeftRightStrafe(10, FWD, FWD, 0);
```

###void driveForwardTurnStrafe(int slewRate, int forward, int turn, int strafe)

- `int slewRate` is the rate at which the motor approaches its target.
- `int forward` is the target forward or backward speed. Negative for reverse.
- `int turn` is the target turning speed. Negative for left.
- `int strafe` is the target strafing speed. Negative for left.

```c
driveForwardTurnStrafe(10, FWD, LEFT, 0);
```

###void liftSpeeds(int slewRate, unsigned int both)

- `int slewRate` is the rate at which the motor approaches its target.
- `unsigned int both` holds the speeds for both sides of the lift. The speeds are encoded. (See [Autonomous Functions](#autonomous-functions) for details.)

```c
liftSpeeds(10, encode(FWD, FWD));
```

###void intakeSpeed(int slewRate, int target)

- `int slewRate` is the rate at which the motor approaches its target.
- `int target` is the target speed. Negative for reverse.

```c
intakeSpeed(10, FWD);
```

##Utility Functions

###int rangeLimit(int min, int value, int max)

- `int min` is the minimum value that `rangeLimit()` will return
- `int value` is the value in question. (Note that this variable is not changed. A new integer is returned.)
- `int max` is the maximum value that `rangeLimit()` will return.

Returns the value between min and max. If it is already between min and max, the same value will be returned.

```c
       //min val max
rangeLimit(3, 1, 7); //Returns 3
rangeLimit(3, 2, 7); //Returns 3
rangeLimit(3, 3, 7); //Returns 3
rangeLimit(3, 4, 7); //Returns 4
rangeLimit(3, 5, 7); //Returns 5
rangeLimit(3, 6, 7); //Returns 6
rangeLimit(3, 7, 7); //Returns 7
rangeLimit(3, 8, 7); //Returns 7
rangeLimit(3, 9, 7); //Returns 7
```

###void constantLoopTime()

This function keeps the loop iteration interval consistent in the loop that it's called.

It does this by waiting after each loop iteration until `LOOP_TIME_MS` milliseconds have passed since it started the loop iteration. (`LOOP_TIME_MS` can be found in `config.c` around line 50.)

Basically it puts the code in a [noop](https://en.wikipedia.org/wiki/NOP) loop until X ms have passed since it was last called.

```c
task main() {
	for (int i = 0; i < 500; i++) {
		writeDebugStreamLine("Hello world, %d", time1[T1]);
		wait1Msec(2);
		writeDebugStreamLine("Hello again, %d", time1[T1]);
		constantLoopTime();
	}
}
```

###int potReverse(int potentiometer)

- `int potentiometer` is a number from 0 - 4095 that this function "reverses". (Note that this variable is not changed. A new integer is returned.)

(Technically, this can "reverse" any number from 0 - 4095, but it is commonly used for potentiometers.)

Returns a corrected value for potentiometers that were installed backwards.

```c
potReverse(1234); //Returns 2861
potReverse(2861); //Returns 1234
potReverse(0);    //Returns 4096
potReverse(4096); //Returns 0
potReverse(2048); //Returns 2047
```

###int potPosition(int numOfOptions, int potentiometer)

This function checks how far the potentiometer is turned compared to how many options number allowed.

(Technically, this can scale any number from 0 - 4095, but it is commonly used for potentiometers.)

- `int numOfOptions` is a number from 0 - 4095 that this function allows as a range.
- `int potentiometer` is a number from 0 - 4095 that this function scales.

For example, `potPosition(100, [number])` returns a number between 0 and 99, proportional to how large `[number]` is.

```c
potPosition(3, 0);    //Returns 0
potPosition(3, 1000); //Returns 0
potPosition(3, 1365); //Returns 0
potPosition(3, 1366);  //Returns 1
potPosition(3, 2000);  //Returns 1
potPosition(3, 2730);  //Returns 1
potPosition(3, 2731); //Returns 2
potPosition(3, 3000); //Returns 2
potPosition(3, 4000); //Returns 2
potPosition(3, 4095); //Returns 2
```

###int slew(int target, int now, int rate)

- `int target` is the value that `now` goes toward.
- `int now` is the value that is targetting `target`. (Note that this variable is not changed. A new integer is returned.)
- `int rate` is the rate at which `now` reaches `target`.

Calculates a new speed using target, current speed (`now`), and rate of slew.

See wikipedia article on [slew](https://en.wikipedia.org/wiki/Slew_rate).

```c
//target,  now, rate
slew(100,  -30, 10) //Returns -20
slew(100,  -20, 10) //Returns -10
slew(100,  -10, 10) //Returns 0
slew(100,   0,  10) //Returns 10
slew(100,   10, 10) //Returns 20
slew(100,   20, 10) //Returns 30
slew(100,   30, 10) //Returns 40

//target,  now, rate
slew(127,  100, 10) //Returns 110
slew(127,  110, 10) //Returns 120
slew(127,  120, 10) //Returns 127

//target,  now, rate
slew(-100, -70, 10) //Returns -80
slew(-100, -80, 10) //Returns -90
slew(-100, -90, 10) //Returns -100
```

###int buttonsToSpeed(TVexJoysticks forwardButton, TVexJoysticks reverseButton)

- `TVexJoysticks forwardButton` 
- `TVexJoysticks reverseButton`

Turns two buttons (up/down or left/right) into a motor speed.

```c
buttonsToSpeed(Btn6U, Btn6D);
//Returns 127 if 6U is pressed and 6D is not.
//Returns -127 if 6U is not pressed and 6D is.
//Returns 0 if 6U and 6D are both not pressed, or both pressed.

buttonsToSpeed(Btn8L, Btn8R);
//Returns 127 if 8L is pressed and 8R is not.
//Returns -127 if 8L is not pressed and 8R is.
//Returns 0 if 8L and 8R are both not pressed, or both pressed.
```

##Autonomous Functions

The functions in this section return encoded values. These values have two motor speeds encoded inside them. These values are `unsigned int`s. This is how it works:

RobotC's `unsigned int`s are 16 bits. The `byte`s are 8 bits. Two `byte`s can be stuffed into an `unsigned int`. The `motor[]` array built into RobotC is an array of `byte`s.

Imagine you have a function that calculates how fast the left and right motors should spin depending on a sensor. How do you get both numbers `return`ed from a function? Encode them!

Let's say that the left side is supposed to go a speed of -60, and the right side is supposed to go a speed of 120. 127 is added to both numbers to make them positive. Then the right number is bitshifted and then the numbers are added. The resulting number is returned. See example below:

```
l     r       l(hex)  r(hex)   l(binary)  r(binary)          Notes
-60   120                                                    start
67    247     0x43    0xF7     1000011    11110111           add 127
67    63232   0x43    0xF700   1000011    1111011100000000   bitshift 'r' 8 bits
  63299          0xF743            1111011101000011          resulting number
```

###unsigned int stopped()

Returns an encoded number for both sides going a speed of 0.

Aliases: `encode2(0)`, `encode(0, 0)`


###unsigned int straight(int spd)

Returns an encoded number for both sides going a speed of `spd`.

Aliases: `encode2(spd)`, `encode(spd, spd)`


###unsigned int turn(int spd)

Returns an encoded number for the left side going a speed of `spd`, and the right side going a speed of `-spd`.

Aliases: `encodeNegative2(spd)`, `encode(spd, -spd)`


###unsigned int speeds(int l, int r)

Returns an encoded number for the left side going a speed of `l`, and the right side going a speed of `r`.

Alias: `encode(l, r)`


###unsigned int speedL(int l)

Returns an encoded number for the left side going a speed of `l`, and the right side going a speed of 0.

Aliases: `encode(l, 0)`, `encodeL(l)`


###unsigned int speedR(int r)

Returns an encoded number for the left side going a speed of 0, and the right side going a speed of `r`.

Aliases: `encode(0, r)`, `encodeR(r)`


###unsigned int gyro2(int deg)

Returns an encoded number for the left side to go one way and the right side to go the other. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speeds the wheels should turn to accurately turn `deg` number of degrees.


###unsigned int enc(int dist)

Returns an encoded number for the left and right sides to go to the target encoder distance. It reads the left and right encoders specified in `config.c` as `CURRENT_LEFT_ENC` and `CURRENT_RIGHT_ENC` and uses `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints.

Alias: `enc2(dist, dist)`, `enc1Spd(dist, FWD)`



#Advanced API

If you're not satisfied with the [Basic API](#basic-api), and just want more, you've come the the right place; the Advanced API!

##Motor Functions

###void applySlew(int slewRate, int mtr, int target)

A wrapper function for [`slew()`](#int-slewint-target-int-now-int-rate) that applies the slew result to a given motor. The `slew()` function's `now` argument is `applySlew()`'s supplied motor's speed.

- `int slewRate` is the rate at which the motor's power approaches its target.
- `int mtr` is the motor number to which the slew is applied. You can supply the motor name.
- `int target` is the target power for the motor.

Please note that these arguments are backwards from the `slew()` function, in which `target` is first and `slewRate` is last.

```c
applySlew(10, DRIVE_FL, 100);
applySlew(10, 0,        100);
```


##Utility Functions

Forgot how encoding works? Check out the [explanation](#autonomous-functions).

###unsigned int encode(int l, int r)

This function produces the encoded value from two numbers ranging from -127 - 127, and returns it as an `unsigned int`.

###unsigned int encode2(int lr)

Shorthand for `encode(lr, lr)`.

###unsigned int encodeNegative2(int lr)

Shorthand for `encode(lr, -lr)`.

###unsigned int encodeL(int l)

Shorthand for `encode(l, 0)`.

###unsigned int encodeR(int r)

Shorthand for `encode(0, r)`.

###int decodeL(unsigned int n)

This function takes an encoded value and returns the motor power for the left side.

```c
decodeL( encode(100, 99) ); //Returns 100
```

###int decodeL(unsigned int n)

This function takes an encoded value and returns the motor power for the right side.

```c
decodeL( encode(100, 99) ); //Returns 99
```


##Autonomous Functions

###int encStrafe1(int n)

Returns a *non*-encoded number for strafing `n` encoder ticks reading 1 encoder.

###int encStrafe2(int n)

Returns a *non*-encoded number for strafing `n` encoder ticks reading 2 encoders.

###unsigned int gyroL(int deg)

Returns an encoded number for the left side to target the gyro. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speed the left wheel(s) should turn to accurately turn `deg` number of degrees.

###unsigned int gyroR(int deg)

Returns an encoded number for the right side to target the gyro. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speed the right wheel(s) should turn to accurately turn `deg` number of degrees.

###unsigned int enc2(int distL, int distR)

Returns an encoded number for the left and right sides to go to their individual target encoder distances. It reads the left and right encoders (specified in `config.c`) and `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints.

###unsigned int enc1Spd(int dist, int spd)

Returns an encoded number for the left and right sides to go to the target encoder distance. It reads the left and right encoders (specified in `config.c`) and `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints. The max speed for each side is specified in `spd`.

###unsigned int liftPreset(T_PRESETS height)

Returns an encoded number for the left and right sides of the lift to go toward the height specified.


#License

[MIT](http://opensource.org/licenses/MIT)

Copyright (c) 2014, Joseph Dykstra
