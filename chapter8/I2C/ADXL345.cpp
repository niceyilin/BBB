/*
 * ADXL345.cpp
 *
 *  Created on: Jun 26, 2016
 *      Author: yilin
 */


#include "ADXL345.h"
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <iomanip>

using namespace std;

ADXL345::ADXL345(unsigned int i2cBusNum, unsigned int i2cAddr): // CAUTION!
	I2CDevice(i2cBusNum, i2cAddr){ // parent's constructor, open I2C device to read
	this->i2cBusNum = i2cBusNum;
	this->i2cAddr = i2cAddr;
	this->registers = NULL;
	this->range = PLUSMINUS_16_G;
	this->resolution = HIGH;
	this->accelerationX = 0;
	this->accelerationY = 0;
	this->accelerationZ = 0;
	this->pitch = 0.0;
	this->roll = 0.0;
	this->writeRegister(POWER_CTL, 0x08);
	this->updateFormatReg();
}


ADXL345::~ADXL345(){

}

int ADXL345::updateFormatReg(){
	char data_format = 0x00;
	data_format = data_format|(this->resolution<<3);
	data_format = data_format|this->range;
	return this->writeRegister(DATA_FORMAT, data_format);
}

int ADXL345::setRange(RANGE range){
	this->range = range;
	this->updateFormatReg();
	return 0;
}

int ADXL345::setResolution(RESOLUTION resolution){
	this->resolution = resolution;
	this->updateFormatReg();
	return 0;
}

/* read register from sensor, update raw XYZ data, calculate pitch and roll
 *
 */
int ADXL345::readSensorState(){
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	// sanity check
	if(*(registers + DEVID) != 0xe5){
		perror("Error: ADXL345 sensor DEVID validation failed!");
		return -1;
	}
	this->accelerationX = this->combineRegisters(*(registers+ACC_X_MSB), *(registers+ACC_X_LSB));
	this->accelerationY = this->combineRegisters(*(registers+ACC_Y_MSB), *(registers+ACC_Y_LSB));
	this->accelerationZ = this->combineRegisters(*(registers+ACC_Z_MSB), *(registers+ACC_Z_LSB));
	this->resolution = (ADXL345::RESOLUTION)((*(registers+DATA_FORMAT) & 0x08) >> 3);
	this->range = (ADXL345::RANGE)(*(registers+DATA_FORMAT) & 0x03);
	this->calculatePitchAndRoll();
	return 0;
}

short ADXL345::combineRegisters(unsigned char msb, unsigned char lsb){
	short combinedReg;
	combinedReg = (((short)msb << 8) | ((short)lsb));
	return combinedReg;
}

int ADXL345::calculatePitchAndRoll(){
	float gravity_range;
	switch(this->range){
		case PLUSMINUS_2_G: gravity_range = 4.0; break;
		case PLUSMINUS_4_G: gravity_range = 8.0; break;
		case PLUSMINUS_8_G: gravity_range = 16.0; break;
		default: 			gravity_range = 32.0; break;
	}
	float res;
	if(this->resolution == NORMAL){
		res = 1024.0;
	}
	else{
		res = 8192.0;
	}
	float factor = gravity_range / res ;
	float accX = this->accelerationX * factor;
	float accY = this->accelerationY * factor;
	float accZ = this->accelerationZ * factor;
	float accX2 = accX * accX;
	float accY2 = accY * accY;
	float accZ2 = accZ * accZ;
	this->pitch = 180 * atan(accX/sqrt(accY2+accZ2)) / M_PI;
	this->roll  = 180 * atan(accY/sqrt(accX2+accZ2)) / M_PI;

	return 0;
}

/*  every some time, read sensor state, calculate pitch and roll
 *  and display them.  iterate many times
 */
void ADXL345::displayPitchAndRoll(int iteration){
	int i = iteration;
	while(i > 0){
		cout << "Pitch = " << this->pitch << ", Roll = " << this->roll << endl;
		usleep(100000); //0.1s
		this->readSensorState(); // get update
		i--;
	}
}

void ADXL345::debugSantityCheck(int numOfRd){
	cout << "DEVID = " << "0x" << setfill('0') << setw(2) << hex << int(this->readRegister(0x00)) << endl;
	this->debugDumpReg(numOfRd);
}


