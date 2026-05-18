#include "debugUART.h"
#include "definitions.h"

volatile char printMsgBuff[DEBUG_UART_TX_LEN];

void debugPrint(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(printMsgBuff, sizeof(printMsgBuff), format, args);
    va_end(args);
    
    SERCOM3_USART_Write(printMsgBuff, strlen(printMsgBuff));
    while(SERCOM3_USART_WriteIsBusy());
}

void debugUartInit()
{
    SERCOM3_USART_Enable();
    SERCOM3_USART_TransmitterEnable();
    SERCOM3_USART_ReceiverEnable();
}