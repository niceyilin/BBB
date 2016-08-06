//============================================================================
// Name        : SevenSegmentDisplay.cpp
// Author      : yilin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "SevenSegmentDisplay.h"
#include <iostream>
#include <stdio.h>

using namespace std;

const uint8_t SevenSegmentDisplay::symbols[16] = { //(msb) HGFEDCBA (lsb)
	     0b00111111, 0b00000110, 0b01011011, 0b01001111,     //0123
	     0b01100110, 0b01101101, 0b01111101, 0b00000111,     //4567
	     0b01111111, 0b01100111, 0b01110111, 0b01111100,     //89Ab
	     0b00111001, 0b01011110, 0b01111001, 0b01110001      //CdEF
};

SevenSegmentDisplay::SevenSegmentDisplay(SPIDevice *spiBus, size_t numOfDisplays){
	this->spiBus = spiBus;
	this->numOfDisplays = numOfDisplays;
	this->numberBase = 10; // default to decimal display
	this->isCommonAnode = 0; // default to common cathode
	//spiBus->setSpeed(); SPI bus default 8 bits, 500KHz, delay=0, Mode3
}

void SevenSegmentDisplay::writeInt(int number){
	int index = 0;
	uint8_t tobeWrite = 0;
	unsigned int i = 0;
	int remain = number;
	for(i=0; i<this->numOfDisplays; i++){
		index = remain % this->numberBase;
		remain = remain / this->numberBase;
		tobeWrite = this->symbols[index];
		if(this->isCommonAnode){
			tobeWrite = ~tobeWrite;
		}
		cout << "Sending symbol[" << index << "] = " << unsigned(tobeWrite) << endl; // for debug
		this->spiBus->write(tobeWrite);
	}
}
void SevenSegmentDisplay::writeFloat(float number){
	size_t placeOfPoint = this->numOfDisplays;
	float copy = number;
	while(1){
		if((int)copy == copy){
			cout << "break : copy=" << copy << endl;
			break;
		}
		copy = copy * 10;
		placeOfPoint--;
		cout << "copy=" << copy << endl;
	}

	if(placeOfPoint > this->numOfDisplays){
		cerr << "Error : placeOfPoint is invalid!" << endl;
		return;
	}
	uint8_t index = 0;
	uint8_t tobeWrite = 0;
	unsigned int i = 0;
	//int remain = number; // BUG here!!!
	int remain = number * this->numberBase * (this->numOfDisplays - placeOfPoint);
	for(i=0; i<this->numOfDisplays; i++){
		index = remain % this->numberBase;
		remain = remain / this->numberBase;
		tobeWrite = this->symbols[index];
		if(i == (this->numOfDisplays - placeOfPoint)){ // dot
			tobeWrite = (tobeWrite | (1<<7));
		}
		if(this->isCommonAnode){
			tobeWrite = ~tobeWrite;
		}
		cout << "Sending symbol[" << unsigned(index) << "] = " << unsigned(tobeWrite) << endl; // for debug
		this->spiBus->write(tobeWrite);
	}
}

void SevenSegmentDisplay::writeFloat(float number, uint8_t placeOfPoint){
	if(placeOfPoint > this->numOfDisplays){
		cerr << "Error : placeOfPoint is invalid!" << endl;
		return;
	}
	uint8_t index = 0;
	uint8_t tobeWrite = 0;
	unsigned int i = 0;
	//int remain = number; // BUG here!!!
	int remain = number * this->numberBase * (this->numOfDisplays - placeOfPoint);
	for(i=0; i<this->numOfDisplays; i++){
		index = remain % this->numberBase;
		remain = remain / this->numberBase;
		tobeWrite = this->symbols[index];
		if(i == (this->numOfDisplays - placeOfPoint)){ // dot
			tobeWrite = (tobeWrite | (1<<7));
		}
		if(this->isCommonAnode){
			tobeWrite = ~tobeWrite;
		}
		cout << "Sending symbol[" << unsigned(index) << "] = " << unsigned(tobeWrite) << endl; // for debug
		this->spiBus->write(tobeWrite);
	}
}


void SevenSegmentDisplay::setNumberBase(uint8_t numberBase){
	this->numberBase = numberBase;
}

void SevenSegmentDisplay::setCommonAnode(){
	this->isCommonAnode = 0;
}

void SevenSegmentDisplay::setCommonCathode(){
	this->isCommonAnode = 1;
}

SevenSegmentDisplay::~SevenSegmentDisplay(){

}

