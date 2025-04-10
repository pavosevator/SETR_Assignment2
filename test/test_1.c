#include "unity.h"
#include "cmdproc.h"

//Configuration
void setUp(void) {
    // Initialisation
    rxBufLen = 0;
    txBufLen = 0;
    memset(UARTRxBuffer, '0', UART_RX_SIZE);
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
    TEST_ASSERT_EQUAL(CMD_BUFFER_FULL, rxChar('A'));
}

// Test function txChar
void test_txChar_ShouldAddCharToBuffer(void) {
    int result = txChar('#');
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, txBufLen);
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
}

// Test function txChar buffer full
void test_txChar_ShouldReturnErrorWhenBufferFull(void) {
    for (int i = 0; i < UART_TX_SIZE; i++) {
        txChar('A');
    }
    TEST_ASSERT_EQUAL(CMD_BUFFER_FULL, txChar('A'));
}

// Test function resetRxBuffer
void test_resetRxBuffer_ShouldResetRxBuffer(void) {
    rxChar('A');
    resetRxBuffer();
    TEST_ASSERT_EQUAL(0, rxBufLen);
    TEST_ASSERT_EQUAL('0', UARTRxBuffer[0]);
}

// Test function resetTxBuffer
void test_resetTxBuffer_ShouldResetTxBuffer(void) {
    txChar('A');
    resetTxBuffer();
    TEST_ASSERT_EQUAL(0, txBufLen);
    TEST_ASSERT_EQUAL('0', UARTRxBuffer[0]);
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

// Test empty string
void test_checkRxChecksum_ShouldReportEmptyString(void) {
    TEST_ASSERT_EQUAL(CMD_EMPTY_STRING, cmdProcessor());
    rxChar('#');
    resetRxBuffer();
    TEST_ASSERT_EQUAL(CMD_EMPTY_STRING, cmdProcessor());
}

// Test that command is missing start of frame symbol
void test_checkCommand_ShouldReportMissingSof(void) {
    rxChar('A');
    rxChar('!');
    TEST_ASSERT_EQUAL(CMD_MISSING_SOF_ERROR, cmdProcessor());
}

// Test that command is missing start of frame symbol
void test_checkCommand_ShouldReportMissingEof(void) {
    rxChar('#');
    rxChar('A');
    TEST_ASSERT_EQUAL(CMD_MISSING_EOF_ERROR, cmdProcessor());
}

// Test function checkRxChecksum
void test_checkRxChecksum_ShouldReportWrongChecksum(void) {
    rxChar('#');
    rxChar('A');
    rxChar('0');
    rxChar('0');
    rxChar('0');
    rxChar('!');
    TEST_ASSERT_EQUAL(CMD_CS_ERROR, cmdProcessor());
}

// Test correct checksum but wrong command
void test_checkCommand_ShouldReportWrongCommandWithCorrectChecksum(void) {
    rxChar('#');
    rxChar('B');
    rxChar('0');
    rxChar('6');
    rxChar('6');
    rxChar('!');
    TEST_ASSERT_EQUAL(CMD_INVALID, cmdProcessor());
}

// Test correct checksum but wrong format of the command
void test_checkCommand_ShouldReportInvalidFormatOfFrame(void) {
    rxChar('#');
    rxChar('A');
    rxChar('x');
    rxChar('1');
    rxChar('8');
    rxChar('5'); // 'A' + 'x' = (ASCII) dec. 185, checksum is OK
    rxChar('!');
    TEST_ASSERT_EQUAL(CMD_INVALID, cmdProcessor());
}

// Test command A if it outputs right results
void test_checkCommand_A_ShouldReportCorrectValue(void) {
    printf("%s\n", UARTTxBuffer);
    resetRxBuffer();
    resetTxBuffer();
    rxChar('#');
    rxChar('A');
    rxChar('0');
    rxChar('6');
    rxChar('5'); 
    rxChar('!');
    TEST_ASSERT_EQUAL(CMD_OK, cmdProcessor());

    int len;
	unsigned char ans[30];
    getTxBuffer(ans,&len);
    unsigned char ansTest1[]={'#','a','t', '+', '1', '1', 'h', '0', '8','3', 'c','0','0','9','9','5', '2', '4','2', '!','\0'};

    printf("Evo banke: %s\n",ans);
    printf("Evo banke: %s\n",ansTest1);

    TEST_ASSERT_EQUAL(0, memcmp(ans, ansTest1, 17)); // memcmp returns 0 if strings are equal
}

void test_checkCommand_P_ShouldReportCorrectValue(void) {
    printf("%s\n", UARTTxBuffer);
    resetRxBuffer();
    resetTxBuffer();
    rxChar('#');
	rxChar('P');
	rxChar('t'); 
	rxChar('1');
	rxChar('9');
	rxChar('6');
	rxChar('!');
    TEST_ASSERT_EQUAL(CMD_OK, cmdProcessor());

    int len;
	unsigned char ans[30];
    getTxBuffer(ans,&len);
    unsigned char ansTest1[]={'#','p','t', '+', '1', '1', '!','\0'};

    printf("Evo banke: %s\n",ans);
    printf("Evo banke: %s\n",ansTest1);

    TEST_ASSERT_EQUAL(0, memcmp(ans, ansTest1, 17)); // memcmp returns 0 if strings are equal
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
    RUN_TEST(test_checkRxChecksum_ShouldReportEmptyString);
    RUN_TEST(test_checkCommand_ShouldReportMissingSof);
    RUN_TEST(test_checkCommand_ShouldReportMissingEof);
    RUN_TEST(test_checkRxChecksum_ShouldReportWrongChecksum);
    RUN_TEST(test_checkCommand_ShouldReportWrongCommandWithCorrectChecksum);
    RUN_TEST(test_checkCommand_ShouldReportInvalidFormatOfFrame);
    RUN_TEST(test_checkCommand_A_ShouldReportCorrectValue);
	RUN_TEST(test_checkCommand_P_ShouldReportCorrectValue);
    return UNITY_END();
}
