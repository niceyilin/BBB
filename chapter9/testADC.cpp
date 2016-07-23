/*
 * testADC.cpp
 *
 *  Created on: Jul 23, 2016
 *      Author: yilin
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <unistd.h>

using namespace std;

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/"
#define READ_PERIOD 10000


int readADC(int adcNumber){
	stringstream ss;
	int adcVal;
	ss << ADC_PATH << "in_voltage" << adcNumber << "_raw";
	fstream fs;
	//cout << ss.str() << endl; // for debug
	fs.open(ss.str().c_str(), fstream::in);
	fs >> adcVal;
	fs.close();
	return adcVal;
}

int main(int argc, char *argv[]){
	int adcVal, i;
	float distanceVal;
	int adcNumber;
	int numOfRead;
	if(argc != 3){
		cerr << "Error : Right cmd should be ./progrma ADC-NUMBER TIMES-of-READ !" << endl;
		return 1;
	}
	else{
		istringstream ss(argv[1]);
		ss >> adcNumber;
		if((adcNumber<0)||(adcNumber>7)){
			cerr << "Error: invalid ADC number : " << adcNumber << endl;
		}
		else{
			cout << "Read ADC " << adcNumber << endl;
		}
		istringstream ss2;
		ss2.str(argv[2]);
		ss2 >> numOfRead;
		if(numOfRead <= 0){
					cerr << "Error: invalid number of read : " << numOfRead << endl;
		}
		else{
			cout << "Read ADC " << numOfRead << " times: " << endl;
		}
	}

	for(i=0; i<numOfRead; i++){
		adcVal = readADC(adcNumber);
		distanceVal = 115.804f * exp(-0.000843107f * float(adcVal));
		cout << " \tADC = " << adcVal << "/4095";
		cout << "\t distance = " << distanceVal << " cm" << endl;
		//cout << " = " << adcVal << "/4095   " << '/r' << flush;
		usleep(READ_PERIOD); // CAUTION !  unistd header
	}
	return 0;
}
