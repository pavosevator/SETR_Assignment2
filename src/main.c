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
	unsigned char ansTest1[]={'#','a','t', '+', '1', '1', 'h', '0', '8','3', 'c','0','0','9','9','5', '2', '4','2', '!'};
	
	printf("\n Smart Sensor interface emulation \n");
	printf(" \t - simple illustration of interface and use \n\n\r");
	
	/* Init UART RX and TX buffers */
	resetTxBuffer();
	resetRxBuffer();
	
	/* Test 1 */
	
	printf("Test1 - check the answer to a valid A command\n\n");
	
	printf("Expected value of the answers: t = %d\n", 11); 
	printf("Expected value of the answers: h = %d\n", 83); 
	printf("Expected value of the answers: c = %d\n", 995); 
	
	/* 1 - send the command */
	//rxChar('x');
	rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('6');
	rxChar('5');
	rxChar('!');

			
	/* 2 - Process the comand and check the answer */

	if(cmdProcessor() == 0){
		printf("Test 1 succeeded - command processed\n");
		getTxBuffer(ans,&len);
	}
	

	
	if(memcmp(ans,ansTest1,len)) {
		printf("Test 1 failed\n");
	} else {
		printf("Test 1 succeeded - received and expected results are the same\n");
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
	printf("err = %d\n", err);

	if(err == -4) {
		printf("Test 2 succeeded, as omission was detected - checksum was appropriate\n");
	} else {
		printf("Test 2 failed, as omission was not detected - was not appropriate\n");
	}		
	
	
	/* Test 3 */
	
	printf("\nTest3 - check the answer to an invalid command \n");

	/* 1 - send the command */	
	rxChar('#');
	rxChar('x');
	rxChar('1');
	rxChar('2');
	rxChar('0');
	rxChar('!');
		
	/* 2 - Process the comand and check the answer */
	err=cmdProcessor();
	printf("err = %d\n", err);
	
	if(err == -2) {
		printf("Test 3 succeeded, as invalid command was detected\n");
	} else {
		printf("Test 3 failed, as invalid command was not detected\n");
	}
	
	return 0;
}