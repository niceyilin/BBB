/*
 * ADXL345app.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: yilin
 */

#include "I2CDevice.h"
#include "ADXL345.h"
#include <iostream>
#include <stdio.h>
#include <sstream>


using namespace std;

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
	ADXL345 ADXLSensor(1,0x53);
	ADXLSensor.setRange(ADXL345::PLUSMINUS_4_G);
	cout << "Sensor range is set to: " << ADXLSensor.getRange() << endl;
	ADXLSensor.setResolution(ADXL345::NORMAL);
	cout << "Sensor resolution is set to: " << ADXLSensor.getResolution() << endl;
	//ADXLSensor.readSensorState();
	ADXLSensor.debugSantityCheck(30);
	cout << "Poll Pitch and Roll from sensor:" << endl;


	ADXLSensor.displayPitchAndRoll(numOfRd);

	return 0;
}


