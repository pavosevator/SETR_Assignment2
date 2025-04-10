#include "unity.h"
#include "cmdproc.h"

//Configuration
void setUp(void) {
    // Initialisation
    rxBufLen = 0;
    txBufLen = 0;
    memset(UARTRxBuffer, '\0', UART_RX_SIZE);
    memset(UARTTxBuffer, '\0', UART_TX_SIZE);
}

//Clear
void tearDown(void) {
    
}

// Test function rxChar
void test_rxChar_ShouldAddCharToBuffer(void) {
    int result = rxChar('#');
    TEST_ASSERT_EQUAL(1, rxBufLen);
    TEST_ASSERT_EQUAL('#', UARTRxBuffer[0]);
}

// Test function rxChar buffer full
void test_rxChar_ShouldReturnErrorWhenBufferFull(void) {
    for (int i = 0; i < UART_RX_SIZE; i++) {
        rxChar('A');
    }
    TEST_ASSERT_EQUAL(-1, rxChar('A'));
}

// Test function txChar
void test_txChar_ShouldAddCharToBuffer(void) {
    int result = txChar('#');
    TEST_ASSERT_EQUAL(1, txBufLen);
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
}

// Test function txChar buffer full
void test_txChar_ShouldReturnErrorWhenBufferFull(void) {
    for (int i = 0; i < UART_TX_SIZE; i++) {
        txChar('A');
    }
    TEST_ASSERT_EQUAL(-1, txChar('A'));
}

// Test function resetRxBuffer
void test_resetRxBuffer_ShouldResetRxBuffer(void) {
    rxChar('A');
    resetRxBuffer();
    TEST_ASSERT_EQUAL(0, rxBufLen);
}

// Test function resetTxBuffer
void test_resetTxBuffer_ShouldResetTxBuffer(void) {
    txChar('A');
    resetTxBuffer();
    TEST_ASSERT_EQUAL(0, txBufLen);
}

// Test function generateCharArray
void test_generateCharArray_ShouldGenerateCorrectArrayForTemperature(void) {
    char buffer[4];
    generateCharArray('t', 25, buffer);
    TEST_ASSERT_EQUAL_STRING("+25", buffer);
}

// Test function generateCharArray
void test_generateCharArray_ShouldGenerateCorrectArrayForHumidity(void) {
    char buffer[4];
    generateCharArray('h', 50, buffer);
    TEST_ASSERT_EQUAL_STRING("050", buffer);
}

// Test function generateCharArray
void test_generateCharArray_ShouldGenerateCorrectArrayForCO2(void) {
    char buffer[6];
    generateCharArray('c', 1200, buffer);
    TEST_ASSERT_EQUAL_STRING("01200", buffer);
}

// RUN TESTS
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_rxChar_ShouldAddCharToBuffer);
    RUN_TEST(test_rxChar_ShouldReturnErrorWhenBufferFull);
    RUN_TEST(test_txChar_ShouldAddCharToBuffer);
    RUN_TEST(test_txChar_ShouldReturnErrorWhenBufferFull);
    RUN_TEST(test_resetRxBuffer_ShouldResetRxBuffer);
    RUN_TEST(test_resetTxBuffer_ShouldResetTxBuffer);
    RUN_TEST(test_generateCharArray_ShouldGenerateCorrectArrayForTemperature);
    RUN_TEST(test_generateCharArray_ShouldGenerateCorrectArrayForHumidity);
    RUN_TEST(test_generateCharArray_ShouldGenerateCorrectArrayForCO2);
    return UNITY_END();
}
