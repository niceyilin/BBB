/*
 * SevenSegmentDisplay.h
 *
 *  Created on: Jul 31, 2016
 *      Author: yilin
 */

#ifndef SEVENSEGMENTDISPLAY_H_
#define SEVENSEGMENTDISPLAY_H_

#include "SPIDevice.h"
#include <stdint.h> // uint_8

/*
 * configurable parameter:
 * - number base, like hex or decimal
 * - common anode or common cathode
 * - number of display
 */
class SevenSegmentDisplay{
private:
	SPIDevice *spiBus;
	size_t numOfDisplays;
	size_t numberBase;
	bool isCommonAnode;
	// Every 8 bits (1 byte) of serial data can light a SevenSegmentDisplay to display a number
	const static uint8_t symbols[]; // array of bytes

public:
	SevenSegmentDisplay(SPIDevice *spiBus, size_t numOfDisplays);
	virtual void writeInt(int number);
	virtual void writeFloat(float number, uint8_t placeOfPoint);
	virtual void writeFloat(float number);

	virtual void setNumberBase(uint8_t numberBase);
	virtual void setCommonAnode();
	virtual void setCommonCathode();

	virtual ~SevenSegmentDisplay();

};




#endif /* SEVENSEGMENTDISPLAY_H_ */
