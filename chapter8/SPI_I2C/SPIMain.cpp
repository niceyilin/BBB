/*
 * SPIMain.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: yilin
 */

#include "SPIDevice.h"
#include "I2CDevice.h"
#include "ADXL345.h"
#include <iostream>
#include <sstream>

using namespace std;

/*
//Test program for SPIDevice class
int main(){
	// accessing /dev/spidev1.0
	// which is CS0 on SPI BUS 1

	SPIDevice SPIDEVICE(1, 0);
	SPIDEVICE.setSpeed(1000000); //1MHz
	cout << "the SPI device ID is : " << SPIDEVICE.dec2hex(SPIDEVICE.readRegister(0x00)) << endl;
	SPIDEVICE.setMode(SPIDevice::MODE3);
	SPIDEVICE.writeRegister(0x2D, 0x80); //power_ctl
	SPIDEVICE.debugDumpRegisters(0x40);

	return 0;
}
*/

/*Test program for I2CDevice class
int main(){
	// I2C bus 1, slave address 0x53
	I2CDevice i2c(1, 0x53);
	i2c.writeRegister(0x2D, 0x08); // power_ctl
	cout << "the I2C device ID is : " << i2c.dec2hex(i2c.readRegister(0x00)) << endl;
	i2c.debugDumpRegisters(0x40);
	return 0;
}
*/

// Test program for ADXL345 class
int main(int argc, char* argv[]){
	if(argc != 2){
		cerr << "Error: use ./program_name followed by number_of_read !\n";
		return -1;
	}
	int numOfRd;
	istringstream ss(argv[1]);
	if(!(ss >> numOfRd)){
		cerr << "Invalid Number" << argv[1] << endl;
		return -1;
	}

	cout << "Start running ADXL345 sensor..." << endl;

	SPIDevice SPIBUS(1,0);
	I2CDevice I2CBUS(1,0x53);
	ADXL345 ADXLSensor(&SPIBUS);

	ADXLSensor.setRange(ADXL345::PLUSMINUS_4_G);
	cout << "Sensor range is set to: " << ADXLSensor.getRange() << endl;
	ADXLSensor.setResolution(ADXL345::NORMAL);
	cout << "Sensor resolution is set to: " << ADXLSensor.getResolution() << endl;

	ADXLSensor.debugSantityCheck(30);
	cout << "Poll Pitch and Roll from sensor:" << endl;
	ADXLSensor.displayPitchAndRoll(numOfRd);

	return 0;
}










