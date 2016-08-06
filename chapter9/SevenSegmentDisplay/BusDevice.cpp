/*
 * BusDevice.cpp
 *
 *  Created on: Jul 1, 2016
 *      Author: yilin
 */

#include "BusDevice.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

BusDevice::BusDevice(unsigned int busNum, unsigned int devNum){
	this->busNum = busNum;
	this->devNum = devNum;
}

string BusDevice::dec2hex(int dec){
	stringstream ss;
	ss << setw(2) << setfill('0') << hex << int(dec);
	return ss.str();
}


