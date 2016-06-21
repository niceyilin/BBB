/*
 * GPIO.h
 *
 *  Created on: Jun 11, 2016
 *      Author: yilin
 */

#ifndef GPIO_H_
#define GPIO_H_
#include<string>
#include<fstream>
#include <fcntl.h>
#include <unistd.h>

using std::string;
using namespace std;

#define GPIO_PATH "/sys/class/gpio/"

enum GPIO_DIRECTION {INPUT, OUTPUT};
enum GPIO_VALUE {HIGH=1, LOW=0};
enum GPIO_EDGE_TYPE {none, rising, falling, both};

class GPIO{

private: // variable
	int gpioIndex;
	int togglePeriod;
	string gpioPath;
	ofstream valStream;
	int (*callbackFunc)(int);
private: // method
	int exportGPIO();
	int unexportGPIO();
	int writeGPIO(string path, string filename, int value); // write a number into a file
	int writeGPIO(string path, string filename, string value); //function overload, write a string into a file
	string readGPIO(string path, string filename);
public:
	GPIO(int gpioIndex); //constructor, export that GPIO
	virtual ~GPIO(); //destructor, unexport that GPIO
	virtual void printDebugInfo();

	virtual int setDirection(GPIO_DIRECTION);
	virtual GPIO_DIRECTION getDirection();

	virtual int setValue(GPIO_VALUE);
	virtual GPIO_VALUE getValue();

	virtual int toggleGPIO(int delay); //toggle period/2  , in microseconds format

	virtual int streamOpen();
	//virtual int streamWrite(string path, string filename, int value);
	virtual int streamClose();
	virtual int streamToggleGPIO(int period);

	// epoll related
	virtual int setEdgeType(GPIO_EDGE_TYPE edgeType);
	virtual int waitForEdge();

	virtual int pthreadWait(int (*callback)(int), int togglePeriod);
	friend void* threadedPoll(void *value);
};

void * threadedPoll(void * value);


#endif /* GPIO_H_ */














