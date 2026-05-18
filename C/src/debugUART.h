#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef DEBUGUART_H
#define	DEBUGUART_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DEBUG_UART_TX_LEN 256
    
extern volatile char printMsgBuff[DEBUG_UART_TX_LEN];

void debugPrint(const char* format, ...);
void debugUartInit();

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUGUART_H */

