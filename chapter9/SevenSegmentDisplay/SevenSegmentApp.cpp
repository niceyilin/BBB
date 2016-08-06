/*
 * SevenSegmentApp.cpp
 *
 *  Created on: Jul 31, 2016
 *      Author: yilin
 */

#include "SevenSegmentDisplay.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <string>
#include <stdlib.h>


using namespace std;

int main(int argc, char *argv[]){
	stringstream ss;
	stringstream ss2;
	int input_number;
	float input_floatnumber;
	string input_format;

	SPIDevice spi(1,0);
	SevenSegmentDisplay Displays(&spi, 2);

	if(argc != 3){
		cerr << "Error: should be used as ./program-name followed by number format and the number to be display\n";
		cerr << " for example : ./SevenSegmentDisplay dec 12 OR hex ff OR float 1.2\n";
		return -1;
	}
	else{
		//cout << "Display " << argv[1] << " on seven segment LED" << endl;
		ss << argv[1];
		ss >> input_format;
		if(input_format == "dec"){
			cout << "input format is decimal\n";
			Displays.setNumberBase(10);
			ss2 << argv[2];
			ss2 >> input_number;
			if((input_number > 100) || (input_number < 0)){
				cerr << "Error: input number " << input_number << " is not valid\n";
				return -1;
			}
			else{
				cout << "Display " << input_number << " on seven segment LED" << endl;
			}
			Displays.writeInt(input_number);
		}
		else if(input_format == "hex"){
			cout << "input format is hex\n";
			Displays.setNumberBase(16);
			ss2 << hex << argv[2];
			input_number = atof(ss2.str().c_str());
			cout << "Display " << input_number << " on seven segment LED" << endl;
			Displays.writeInt(input_number);
		}
		else if (input_format == "float"){
			cout << "input format is float\n";
			Displays.setNumberBase(10);
			ss2 << argv[2];
			ss2 >> input_floatnumber;
			cout << "Display " << input_floatnumber << " on seven segment LED" << endl;
			Displays.writeFloat(input_floatnumber);
		}
		else{
			cerr << "Error: input format is unknown!\n";
			return -1;
		}
	}

	return 0;
}
