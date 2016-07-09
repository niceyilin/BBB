/*
 * SPIDevice.cpp
 *
 *  Created on: Jul 1, 2016
 *      Author: yilin
 */

#include "SPIDevice.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h> // kernel driver for SPI bus

using namespace std;

// constructor, set default parameter, open spi device
SPIDevice::SPIDevice(unsigned int busNum, unsigned int devNum)
	:BusDevice(busNum, devNum){
		stringstream ss;
		ss << SPI_PATH << busNum << "." << devNum;
		this->filename =string(ss.str());
		this->fd = -1; // file not opened yet
		this->mode = MODE3;
		this->speed = 500000; // default 500kHz
		this->delay = 0;
		this->bits = 8;
		this->open();
	}

// destructor, close spi device
SPIDevice::~SPIDevice(){
	this->close();
}

// Common BusDevice method

/* open specific spi bus dev file
 * return 0 on success, -1 on failure
 */
int SPIDevice::open(){
	this->fd = ::open(this->filename.c_str(), O_RDWR);
	if(this->fd < 0){
		perror("Error: can't open related /dev/spidev file");
		return -1;
	}
	//for debug
	else{
		cout << "Opened file : " << this->filename << endl;
	}
	if(	(this->setBitsPerWord(this->bits) == -1) |
		(this->setMode(this->mode) == -1) |
		(this->setSpeed(this->speed) == -1)){
		perror("Error: can't set SPI configuration properly");
		return -1;
	}
	return 0;
}

void SPIDevice::close(){
	::close(this->fd);
	this->fd = -1;
}


int SPIDevice::write(/*unsigned char*/ uint8_t value){
	unsigned char null_return = 0x00;
	this->transfer(&value, &null_return, 1);
	return 0;
}

int SPIDevice::writeRegister(/*unsigned int*/ size_t address, /*unsigned char*/ uint8_t value){
	unsigned char send_arr[2], receive_arr[2];
	memset(receive_arr, 0, sizeof receive_arr);
	send_arr[0] = (unsigned char)address;
	send_arr[1] = value;
	this->transfer(send_arr, receive_arr, 2);
	return 0;
}

/* need 2 cycles
 * the 1st one, BBB transfer address (combined with WRITE cmd) to sensor
 * the 2nd one, sensor transfer data to BBB
 * return sensor data
 */
unsigned char SPIDevice::readRegister(unsigned int regAddr){
	unsigned char send_data[2], receive_data[2];
	memset(send_data, 0, sizeof(send_data));
	memset(receive_data, 0, sizeof(receive_data));
	send_data[0] = (unsigned char)(regAddr | 0x80); // 7th bit is write/read
	this->transfer(send_data, receive_data, 2);
	return receive_data[1];
}

unsigned char* SPIDevice::readRegisters(unsigned int numOfRd, unsigned int fromAddr){
	unsigned char* data = new unsigned char[numOfRd];
	unsigned char send_data[numOfRd+1], receive_data[numOfRd+1];
	memset(send_data, 0, sizeof(send_data));
	//memset(receive_data, 0, sizeof(receive_data));
	send_data[0] = (unsigned char)(fromAddr | 0x80 | 0x40); // 5th bit is MB multiple read/write
	this->transfer(send_data, receive_data, numOfRd+1);
	memcpy(data, receive_data+1, numOfRd); //throw away receive_data[0]
	return data;
}

void SPIDevice::debugDumpRegisters(unsigned int numOfDump){
	cout << "SPI mode: " << this->mode << endl;
	cout << "Bits per word: " << (int)this->bits << endl;
	cout << "Max speed: " << this->speed << " Hz" << endl;
	cout << "Dumping registers for debug purpose: " << endl;
	unsigned char *registers = this->readRegisters(numOfDump, 0);
	int i = 0;
	for(i=0;i<(int)numOfDump; i++){
		cout << " " << HEX(*(registers+i)) << " ";
		if(i%16==15) cout << endl;
	}
	cout << endl;
}

// SPI specific method
/* let user to configure speed/mode/bitsPerWord
 * other than default value set by constructor
 */
int SPIDevice::setSpeed(uint32_t speed){
	this->speed = speed;
	// below statement call kernel driver to write this->speed to SPI device register
	if(ioctl(this->fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->speed) == -1){
		perror("Error: can't set SPI write speed properly");
		return -1;
	}
	if(ioctl(this->fd, SPI_IOC_RD_MAX_SPEED_HZ, &this->speed) == -1){
		perror("Error: can't set SPI read speed properly");
		return -1;
	}
	return 0;
}


int SPIDevice::setMode(SPI_MODE mode){
	this->mode = mode;
	if(ioctl(this->fd, SPI_IOC_WR_MODE, &this->mode) == -1){
		perror("Error: can't set SPI write mode properly");
		return -1;
	}
	if(ioctl(this->fd, SPI_IOC_RD_MODE, &this->mode) == -1){
		perror("Error: can't set SPI read mode properly");
		return -1;
	}
	return 0;

}

int SPIDevice::setBitsPerWord(uint8_t bits){
	this->bits = bits;
	if(ioctl(this->fd, SPI_IOC_WR_BITS_PER_WORD, &this->bits) == -1){
		perror("Error: can't set SPI write bpw properly");
		return -1;
	}
	if(ioctl(this->fd, SPI_IOC_RD_BITS_PER_WORD, &this->bits) == -1){
		perror("Error: can't set SPI read bpw properly");
		return -1;
	}
	return 0;

}
int SPIDevice::transfer(unsigned char send[], unsigned char receive[], int length){
	struct spi_ioc_transfer transfer;
	transfer.tx_buf = (unsigned long) send;
	transfer.rx_buf = (unsigned long) receive;
	transfer.len = length;
	transfer.speed_hz = this->speed;
	transfer.bits_per_word = this->bits;
	transfer.delay_usecs = this->delay;
	int status;
	// below statement call kernel driver to transfer
	// data(passed from other method) to/from SPI device
	status = ioctl(this->fd, SPI_IOC_MESSAGE(1), &transfer);
	if(status < 0){
		perror("Error: can't transfer SPI send[]&receive[] properly");
		return -1;
	}
	return 0;
}


