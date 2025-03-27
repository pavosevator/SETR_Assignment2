#include <stdio.h>
#include <string.h>

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
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'A': /*  reads the real-time values of the variables provided by the sensor */
				
				/* Check checksum */
				if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
					return -3;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+4] != EOF_SYM) {
					return -4;
				}

				/* Sending of the data */
				/* Emulate pseudo random generation of sensor outputs */
				temp = (signed char)pseudorandom(-50,60);
				hum = (unsigned char)pseudorandom(0,100);
				co2 = (unsigned int)pseudorandom(400,20000);

				/* Store read values in history */
				addTHistory(temp, tHistory, &tHistoryLen);
				addHHistory(hum, hHistory, &hHistoryLen);
				addCHistory(co2, cHistory, &cHistoryLen);
				
				/* Convert values to char */
				


				/* Use txChar func() */


			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
		
				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return -2;
				}
				
				/* Check checksum */
				if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
					return -3;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+6] != EOF_SYM) {
					return -4;
				}
			
				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				txChar('#');
				txChar('p'); /* p is the reply to P 							*/	
				txChar('t'); /* t indicate that it is a temperature 			*/
				txChar('+'); /* This is the sensor reading. You should call a 	*/
				txChar('2'); /*   function that emulates the reading of a 		*/
				txChar('1'); /*   sensor value 	*/
				txChar('1'); /* Checksum is 114 decimal in this case		*/
				txChar('1'); /*   You should call a funcion that computes 	*/
				txChar('4'); /*   the checksum for any command 				*/  
				txChar('!');
				
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

/*
	Separate function for adding values in history
*/
void addInHistory(void *measuredValue, char sensorType) {
	switch (sensorType) {
        case 't': // Temperature
            tHistory[tHistoryLen] = *(signed char *)measuredValue; // Cast to signed char
            tHistoryLen = (tHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            break;

        case 'h': // Humidity
            hHistory[hHistoryLen] = *(unsigned char *)measuredValue; // Cast to unsigned char
            hHistoryLen = (hHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            break;

        case 'c': // CO2
            cHistory[cHistoryLen] = *(unsigned int *)measuredValue; // Cast to unsigned int
            cHistoryLen = (cHistoryLen + 1) % HISTORY_SIZE; // Move to the next position in a circular manner
            break;

        default:
            // Invalid sensor type
            return -1;
    }
}

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
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
	}	
	/* If cmd string full return error */
	return -1;
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
signed int pseudorandom(int min, int max) {
    seed = (25173 * seed + 13849) % 65536; // xn = (a * xn-1 + c) % m, m = 2^16, 
    
	// Scale the result to the desired range
    int range = max - min + 1;
    int scaled = (seed % range) + min;

	return scaled;

}


