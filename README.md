jpc-autonomous-framework
========================

An autonomous framework for vex teams using [RobotC](http://www.robotc.net)...

The point of this project is so that VEX teams can use this code and write autonomous routines easily. I am disappointed by the lack of autonomous routines in VEX. I hope that this will help people to see that writing routines is not hard. I also hope that teams will contribute to this project, which helps out other teams by doing so.

This was an entry for [jpearman's 2014 programming challenge](http://www.vexforum.com/showthread.php?t=81887).

- [How to Use](#how-to-use)
- [Contributing](#contributing)
- [Terms](#terms)
- [Basic Api](#basic-api)
	- [Motor Functions](#motor-functions)
	- [Utility Functions](#utility-functions)
	- [Autonomous Functions](#autonomous-functions)
- [Advanced Api](#advanced-api)
	- [Motor Functions](#motor-functions-1)
	- [Utility Functions](#utility-functions-1)
	- [Autonomous Functions](#autonomous-functions-1)
- [License](#license)



# How to use

1. Read this entire file. :D
2. [Download the source code](https://github.com/ArtskydJ/jpc-autonomous-framework/releases) to your computer.
3. Open `index.c` and enter all your motors and sensors.
4. Open `config.c` and enter your configuration:
	- Enter your sensor config into the `#define`s. (Should be around line 25.)
	- Enter your lift presets into the `T_PRESETS` enum. (Should be around line 55.)
	- Comment out the definition of `STRAFE` if you have a tank drive.
	- Un-comment out the definition of `H_DRIVE` if you have an H-Drive.
	- If you know what proportional, slew, and other constants you want, enter them now.
5. Open `motor-functions.c`. Change the motor names in the following functions:
	- In [`driveFlFrBlBr()`][driveFlFrBlBr] change `DRIVE_FL` down to `DRIVE_BR2`.
	- If you have an H-Drive, change `DRIVE_L1` through `DRIVE_L2`.
	- In [`liftSpeeds()`][liftSpeeds] change `LIFT_L` and `LIFT_R`.
	- In [`intakeSpeed()`][intakeSpeed] change `INTK_L` and `INTK_R`.



# Contributing

When making an issue or a pull request, please do not be vague.

If you see a typo or want to add documentation, please [click here to edit](https://github.com/ArtskydJ/jpc-autonomous-framework/edit/master/README.md) on github.com. Commit it, send me a pull request, and I will merge it when I get the chance.

If you want to add/modify/delete code; go ahead, then make a pull request, and tell me what your code does.

If you have a question; create an issue on github. That way, the community can be involved. Please check the documentation to avoid embarrassment. ;-)

If you find a bug; create an issue on github, or fix it and send a pull request. Or both.

If you have concerns that this will help teams that don't know how to program, keep those concerns to yourself! VEX is about learning!



# Terms

- The acronym `jpc` stands for `jpearman programming challenge`.
- The terms `speed` and `power` are used interchangeably as motor speed/power values.
- An `API` is (in this case) a collection of functions that are for the programmer to use.



# Constants

## `#define`s

These can be used anywhere.

| Name             | Value  | Description
|:-----------------|:------:|:-----------------|
| FWD              |   127  | Drive direction
| REV              | (-127) | Drive direction
| UP               |   127  | Lift
| DOWN             | (-127) | Lift
| LEFT             | (-127) | Turning
| RIGHT            |   127  | Turning
| IN               | (-127) | Intake
| OUT              |   127  | Intake
| FULL             |   127  | Max speed
| HALF             |   64   | Half of max speed
| BRAKE            |   5    | Braking motor power
| MOTOR_ALLOW_ZONE |   30   | Allowable speed for going to next step.


## `enum`s:

### T_SOLENOID_OPTS

- `OFF` - When used with [`solenoid()`][solenoid], this will turn the specified solenoid off.
- `ON` - When used with [`solenoid()`][solenoid], this will turn the specified solenoid on.
- `TOGGLE` - When used with [`solenoid()`][solenoid], this will toggle the specified solenoid.
- `NUM_OF_SOLENOID_OPTS` - `3` in this case.

### T_END_TYPES
- `TIME_LIMIT` - End when the step reached the time specified in `endTime`.
- `DRIVE_MOTORS` - End when drive motors are going slow, and then add `endTime`. (I.E., They reached their target position(s).)
- `LIFT_MOTORS` - End when lift motors are going slow, and then add `endTime`. (I.E., They reached their target position(s).)
- `INTAKE_MOTORS` - End when intake motors are going slow, and then add `endTime`. (I.E., They reached their target position.)
- `ALL_MOTORS` - End when the drive, lift, and intake motors are going slow, and then add `endTime`. (I.E., They all reached their respective target positions.)
- `NUM_OF_END_TYPES` - In this case, `5`. Do not use in [`auto()`][auto] as an `endType`.



# Basic API

The basic API has all the functions you should need. If you want more, check out the [Advanced API](#advanced-api).


## Motor Functions

It is not suggested to use these functions for autonomous. Try using [`auto()`][auto].

### void driveFlFrBlBr(int slewRate, int frontLeft, int frontRight, int backLeft, int backRight)

- `int slewRate` is the rate at which the motor approaches its target.
- `int frontLeft` is the target speed for the front left motor(s). Negative for reverse.
- `int frontRight` is the target speed for the front right motor(s). Negative for reverse.
- `int backLeft` is the target speed for the back left motor(s). Negative for reverse.
- `int backRight` is the target speed for the back right motor(s). Negative for reverse.

```c
driveFlFrBlBr(10, FWD, FWD, FWD, FWD); //Drive forward
```

### void driveLeftRightStrafe(int slewRate, int left, int right, int strafe)

- `int slewRate` is the rate at which the motor approaches its target.
- `int left` is the target speed for the left side. Negative for reverse.
- `int right` is the target speed for the left side. Negative for reverse.
- `int strafe` is the target strafing speed. Negative for left.

If you run this once with a preset distance, don't expect this to work. This has to be run in a loop to accurately target a certain distance. Don't worry; the looping is done automatically in `task autonomous()`, inside `index.c`.

```c
driveLeftRightStrafe(10, FWD, FWD, 0); //Drive forward
```

### void driveForwardTurnStrafe(int slewRate, int forward, int turn, int strafe)

- `int slewRate` is the rate at which the motor approaches its target.
- `int forward` is the target forward or backward speed. Negative for reverse.
- `int turn` is the target turning speed. Negative for left.
- `int strafe` is the target strafing speed. Negative for left.

If you run this once with a preset distance, don't expect this to work. This has to be run in a loop to accurately target a certain distance. Don't worry; the looping is done automatically in `task autonomous()`, inside `index.c`.

```c
driveForwardTurnStrafe(10, FWD, 0, 0); //Drive forward
```

### void liftSpeeds(int slewRate, unsigned int both)

- `int slewRate` is the rate at which the motor approaches its target.
- `unsigned int both` holds the speeds for both sides of the lift. The speeds are encoded. (See [Encoding][encoding]) for details of encoding.)

If you run this once with a preset height, don't expect this to work. This has to be run in a loop to accurately target a certain height. Don't worry; the looping is done automatically in `task autonomous()`, inside `index.c`.

```c
liftSpeeds(10, encode(FWD, FWD)); //Lift forward
```

### void intakeSpeed(int slewRate, int target)

- `int slewRate` is the rate at which the motor approaches its target.
- `int target` is the target speed. Negative for reverse.

```c
intakeSpeed(10, FWD); //Intake forward
```


## Utility Functions

### int rangeLimit(int min, int value, int max)

- `int min` is the minimum value that `rangeLimit()` will return
- `int value` is the value in question. (Note that this variable is not changed. A new integer is returned.)
- `int max` is the maximum value that `rangeLimit()` will return.

Returns the value between min and max. If it is already between min and max, the same value will be returned. If min is larger than max, then the values are swapped.

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

### void constantLoopTime()

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

### int potReverse(int potentiometer)

- `int potentiometer` is a number from `0` - `4095` that this function "reverses". (Note that this variable is not changed. A new integer is returned.)

(Technically, this can "reverse" any number from `0` - `4095`, but it is commonly used for potentiometers.)

Returns a corrected value for potentiometers that were installed backwards. Range limits the returned value between `0` and `4095`.

```c
potReverse(1234); //Returns 2861
potReverse(2861); //Returns 1234
potReverse(0);    //Returns 4095
potReverse(4095); //Returns 0
potReverse(2048); //Returns 2047
```

### int potPosition(int numOfOptions, int potentiometer)

- `int numOfOptions` is a number from `0` - `4095` that this function allows as a range.
- `int potentiometer` is a number from `0` - `4095` that this function scales.

This function checks how far the potentiometer is turned compared to how many options number allowed.

(Technically, this can scale any number from `0` - `4095`, but it is commonly used for potentiometers.)

For example, `potPosition(100, variable)` returns a number between `0` and `99`, proportional to how large `variable` is.

Range limits the returned value between `0` and `numOfOptions-1`.

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

### int slew(int target, int now, int rate)

- `int target` is the value that `now` goes toward.
- `int now` is the value that is targetting `target`. (Note that this variable is not changed. A new integer is returned.)
- `int rate` is the rate at which `now` reaches `target`.

Calculates a new speed using the target speed, current speed (`now`), and rate of slew. Range limits the returned value between `REV` and `FWD`.

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
slew(129,  120, 10) //Returns 127
slew(-129,-120, 10) //Returns -127

//target,  now, rate
slew(-100, -70, 10) //Returns -80
slew(-100, -80, 10) //Returns -90
slew(-100, -90, 10) //Returns -100
```

### int buttonsToSpeed(TVexJoysticks forwardBtn, TVexJoysticks reverseBtn, int holdPower=0)

- `TVexJoysticks forwardBtn` is the button that causes `127` to be returned when pressed.
- `TVexJoysticks reverseBtn` is the button that causes `-127` to be returned when pressed.
- `int holdPower` is the value returned if `forwardBtn` and `reverseBtn` are both not pressed or both pressed. Useful for default lift holding power, or similar.

Turns two buttons (up/down or left/right) into a motor speed.

```c
buttonsToSpeed(Btn6U, Btn6D);
```

Returns `127` if `6U` is pressed and `6D` is not.  
Returns `-127` if `6U` is not pressed and `6D` is.  
Returns `0` if `6U` and `6D` are both not pressed, or both pressed.

```c
buttonsToSpeed(Btn8L, Btn8R, 45);
```

Returns `127` if `8L` is pressed and `8R` is not.  
Returns `-127` if `8L` is not pressed and `8R` is.  
Returns `45` if `8L` and `8R` are both not pressed, or both pressed.


## Autonomous Functions

### How Encoding Works:

The functions in this section return encoded values. These values have two motor speeds encoded inside them. These values are `unsigned int`s. This is how it works:

RobotC's `unsigned int`s are 16 bits. The `byte`s are 8 bits. Two `byte`s can be stuffed into an `unsigned int`. The `motor[]` array built into RobotC is an array of `byte`s.

Imagine you have a function that calculates how fast the left and right motors should spin depending on a sensor. How do you get both numbers `return`ed from a function? Encode them!

Let's say that the left side is supposed to go a speed of `-60`, and the right side is supposed to go a speed of `120`. `127` is added to both numbers to make them positive. Then the right number is bitshifted and then the numbers are added. The resulting number is returned. See example below:

| l   | r     |l (hex)| r (hex)| l (binary)| r (binary)        | Notes               |
|:----|:------|:------|:-------|:----------|:------------------|:--------------------|
| -60 | 120   |       |        |           |                   | original numbers    |
| 67  | 247   |  0x43 | 0xF7   |  01000011 | 11110111          | add 127 to both     |
| 67  | 63232 |  0x43 | 0xF700 |  01000011 | 11110111 00000000 | bitshift 'r' 8 bits |
|  └► | 63299 |    └► | 0xF743 |     └►    | 11110111 01000011 | add 'l' and 'r'     |


### void auto(unsigned int driveLR, int driveStrafe, unsigned int liftLR, int intake, T_END_TYPES endType, int endTime)

- `unsigned int driveLR` is an encoded value holding the speeds for the left and right sides of the drive.
- `int driveStrafe` is number from `-127` - `127` for the power that the robot will strafe. Negative for left.
- `unsigned int liftLR` is an encoded value holding the speeds for the left and right sides of the lift.
- `int intake` is a number from `-127` - `127` for the power that the robot will intake. Negative for in, positive for out.
- `T_END_TYPES endType` is how the step decides when to continue to the next step. See [`T_END_TYPES`](#t_end_types).
- `int endTime` is a number of milliseconds. If the `endType` is `TIME_LIMIT` then this is how long before the step goes to the next step. If the `endType` is something else, then this is how long the step continues after its target is reached. This is useful when, for example, the robot is turning to a certain gyro target, the step will not continue when the robot is close, but when it is done.

```c
//Do nothing for half a second:
auto(stopped(), 0, stopped(), 0, TIME_LIMIT, 500);

//Turn 90 degrees:
auto(gyro2(90), 0, stopped(), 0, DRIVE_MOTORS, 300);

//Try to turn 90 degrees, but continue to next step too soon,
//because it got close and continued immediately:
auto(gyro2(90), 0, stopped(), 0, DRIVE_MOTORS, 0);

//Raise lift to the medium post:
auto(stopped(), 0, liftPreset(MED_POST), 0, LIFT_MOTORS, 500);

//Strafe right for half a second:
auto(stopped(), RIGHT, stopped(), 0, TIME_LIMIT, 500);
```

### unsigned int stopped()

Returns an encoded number for both sides going a speed of 0.

Aliases: `encode2(0)`, `encode(0, 0)`


### unsigned int straight(int spd)

Returns an encoded number for both sides going a speed of `spd`.

Aliases: `encode2(spd)`, `encode(spd, spd)`


### unsigned int speeds(int l, int r)

Returns an encoded number for the left to go a speed of `l`, and the right to go going a speed of `r`.

Alias: `encode(l, r)`


### unsigned int turn(int spd)

Returns an encoded number for the left to go a speed of `spd`, and the right to go a speed of `-spd`.

Aliases: `encodeNegative2(spd)`, `encode(spd, -spd)`


### unsigned int speedL(int l)

Returns an encoded number for the left side going a speed of `l`, and the right side to be stopped.

Aliases: `encode(l, 0)`, `encodeL(l)`


### unsigned int speedR(int r)

Returns an encoded number for the left side to be stopped, and the right side going a speed of `r`.

Aliases: `encode(0, r)`, `encodeR(r)`


### unsigned int gyro2(int deg)

- `int deg` is the number of degrees the robot is supposed to turn in that step.

Returns an encoded number for the left side to go one way and the right side to go the other. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speeds the wheels should turn to accurately turn `deg` number of degrees.


### unsigned int enc(int dist)

- `int dist` is the number of encoder ticks the robot is supposed to travel in that step.

Returns an encoded number for the left and right sides to go to the target encoder distance. It reads the left and right encoders specified in `config.c` as `CURRENT_LEFT_ENC` and `CURRENT_RIGHT_ENC` and uses `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints.

Alias: `enc2(dist, dist)`, `enc1Spd(dist, FWD)`



# Advanced API

Such advanced. Much wow.


## Motor Functions

### void applySlew(int slewRate, int mtr, int target)

A wrapper function for [`slew()`][slew] that applies the slew result to a given motor. The [`slew()`][slew] function's `now` argument is `applySlew()`'s supplied motor's speed.

- `int slewRate` is the rate at which the motor's power approaches its target.
- `int mtr` is the motor number to which the slew is applied. You can supply the motor name.
- `int target` is the target power for the motor.

Please note that these arguments are backwards from the [`slew()`][slew] function, in which `target` is first and `slewRate` is last.

```c
applySlew(10, DRIVE_FL, 100); //Sets motor[DRIVE_FL] 10 power closer to 100
applySlew(10, 0,        100); //Sets motor[0] 10 power closer to 100
```


## Utility Functions

Forgot how encoding works? Check out the [explanation][encoding].

These functions are used by higher-level functions in the "Autonomous Functions" sections. Direct usage could lead to confusion.

If you want to add one of these functions as an argument to [`auto()`][auto], please use an alias: [Basic](#autonomous-functions) or [Advanced](#autonomous-functions-1).

If you are creating a function to be used as an argument to [`auto()`][auto], feel free to use the following:

### unsigned int encode(int l, int r)

This function produces the encoded value from two numbers ranging from `-127` - `127`, and returns it as an `unsigned int`.

### unsigned int encode2(int lr)

Shorthand for `encode(lr, lr)`.

### unsigned int encodeNegative2(int lr)

Shorthand for `encode(lr, -lr)`.

### unsigned int encodeL(int l)

Shorthand for `encode(l, 0)`.

### unsigned int encodeR(int r)

Shorthand for `encode(0, r)`.

### int decodeL(unsigned int n)

This function takes an encoded value and returns the motor power for the left side.

```c
decodeL( encode(100, 99) ); //Returns 100
```

### int decodeL(unsigned int n)

This function takes an encoded value and returns the motor power for the right side.

```c
decodeL( encode(100, 99) ); //Returns 99
```


## Autonomous Functions

### int encStrafe1(int n)

- `int n` is the number of encoder ticks the robot is supposed to travel in that step.

Returns a *non*-encoded number for strafing `n` encoder ticks reading 1 encoder.

### int encStrafe2(int n)

- `int n` is the number of encoder ticks the robot is supposed to travel in that step.

Returns a *non*-encoded number for strafing `n` encoder ticks reading 2 encoders.

### unsigned int gyroL(int deg)

- `int deg` is the number of degrees the robot is supposed to turn in that step.

Returns an encoded number for the left side to target the gyro. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speed the left wheel(s) should turn to accurately turn `deg` number of degrees.

### unsigned int gyroR(int deg)

- `int deg` is the number of degrees the robot is supposed to turn in that step.

Returns an encoded number for the right side to target the gyro. It reads the gyro sensor specified in `config.c` as `CURRENT_GYRO` and uses `GYRO_P` to calculate the speed the right wheel(s) should turn to accurately turn `deg` number of degrees.

### unsigned int enc2(int distL, int distR)

- `int distL` is the number of encoder ticks the robot's left side is supposed to travel in that step.
- `int distR` is the number of encoder ticks the robot's right side is supposed to travel in that step.

Returns an encoded number for the left and right sides to go to their individual target encoder distances. It reads the left and right encoders (specified in `config.c`) and `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints.

### unsigned int enc1Spd(int dist, int spd)

- `int dist` is the number of encoder ticks the robot is supposed to travel in that step.
- `int spd` is the maximum speed the robot will go during that step. (If it is set as `99`, the max speed is `99`, and the max reverse speed is `-99`.)

Returns an encoded number for the left and right sides to go to the target encoder distance. It reads the left and right encoders (specified in `config.c`) and `ENC_DRV_P` to calculate the speeds the wheels should turn to accurately reach the setpoints. The max speed for each side is specified in `spd`.

### void solenoid(int sensor, T_SOLENOID_OPTS targetState)

- `int sensor` is the digital out that will be affected.
- `T_SOLENOID_OPTS targetState` is the state that the solenoid is being set to. See [`T_SOLENOID_OPTS`](#t_solenoid_opts)

```c
solenoid(CATAPULT, TOGGLE); //Toggle catapult solenoid
solenoid(0, OFF); //Turn off solenoid 0
```

### unsigned int liftPreset(T_PRESETS height)

`T_PRESETS height` is a custom defined type of variable called `T_PRESETS` that defines the height target for the lift.

`T_PRESETS` is defined in `config.c` around line 50. You must have these numbers match your lift's heights. Presets can be changed, added, or deleted. (E.g. `GROUND` could be deleted, and/or `WALL` could be added. While adding or deleting any, watch for commas; They'll cause compiler errors if they're missing or if there are too many.)

Returns an encoded number for the left and right sides of the lift to go toward the height specified.

Normally used inside [`auto()`][auto] in the `unsigned int liftLR` parameter.

Example of direct usage:

```c
//Raise/lower lift to medium post height with a slew of 10.
ClearTimer(T1);
while (time1[T1] < 500 || abs(motor[LIFT_L]) > 30) {
	liftSpeeds(10, liftPreset(MED_POST));
}
```


# License

[MIT](http://opensource.org/licenses/MIT)

Copyright (c) 2014, Joseph Dykstra



[encoding]: #how-encoding-works
[slew]: #int-slewint-target-int-now-int-rate
[auto]: #void-autounsigned-int-drivelr-int-drivestrafe-unsigned-int-liftlr-int-intake-t_end_types-endtype-int-endtime
[driveFlFrBlBr]: #void-driveflfrblbrint-slewrate-int-frontleft-int-frontright-int-backleft-int-backright
[driveForwardTurnStrafe]: #void-driveforwardturnstrafeint-slewrate-int-forward-int-turn-int-strafe
[driveLeftRightStrafe]: #void-driveleftrightstrafeint-slewrate-int-left-int-right-int-strafe
[solenoid]: #void-solenoidint-sensor-t_solenoid_opts-targetstate
[liftSpeeds]: #void-liftspeedsint-slewrate-unsigned-int-both
[intakeSpeed]: #void-intakespeedint-slewrate-int-target
