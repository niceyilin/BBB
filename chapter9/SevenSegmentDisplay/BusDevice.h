/*
 * BusDevice.h
 *
 *  Created on: Jul 1, 2016
 *      Author: yilin
 */

#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

#include <string>

using namespace std;

/* General class for bus device, could be inherited to be any specific bus device
 * including I2C device and  SPI device
 */
class BusDevice{
private:
	unsigned int  busNum;
	unsigned int  devNum;

public:
	BusDevice(unsigned int busNum, unsigned int devNum);
	virtual ~BusDevice(){};

	virtual int open() = 0;
	virtual void close() = 0;

	virtual int write(unsigned char value) = 0;
	virtual int writeRegister(unsigned int address, unsigned char value) = 0;

	virtual unsigned char readRegister(unsigned int regAddr) = 0;
	virtual unsigned char* readRegisters(unsigned int numOfRd, unsigned int fromAddr) = 0;

	virtual void debugDumpRegisters(unsigned int numOfDump) = 0;
	virtual string dec2hex(int dec);

};



#endif /* BUSDEVICE_H_ */
