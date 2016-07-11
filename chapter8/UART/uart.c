/*
 * uart.c
 *
 *  Created on: Jul 10, 2016
 *      Author: yilin
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		perror("Error: ./exe followed by the a string to be sent out\n");
		return -1;
	}

	int fd;
	fd = open("/dev/ttyO4", O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd < 0){
		perror("Error: can't open UART device\n");
		return -1;
	}

	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	options.c_iflag = IGNPAR | ICRNL;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	//unsigned char transmit[18] = "Hello BeagleBone!";
	int txLen = strlen(argv[1])+1;
	//char transmit[20] = argv[1]; ??
	if(write(fd, argv[1], txLen) < 0){
		perror("Error: fail to write to UART device\n");
		return -1;
	}
	usleep(100000);

	unsigned char receive[100];
	int count = read(fd, (void*)receive, 100);
	if(count < 0){
		perror("Error: fail to read from UART device\n");
		return -1;
	}
	else if (count == 0){
		printf("Nothing read from UART device\n");
		return -1;
	}
	else{
		receive[count] = 0;
		printf("The following was read in [%d] : %s\n", count, receive);
	}
	close(fd);

	return 0;

}













