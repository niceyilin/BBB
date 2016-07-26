/*
 * StepperMotor.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: yilin
 */

#include "StepperMotor.h"
#include <iostream>
#include <unistd.h>


StepperMotor::StepperMotor(GPIO *gpio4blue, GPIO *gpio4pink, GPIO *gpio4yellow, GPIO *gpio4orange){
	this->gpio4blue = gpio4blue;
	this->gpio4pink = gpio4pink;
	this->gpio4yellow = gpio4yellow;
	this->gpio4orange = gpio4orange;

	this->gpio4blue->setDirection(OUTPUT);
	this->gpio4pink->setDirection(OUTPUT);
	this->gpio4yellow->setDirection(OUTPUT);
	this->gpio4orange->setDirection(OUTPUT);

	//default setting, can be overwritten by user later
	this->dir = CLOCKWISE;
	this->delay = 5000; //5ms
	this->mode = FULL;

	this->muxedLookupTable_ptr = this->fullMode_lookUpTable;
}

bool StepperMotor::setRotateDir(MOTOR_DIR dir){
	this->dir = dir;
	return 1;
}

bool StepperMotor::setRotateSpeed(size_t delay){
	this->delay = delay;
	return 1;
}

bool StepperMotor::setSteppingMode(STEPPING_MODE mode){
	this->mode = mode;
	return 1;
}

// steps should be the multiple of 8-step
bool StepperMotor::rotateSteps(int steps){
	int i = 0;
	int j = steps / 8;
	// Configure which look up table to use, based on which mode it is
	if(this->mode == FULL){
		this->muxedLookupTable_ptr = this->fullMode_lookUpTable;
	}
	else if(this->mode == FULL_W_DOUBLE_TORQUE){
		this->muxedLookupTable_ptr = this->fullHighTorgue_lookUpTable;
	}
	else{ // HALF
		this->muxedLookupTable_ptr = this->halfMode_lookUpTable;
	}
	// indexing the chosen look up table based on which direction it is
	for(j=steps/8; j>0; j-- ){
		if(this->dir == CLOCKWISE){
			for(i=7; i>=0; i--){
				this->firePulse(i);
				usleep(this->delay); // CAUTION !
			}
		}else{
			for(i=0; i<=7; i++){
				this->firePulse(i);
				usleep(this->delay);  // CAUTION !
			}
		}
	}
	return 1;
}

bool StepperMotor::rotateAngles(int angles){
	size_t steps;
	if(this->mode == HALF){ // HALF step : 0.09 degree step angle, 32*64steps per 360 degree
		steps = angles / 0.09;
	}
	else{ // FULL step : 0.18 degree step angle, 64*64steps per 360 degree
		steps = angles / 0.18;
	}
	this->rotateSteps(steps);
	return 1;
}

void StepperMotor::firePulse(int loopUpEntry){
	bool bit0 = this->muxedLookupTable_ptr[loopUpEntry] & (1 << 0);
	bool bit1 = this->muxedLookupTable_ptr[loopUpEntry] & (1 << 1);
	bool bit2 = this->muxedLookupTable_ptr[loopUpEntry] & (1 << 2);
	bool bit3 = this->muxedLookupTable_ptr[loopUpEntry] & (1 << 3);

	//cout << "firePulse() = " << bit3 << bit2 << bit1 << bit0 << endl; // for debug

	if(bit0 == 1) 	this->gpio4blue->setValue(HIGH);
	else 			this->gpio4blue->setValue(LOW);

	if(bit1 == 1) 	this->gpio4pink->setValue(HIGH);
	else 			this->gpio4pink->setValue(LOW);

	if(bit2 == 1) 	this->gpio4yellow->setValue(HIGH);
	else 			this->gpio4yellow->setValue(LOW);

	if(bit3 == 1) 	this->gpio4orange->setValue(HIGH);
	else 			this->gpio4orange->setValue(LOW);
}

StepperMotor::~StepperMotor(){
	//this->gpio4blue->unexportGPIO();  //needed???? No
}
