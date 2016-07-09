/*
 * I2CDevice.h
 *
 *  Created on: Jun 26, 2016
 *      Author: yilin
 */

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <iostream>

#define BB_I2C_0 "/dev/i2c-0"
#define BB_I2C_1 "/dev/i2c-1"

#define HEX(x) setw(2) << setfill('0') << hex << (int)x //from #include <iomanip>

class I2CDevice{

private:
	int fd;
	int i2cBusNum;
	int i2cDeviceAddr;

public:
	I2CDevice(int i2cBusNum, int i2cDeviceAddr);
	virtual ~I2CDevice();
	virtual int open();
	virtual void close();
	virtual int writeAddr(unsigned char value);
	virtual int writeRegister(unsigned int wrAddr, unsigned char value);
	virtual unsigned char readRegister(unsigned int rdAddr);
	virtual unsigned char* readRegisters(unsigned int numOfRd, unsigned int rdAddr);
	virtual void debugDumpReg(unsigned int numOfRd);
};


#endif /* I2CDEVICE_H_ */
