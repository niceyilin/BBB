/*
 ============================================================================
 Name        : testADXL345.c
 Author      : yilin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define DEVID 0x00
#define BUFFER_SIZE 40

int main(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	puts("Starting the ADXL345 test application");
	int file;
	if((file = open("/dev/i2c-1", O_RDWR)) < 0){
		perror("fail to open the bus i2c-1\n");
		return 1;
	}

	if((ioctl(file, I2C_SLAVE, 0x53)) < 0){
		perror("fail to connect to the sensor \n");
		return 1;
	}

	char writeBuffer[1] = {0x00};
	if(write(file, writeBuffer, 1) != 1){
		perror("fail to set the read address\n");
		return 1;
	}
	char readBuffer[BUFFER_SIZE];
	if(read(file, readBuffer, BUFFER_SIZE) != BUFFER_SIZE){
		perror("fail to read to the buffer");
		return 1;
	}

	printf("The device ID is : 0x%02x\n", readBuffer[DEVID]);
	return EXIT_SUCCESS;
}


