//============================================================================
// Name        : GPIO.cpp
// Author      : yilin
// Version     :
// Copyright   : Your copyright notice
// Description : implematation of GPIO class method
//============================================================================

#include "GPIO.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <pthread.h>
using namespace std;

#define GPIO_PATH "/sys/class/gpio/"

// Private Method of GPIO class
int GPIO::exportGPIO(){
	return (this->writeGPIO(GPIO_PATH, "export", this->gpioIndex));
}

int GPIO::unexportGPIO(){
	return (this->writeGPIO(GPIO_PATH, "unexport", this->gpioIndex));
}

int GPIO::writeGPIO(string path, string filename, string value){
	ofstream fs;
	fs.open((path + filename).c_str());
	if (!fs.is_open()){
		perror("Error: failed to open file to write!");
	}
	//else{
	//	cout << "Open to write:" << (path + filename) << endl;
	//}
	fs << value;
	fs.close();
	return 0;
}

int GPIO::writeGPIO(string path, string filename, int value){
	stringstream s;
	s << value;
	this->writeGPIO( path, filename, s.str());
	return 0;
}

string GPIO::readGPIO(string path, string filename){
	ifstream fs;
	fs.open((path + filename).c_str());
	if(!fs.is_open()){
		perror("Error: failed to file to read!");
	}
	string context;
	getline(fs, context);
	fs.close();
	return context;
}

// Public Method of GPIO class
GPIO::GPIO(int gpioIndex){
	this->callbackFunc = NULL;
	this->gpioIndex = gpioIndex;
	stringstream s;
	s << GPIO_PATH << "gpio" << gpioIndex << "/"; // eg. /sys/class/gpio/gpio49/
	this->gpioPath = string(s.str());
	this->exportGPIO();
	usleep(250000);
}

GPIO::~GPIO(){
	this->setValue(LOW);
	this->unexportGPIO();
}


int GPIO::setDirection(GPIO_DIRECTION dir){
	switch (dir){
	case INPUT:
		return (this->writeGPIO(this->gpioPath, "direction", "in"));
		break;
	case OUTPUT:
		return (this->writeGPIO(this->gpioPath, "direction", "out"));
		break;
	}
	return -1;
}

GPIO_DIRECTION GPIO::getDirection(){
	string dir;
	GPIO_DIRECTION returnDir = INPUT;
	dir = this->readGPIO(this->gpioPath, "direction");
	if(dir == "in"){
		returnDir = INPUT;
	}
	else if(dir == "out"){
		returnDir = OUTPUT;
	}
	else{
		perror("Error: unknown direction type from getDirection() !");
	}
	return returnDir;
}

int GPIO::setValue(GPIO_VALUE val){
	switch(val){
	case HIGH:
		this->writeGPIO(this->gpioPath, "value", "1");
		break;
	case LOW:
		this->writeGPIO(this->gpioPath, "value", "0");
		break;
	}
	return -1;
}

GPIO_VALUE GPIO::getValue(){
	string val;
	GPIO_VALUE returnVal = LOW;
	val = this->readGPIO(this->gpioPath, "value");
	if(val == "0"){
		returnVal = LOW;
	}
	else if (val == "1"){
		returnVal = HIGH;
	}
	else{
		perror("Error: unknown GPIO_VALUE from getValue()!");
	}
	return returnVal;
}

int GPIO::toggleGPIO(int delay){
	//this->setDirection(OUTPUT);
	//while(1){
		if((bool)(this->getValue())){
			this->setValue(LOW);
		}
		else{
			this->setValue(HIGH);
		}
		usleep(delay); //delay 0.5sec = 500000us
	//}
	return 0;
}

void GPIO::printDebugInfo(){
	cout << "-----debug info of GPIO class-----" << endl;
	cout << "gpioIndex=" << this->gpioIndex << ", gpioPath=" << this->gpioPath << endl;
}


int GPIO::streamOpen(){
	this->valStream.open((this->gpioPath + "value").c_str());
	if(!valStream.is_open()){
		perror("Error: can't open file in streamOpen()");
	}
	return 0;
}

int GPIO::streamClose(){
	this->valStream.close();
	return 0;
}

int GPIO::streamToggleGPIO(int period){
	this->valStream << "1" << std::flush;
	usleep(period/2);
	this->valStream << "0" << std::flush;
	usleep(period/2);
	return 0;
}

int GPIO::setEdgeType(GPIO_EDGE_TYPE edgeType){
	if (edgeType == none){
		this->writeGPIO(this->gpioPath, "edge", "none");
	}
	else if (edgeType == rising){
		this->writeGPIO(this->gpioPath, "edge", "rising");
	}
	else if (edgeType == falling){
		this->writeGPIO(this->gpioPath, "edge", "falling");
	}
	else if (edgeType == both){
		this->writeGPIO(this->gpioPath, "edge", "both");
	}
	else{
		perror("Error: unknown GPIO_EDGE_TYPE type from setEdgeType()");
		return -1;
	}
	return 0;
}

// Monitor /sys/class/gpio/gpioxx/value file descriptor,
// blocking/wait until certain event set by setEdgeType()
int GPIO::waitForEdge(){
	int epollfd = epoll_create(1);

	int client_socket;
	client_socket = open((this->gpioPath + "value").c_str(), O_RDONLY | O_NONBLOCK);
	if (client_socket < 0){
		perror("Error: fail to open file from waitForEdge()");
		return -1;
	}

	static struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
	ev.data.fd = client_socket;

	/* register/add my the file i want to monitor into epoll list
	 epoll_ctl(
		a epoll list container created by epoll_create,
		EPOLL_CTL_ADD, a const command type
		client_socket, the file descriptor that i want to monitor
		&ev); a epoll_event struct that used to config the event
	*/
	int epollCtlRes = epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &ev);
	if (epollCtlRes < 0){
		perror("Error: fail to add epoll file descriptor from waitForEdge()");
		return -1;
	}

	// blocking poll/wait until button is pressed
	int counter = 0;
	while(counter <=1){
		int epollWaitRes = epoll_wait(epollfd, &ev, 1, -1);
		if (epollWaitRes < 0){
			perror("Error: fail to wait epoll event from waitForEdge()");
			return -1;
		}
		else{
			counter++;
		}
	}

	close(client_socket);

	return 0;
}

// Create a new thread to run waitForEdge() and callback function after event
int GPIO::pthreadWait(int (*callbackFunc)(int), int togglePeriod){
	this->callbackFunc = callbackFunc;
	this->togglePeriod = togglePeriod;
	pthread_t buttonThread;
	if(pthread_create(&buttonThread, NULL, &threadedPoll, static_cast<void*>(this))){
		perror("Error: pthread_create failed");
	}
	return 0;
}

void* threadedPoll(void *value){
	GPIO *gpio = static_cast<GPIO*>(value);
	gpio->waitForEdge();
	gpio->callbackFunc(gpio->togglePeriod);
	return 0;
}



















