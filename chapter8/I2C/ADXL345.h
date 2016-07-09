/*
 * ADXL345.h
 *
 *  Created on: Jun 26, 2016
 *      Author: yilin
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include "I2CDevice.h"

#define BUFFER_SIZE		0x40
#define DEVID			0x00
#define POWER_CTL 		0x2D
#define DATA_FORMAT		0x31
#define ACC_X_LSB		0x32
#define ACC_X_MSB		0x33
#define ACC_Y_LSB		0x34
#define ACC_Y_MSB		0x35
#define ACC_Z_LSB		0x36
#define ACC_Z_MSB		0x37

/* ADXL345 "protected" inherit I2CDevice
 * so ADXL345 method can access I2CDevice method
 * but ADXL345 instance can't use I2CDevice method directly
 */
class ADXL345 : protected I2CDevice{

public:
	enum RANGE{
		PLUSMINUS_2_G	= 0, // +/- 2g
		PLUSMINUS_4_G	= 1, // +/- 2g
		PLUSMINUS_8_G	= 2, // +/- 2g
		PLUSMINUS_16_G	= 3  // +/- 2g
	};
	enum RESOLUTION {
		NORMAL 	= 0, // 10 bits resolution
		HIGH	= 1  // 13 bits resolution
	};

private:
	unsigned int i2cBusNum, i2cAddr;
	unsigned char *registers;
	RANGE range;
	RESOLUTION resolution;
	short accelerationX, accelerationY, accelerationZ; // raw 2's comp data
	float pitch, roll; // in degree
	// methods used internally
	int updateFormatReg();
	short combineRegisters(unsigned char msb, unsigned char lsb);
	int calculatePitchAndRoll();

public:
	ADXL345(unsigned int i2cBusNum, unsigned int i2cAddr = 0x53);
	virtual ~ADXL345();

	virtual int setRange(RANGE range);
	virtual RANGE getRange() {return this->range;}
	virtual int setResolution(RESOLUTION resolution);
	virtual RESOLUTION getResolution(){return this->resolution;}

	virtual short getAccelerationX(){return this->accelerationX;}
	virtual short getAccelerationY(){return this->accelerationY;}
	virtual short getAccelerationZ(){return this->accelerationZ;}
	virtual float getPitch(){return this->pitch;}
	virtual float getRoll(){return this->roll;}

	virtual int readSensorState();
	virtual void displayPitchAndRoll(int iteration = 600);
	virtual void debugSantityCheck(int numOfRd);
};

#endif /* ADXL345_H_ */







