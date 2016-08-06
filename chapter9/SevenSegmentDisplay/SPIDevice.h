/*
 * SPIDevice.h
 *
 *  Created on: Jul 1, 2016
 *      Author: yilin
 */

#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_

#include "BusDevice.h"
#include <stdint.h>
#include <string>
#include <iomanip>

using namespace std;

#define SPI_PATH "/dev/spidev"
#define HEX(x)  setw(2)<<setfill('0')<<hex<< (int)(x)

class SPIDevice : public BusDevice{
public:
	enum SPI_MODE{
		MODE0 = 0,
		MODE1 = 1,
		MODE2 = 2,
		MODE3 = 3
	};
private:
	SPI_MODE mode;
	uint32_t speed; // max speed of transfer in Hz
	uint8_t bits; // bits per word
	uint16_t delay; // delay of transfer in us
	string filename;
	int fd;

public:
	SPIDevice(unsigned int busNum, unsigned int devNum);
	~SPIDevice();
	// Common BusDevice method
	virtual int open();
	virtual void close();

	//virtual int write(unsigned char value);
	virtual int write(uint8_t value);
	virtual int writeRegister(/*unsigned int*/ size_t address, /*unsigned char*/ uint8_t value);

	virtual unsigned char readRegister(unsigned int regAddr);
	virtual unsigned char* readRegisters(unsigned int numOfRd, unsigned int fromAddr);

	virtual void debugDumpRegisters(unsigned int numOfDump);

	// SPI specific method
	virtual int setSpeed(uint32_t speed);
	virtual int setMode(SPI_MODE mode);
	virtual int setBitsPerWord(uint8_t bits);
	virtual int transfer(/*unsigned char*/ uint8_t send[], /*unsigned char*/ uint8_t receive[], int length);

};



#endif /* SPIDEVICE_H_ */
