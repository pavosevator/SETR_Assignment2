#include <unity.h>  /*have to had UNITY*/
#include "cmdproc.h"

void setUp(void) {
    // Initialisation
    static unsigned char rxBufLen = 0;
    static unsigned char txBufLen = 0;
    char stateRx = UART_STATE_IDLE;
    char stateTx = UART_STATE_IDLE;
}

void tearDown(void) {
    
}

// Test commande 'A'
void test_CommandA_ShouldReturnSensorData(void) {
    // Simulation commande 'A'
    rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('6');
	rxChar('5');
	rxChar('!');

    // Call
    int result = cmdProcessor();

    // Verification
    TEST_ASSERT_EQUAL(CMD_OK, result);

    resetRxBuffer();

	rxChar('A');
	rxChar('0');
	rxChar('6');
	rxChar('5');
	rxChar('!');
    
    // Call
    int result = cmdProcessor();

    // Verification
    TEST_ASSERT_EQUAL(-1, result);
    

    // Verification TX
    char txBuffer[UART_TX_SIZE];
    int len;
    getTxBuffer(txBuffer, &len);
    txBuffer[len] = '\0'; // Null-terminate the buffer

    // Verification TX data
    TEST_ASSERT_GREATER_THAN(0, len);

}

//main function to run the test
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_CommandA_ShouldReturnSensorData);
    return UNITY_END();
}
