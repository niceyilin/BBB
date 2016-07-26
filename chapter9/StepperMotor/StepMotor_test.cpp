/*
 * StepMotor_test.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: yilin
 */



#include "GPIO.h"
#include "StepperMotor.h"

#include <iostream>

using namespace std;

int main(){
	GPIO gpioBlue(44); // BBB GPIO1_12
	GPIO gpioPink(26); // BBB GPIO0_26
	GPIO gpioYellow(46); // BBB GPIO1_14
	GPIO gpioOrange(65); // BBB GPIO2_1

	cout << "Stepper Motor Control Program started..." << endl;
	StepperMotor MOTOR(&gpioBlue, &gpioPink, &gpioYellow, &gpioOrange);
	MOTOR.setRotateDir(StepperMotor::CLOCKWISE);
	MOTOR.setRotateSpeed(1000);
	MOTOR.setSteppingMode(StepperMotor::HALF);

	//MOTOR.rotateSteps(16);
	MOTOR.rotateAngles(90);

	MOTOR.setRotateDir(StepperMotor::ANTICLOCKWISE);
	MOTOR.setRotateSpeed(500);
	MOTOR.setSteppingMode(StepperMotor::FULL_W_DOUBLE_TORQUE);
	MOTOR.rotateAngles(360);

	return 0;
}
