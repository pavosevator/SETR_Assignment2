#include <unity.h>  /*have to had UNITY*/
#include "cmdproc.h"

void setUp(void) {
    // Initialisation
    rxBufLen = 0;
    txBufLen = 0;
    stateRx = UART_STATE_IDLE;
    stateTx = UART_STATE_IDLE;
    memset(UARTRxBuffer, 0, UART_RX_SIZE);
    memset(UARTTxBuffer, 0, UART_TX_SIZE);
}

void tearDown(void) {
    
}

// Test commande 'A'
void test_CommandA_ShouldReturnSensorData(void) {
    // Simulation commande 'A'
    rxChar('A');

    // Call
    int result = cmdProcessor();

    // Verification
    TEST_ASSERT_EQUAL(CMD_OK, result);

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
