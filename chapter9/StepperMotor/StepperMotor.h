/*
 * StepperMotor.h
 *
 *  Created on: Jul 24, 2016
 *      Author: yilin
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include "GPIO.h"

class StepperMotor{
	/*
	 * parameter to configure:
	 * which GPIO pins to control the motor, rotate speed, rotate direction
	 * rotate steps, rotate angles, stepping mode
	 */
public:
	enum MOTOR_DIR {CLOCKWISE, ANTICLOCKWISE};
	enum STEPPING_MODE {FULL, FULL_W_DOUBLE_TORQUE, HALF};
private:
	GPIO *gpio4blue, *gpio4pink, *gpio4yellow, *gpio4orange;
	MOTOR_DIR dir;
	size_t delay;
	STEPPING_MODE mode;
	//int fullMode_lookUpTable[8] 		= {1000, 1000, 100, 100, 10, 10, 1, 1};
	//int fullHighTorgue_lookUpTable[8] 	= {1100, 110, 11, 1001, 1100, 110, 11, 1001};
	//int halfMode_lookUpTable[8] 		= {1000, 1100, 100, 110, 10, 11, 1, 1001};
	int fullMode_lookUpTable[8] 			= {0b1000, 0b1000, 0b0100, 0b0100, 0b0010, 0b0010, 0b0001, 0b0001};
	int fullHighTorgue_lookUpTable[8] 		= {0b1100, 0b0110, 0b0011, 0b1001, 0b1100, 0b0110, 0b0011, 0b1001};
	int halfMode_lookUpTable[8] 			= {0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001};
	int *muxedLookupTable_ptr;
	void firePulse(int loopUpEntry);
public:
	StepperMotor(GPIO *gpio4blue, GPIO *gpio4pink, GPIO *gpio4yellow, GPIO *gpio4orange);

	bool setRotateDir(MOTOR_DIR dir);
	bool setRotateSpeed(size_t delay);
	bool setSteppingMode(STEPPING_MODE mode);

	bool rotateSteps(int steps);
	bool rotateAngles(int angles);

	~StepperMotor();
};



#endif /* STEPPERMOTOR_H_ */
