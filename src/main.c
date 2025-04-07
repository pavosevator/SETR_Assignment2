/* ******************************************************/
/* Compile with: gcc cmdproc.c main.c -o main           */
/*	Feel free to use flags such as -Wall -Wpedantic ...	*/
/*	and it is a good idea to create a makefile			*/
/*                                                      */
/* ******************************************************/
#include <stdio.h>
#include <string.h>
#include "cmdproc.h"


int main(void) 
{	
	int i,len, err;
	unsigned char ans[30]; 
	unsigned char ansTest1[]={'#','p','t', '+', '2', '1', '1', '1', '4','!'};
	
	printf("\n Smart Sensor interface emulation \n");
	printf(" \t - simple illustration of interface and use \n\n\r");
	
	/* Init UART RX and TX buffers */
	resetTxBuffer();
	resetRxBuffer();
	
	/* Test 1 */
	
	printf("Test1 - check the answer to a valid Pt command\n");
	
	/* 1 - send the command */
	rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('6');
	rxChar('5');
	rxChar('!');
			
	/* 2 - Process the comand and check the answer */
	
	int a = cmdProcessor();
	printf("Answer is %d\n", a);

	if(cmdProcessor() == 0){
		printf("Test 1 succeeded\n");
		getTxBuffer(ans,&len);
		printf("Answer is %s\n", ans);
	}
	

	
	if(memcmp(ans,ansTest1,len)) {
		printf("Test 1 failed\n");
	} else {
		printf("Test 1 succeeded\n");
	}	
	
	/* You can print the answer to see what is wrong, if necessary */
	printf("\t Received answer:");
	for(i=0; i < len; i++) {
		printf("%c", ans[i]);
	}
	printf("\n\t Expected answer:");
	i=sizeof(ansTest1);
	for(i=0; i< len; i++) {
		printf("%c", ansTest1[i]);
	}
	printf("\n");
	
	
	/* Test 2 */
	
	printf("Test2 - check the answer to a transmission omission/error \n");
	
	/* 1 - send the command */
	rxChar('#');
	rxChar('P');
	// rxChar('t'); - simulates missing character, emulates a tx error 
	rxChar('1');
	rxChar('9');
	rxChar('6');
	rxChar('!');
			
	/* 2 - Process the comand and check the answer */
	
	err=cmdProcessor();
		
	if(err == -2) {
		printf("Test 2 succeeded, as omission was detected\n");
	} else {
		printf("Test 2 failed, as omission was not detected\n");
	}		
	
	/* Much more tests are needed. Unity shoul be used for it. */
	
	return 0;
}