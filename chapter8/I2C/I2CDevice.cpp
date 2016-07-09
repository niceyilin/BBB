/*
 * I2CDevice.cpp
 *
 *  Created on: Jun 26, 2016
 *      Author: yilin
 */

#include "I2CDevice.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

using namespace std;



/* constructor, take I2C bus number and slave device address
 * open the device to be read/write
 * */
I2CDevice::I2CDevice(int i2cBusNum, int i2cDeviceAddr){
	this->fd = -1; // not opened yet
	this->i2cBusNum = i2cBusNum;
	this->i2cDeviceAddr = i2cDeviceAddr;
	this->open();
}

I2CDevice::~I2CDevice(){
	if(this->fd != -1){
		this->close();
	}
}

int I2CDevice::open(){
	string path;
	if(this->i2cBusNum == 0){
		path = BB_I2C_0;
	}
	else{
		path = BB_I2C_1;
	}
	this->fd = ::open(path.c_str(),O_RDWR);
	if(this->fd < 0){
		perror("Error: fail to open i2c bus file! \n");
		return 1;
	}
	// set the address of I2C slave that you want to interact
	if(ioctl(this->fd, I2C_SLAVE, this->i2cDeviceAddr) < 0){
		perror("Error: fail to connect to i2c bus device! \n");
		return 1;
	}
	return 0;
}

void I2CDevice::close(){
	::close(this->fd);
	this->fd = -1;
}

/* write i2c read-address, to read the content of that address
 * return 0 meaning success, 1 means fail
 */
int I2CDevice::writeAddr(unsigned char address){
	unsigned char buffer[1];
	buffer[0] = address;
	if(::write(this->fd, buffer, 1) != 1){
		perror("Error: fail to write 'read-address' to i2c bus device! \n");
		return 1;
	}
	return 0;
}
/* write "value" to register with "wrAddr"
 * return 0 means success, return 1 means fail
 */
int I2CDevice::writeRegister(unsigned int wrAddr, unsigned char value){
	unsigned char buffer[2];
	buffer[0] = wrAddr;
	buffer[1] = value;
	if(::write(this->fd, buffer, 2) != 2){
		perror("Error: fail to write value to the addressed register on i2c bus device! \n");
		return 1;
	}
	return 0;
}

/* read a register content with the address written by writeAddr()
 * return read content value, return 1 means fail
 */
unsigned char I2CDevice::readRegister(unsigned int rdAddr){
	this->writeAddr(rdAddr);
	unsigned char buffer[1];
	if(::read(this->fd, buffer, 1) != 1){
		perror("Error: fail to read single data from i2c bus device! \n");
		return 1;
	}

	return buffer[0];
}

/* read multiple times of data from "rdAddr"
 * return a pointer point to the first data read from that address
 */
unsigned char* I2CDevice::readRegisters(unsigned int numOfRd, unsigned int rdAddr){
	this->writeAddr(rdAddr);
	unsigned char* buffer = new unsigned char[numOfRd];
	if(::read(this->fd, buffer, numOfRd) != numOfRd) {
		perror("Error: fail to read multiple data from i2c bus device! \n");
		return NULL; // return 1 doesn't pass compile
	}
	//cout << "buffer[0] = " << HEX(buffer[0]) << endl; //for debug
	return buffer;
}

/*read multiple times of data from "rdAddr",
 * printed out in hex format
 */
void I2CDevice::debugDumpReg(unsigned int numOfRd){
	cout << "Dumping I2C device registers for debug purpose: " << endl;
	unsigned char *registers;
	registers = this->readRegisters(numOfRd, 0x00);
	int i=0;
	for(i=0; i<(int)numOfRd; i++){
		cout << HEX(*(registers+i)) << "  ";
		if(i%16 == 15){
			cout << endl;
		}
	}
	cout << endl;
}




