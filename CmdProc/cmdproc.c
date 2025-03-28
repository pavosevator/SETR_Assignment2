#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cmdproc.h"

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 

/* Used as part of storing history*/
static signed char tHistory[HISTORY_SIZE];
static unsigned char hHistory[HISTORY_SIZE];
static unsigned int cHistory[HISTORY_SIZE];

static unsigned char tHistoryLen = 0;
static unsigned char hHistoryLen = 0;
static unsigned char cHistoryLen = 0;

/* Define seed for use of pseudo random value generator */
static unsigned int seed = 1;

/* UART communication is defined with state machines, for Rx and Tx separately */
static UART_State_t stateRx = UART_STATE_IDLE;
static UART_State_t stateTx = UART_STATE_IDLE;

/* Function implementation */

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char sid;

	/* Measured values: temperature [-50, 60] °C, humidity [0,100] %, C02 [400-20k] ppm*/
	signed char temp;
	unsigned char hum;
	unsigned int co2;

	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'A': /*  reads the real-time values of the variables provided by the sensor */

				/* Sending of the data */
				/* Emulate pseudo random generation of sensor outputs */
				temp = (signed char)psrnd(-50,60);
				hum = (unsigned char)psrnd(0,100);
				co2 = (unsigned int)psrnd(400,20000);

				/* Store read values in history */
				addInHistory(&temp, 't');
				addInHistory(&hum, 'h');
				addInHistory(&co2, 'c');
				
				/* Convert values to char */
				char *tempChar = generateCharArray('t', temp);
				char *humChar = generateCharArray('h', hum);
				char *co2Char = generateCharArray('c', co2);

				int printCounter = 0;
				/* Use txChar func() */
				txChar('#');
				txChar('a');
				txChar('t');
				while(tempChar[printCounter] != '\0') {
					txChar(tempChar[printCounter]);
					printCounter++;
				}
				printCounter = 0;
				txChar('h');
				while(humChar[printCounter] != '\0') {
					txChar(humChar[printCounter]);
					printCounter++;
				}
				printCounter = 0;
				txChar('c');
				while(co2Char[printCounter] != '\0') {
					txChar(co2Char[printCounter]);
					printCounter++;
				}
				printCounter = 0;
				/* Don't forget checksum!*/
				txChar('!');

				return 0;

			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
				char* outputChar;

				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return -2;
				} 

				if(sid == 't'){
					temp = (signed char)psrnd(-50,60);
					addInHistory(&temp, 't');
					outputChar = generateCharArray('t',temp);

				} else if(sid == 'h'){
					hum = (unsigned char)psrnd(0,100);
					addInHistory(&hum, 'h');
					outputChar = generateCharArray('h', hum);

				} else if(sid == 'c'){
					co2 = (unsigned int)psrnd(400,20000);
					addInHistory(&co2, 'c');
					outputChar = generateCharArray('c',co2);
				} 
			
				for(int i = 0; i < strlen(outputChar); i++) {
					txChar(outputChar[i]);
				}
				
				/* Here you should remove the characters that are part of the 		*/
				/* command from the RX buffer. I'm just resetting it, which is not 	*/
				/* a good solution, as a new command could be in progress and		*/
				/* resetting  will generate errors									*/
				rxBufLen = 0;	
				
				return 0;
								
			default:
				/* If code reaches this place, the command is not recognized */
				return -2;				
		}
		
		
	}
	
	/* Cmd string not null and SOF not found */
	return -4;

}

/* Checks if the Rx data is ready for processing*/
int checkRxDataReady(void)
{
	int sofIndex;
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(int j = 0; j < rxBufLen; j++) {	
		if(UARTRxBuffer[j] == SOF_SYM) {
			int sofIndex = j;
			break;
		}
	}
	/* Find index of EOF */
	for(int j = 0; j < rxBufLen; j++) {	
		if(UARTRxBuffer[j] == SOF_SYM) {
			int sofIndex = j;
			break;
		}
	}
	/* See if checksum is valid */

	/* Command is valid */
	
	return sofIndex;

}

/*
	Separate function for adding values in history using circular buffer
*/
int addInHistory(void *measuredValue, char sensorType) 
{
	switch (sensorType) {
        case 't': // Temperature
            tHistory[tHistoryLen] = *(signed char *)measuredValue; // Cast to signed char
            tHistoryLen = (tHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            return 0;

        case 'h': // Humidity
            hHistory[hHistoryLen] = *(unsigned char *)measuredValue; // Cast to unsigned char
            hHistoryLen = (hHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            return 0;

        case 'c': // CO2
            cHistory[cHistoryLen] = *(unsigned int *)measuredValue; // Cast to unsigned int
            cHistoryLen = (cHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            return 0;

        default:
            // Invalid sensor type
            return -1;
    }
}

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) 
{
	/* Here you are supposed to compute the modulo 256 checksum */
	/* of the first n bytes of buf. Then you should convert the */
	/* checksum to ascii (3 digitas/chars) and compare each one */
	/* of these digits/characters to the ones in the RxBuffer,	*/
	/* positions nbytes, nbytes + 1 and nbytes +2. 				*/
	
	/* That is your work to do. In this example I just assume 	*/
	/* that the checksum is always OK.							*/	
	return 1;		
}

/*
 * rxChar
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	stateRx = UART_STATE_ERROR;
	/* Delete all wrong chars in buffer until reaching ! */
	
	return -1;
}

/*
 * txChar
 */
int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return 0;		
	} else {
		/* If cmd string full return error */
		return -1;
	} 
}

/*
 * resetRxBuffer
 */
void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/*
 * getTxBuffer
 */
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}

// Pseudo number generator: Linear Congruential Generator
int psrnd(int min,int max) 
{
    seed = (25173 * seed + 13849) % 65536; // xn = (a * xn-1 + c) % m, m = 2^16, 
    
	// Scale the result to the desired range
    int range = max - min + 1;
    int scaled = (seed % range) + min;

	return scaled;

}

char *generateCharArray(char flag, int value) 
{
    char* tResult; // Static arrays are used, because if its one array, it will be overwritten and only last data will we there
    char* hResult; 
    char* cResult; 
    int i = 0;
	char temp;

	/* Adds sign in case of temperature*/
	if(flag == 't'){
		char sign = (value < 0) ? '-' : '+';
		sprintf(tResult, "%c%02d", sign, abs(value));
		return tResult;	
	} else if(flag == 'h') {
		sprintf(hResult, "%03d", value);
		return hResult;
	} else if(flag == 'c') {
		sprintf(cResult, "%05d", value);
		return cResult;
	}


}
