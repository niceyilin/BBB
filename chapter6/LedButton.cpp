/*
 * LedButton.cpp
 *
 *  Created on: Jun 11, 2016
 *      Author: yilin
 */


#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "GPIO.h"


using namespace std;
#define SLOW_TOGGLE_PERIOD 500000
#define FAST_TOGGLE_PERIOD 0

//-----Global instance---
//GPIO LED(49);
//GPIO Button(115);
GPIO *LED;
GPIO *Button;
GPIO *LED2;
//-----------------------
int fakeToggleLED(int period = 4321){
	cout << "Fake Toggling LED2 at period of" << period << endl;
	//LED->setValue(LOW);
	LED2->streamOpen();
	while(1){
		LED2->streamToggleGPIO(FAST_TOGGLE_PERIOD); //0.05sec
	}
	LED2->streamClose();
	return 0;
}

int main(int argc, char *argv[]){
	cout << "hello world from LedButton.cpp!" << endl;
	if(getuid() != 0){
		cout << "You must run this program as ROOT. Exiting ... " << endl;
		return -1;
	}
	if(argc != 2){
		cout << "Error: usage should be ./LedButton followed by one of below option:" << endl;
		cout << "op1\nop2\nop3\nop4\nop5" << endl;
		return -1;
	}
	string cmd(argv[1]);

	//GPIO LED(49); // create GPIO index 49 as a output LED.
	//GPIO Button(115); // create GPIO index 115 as a input button
	LED = new GPIO(49);
	Button = new GPIO(115);
	LED2 = new GPIO(48);

	//LED->printDebugInfo();
	//Button->printDebugInfo();
	LED->setDirection(OUTPUT);
	Button->setDirection(INPUT);
	LED2->setDirection(OUTPUT);

    if(cmd == "op1"){
		cout << "=====Op1: Simply keep toggling LED========" << endl;
		while(1){
			LED->toggleGPIO(FAST_TOGGLE_PERIOD);
		}
    }
    else if (cmd == "op2"){
		cout << "=====Op2: main() keep polling button state, and toggle LED while button is pressed, light 5s and die" << endl;
		cout << "Press and hold the button to toggle LED" << endl;
		GPIO_VALUE buttonVal = LOW;
		while(buttonVal == LOW){
			buttonVal = Button->getValue();
		}
		cout << "release the button to stop LED toggling" << endl;
		while(buttonVal == HIGH){
			LED->toggleGPIO(SLOW_TOGGLE_PERIOD); //0.5sec
			buttonVal = Button->getValue();
		}
		LED->setValue(HIGH);
		sleep(5);
		LED->setValue(LOW);
    }
    else if (cmd == "op3"){
		cout << "=====Op3: main() keep polling button state, and toggle LED using stream while button is pressed, light 5s and die" << endl;
		cout << "Press and hold the button to toggle LED" << endl;
		GPIO_VALUE buttonVal = LOW;
		while(buttonVal == LOW){
			buttonVal = Button->getValue();
		}
		cout << "release the button to stop LED toggling" << endl;
		LED->streamOpen();
		while(buttonVal == HIGH){
			LED->streamToggleGPIO(SLOW_TOGGLE_PERIOD); //0.05sec
			buttonVal = Button->getValue();
		}
		LED->streamClose();
		LED->setValue(HIGH);
		sleep(5);
		LED->setValue(LOW);
    }
    else if(cmd == "op4"){
		cout << "=====Op4: use edge & epoll wait for button, toggle LED using stream 10000 times after button is pressed" << endl;
		cout << "Press and hold the button to toggle LED 1000 times very fast" << endl;
		// block and wait for button pressed
		Button->setEdgeType(rising);
		Button->waitForEdge();
		// toggle LED 10 times
		LED->streamOpen();
		for (int i=0; i<10000; i++){
			LED->streamToggleGPIO(FAST_TOGGLE_PERIOD); //0.05sec
		}
		LED->streamClose();
		cout << "press the button to start LED toggling again 100 times slower" << endl;
		Button->setEdgeType(rising);
		Button->waitForEdge();
		LED->streamOpen();
		for (int i=0; i<100; i++){
			LED->streamToggleGPIO(SLOW_TOGGLE_PERIOD); //0.05sec
		}
		LED->streamClose();
    }
    else if (cmd == "op5"){
    	cout << "=====Op5: keep toggling LED1, use another thread & epoll wait for button, and to toggle LED2" << endl;
		Button->setEdgeType(rising);
		// branch out a new thread to poll button state and toggle LED
		Button->pthreadWait(&fakeToggleLED, 1234);

		// in main thread, toggle LED
		LED->streamOpen();
		while(1){
			LED->streamToggleGPIO(FAST_TOGGLE_PERIOD); //0.05sec
		}
		LED->streamClose();
    }
    else{
		cout << "Error: usage should be ./LedButton followed by one of below option ! " << endl;
		cout << "op1\nop2\nop3\nop4\nop5" << endl;
		return -1;
    }
    cout << "Program is done!" << endl;
	return 0;
}








