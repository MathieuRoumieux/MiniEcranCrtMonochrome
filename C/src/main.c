#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "spi.h"
#include "debugUART.h"
#include "usbUART.h"
#include "rotary.h"
#include "definitions.h"

int main(void)
{
    SYS_Initialize(NULL);
    spiSetup();
    debugUartInit();
    usbUartInit();
    irqInit();
    unblankScreen();
    
    debugPrint("RST\r\n");
    
    while(true)
    {
        SYS_Tasks();
        usbStateMachine();
        handleDacAndBlank();
        handleRotaryEncoder();
    }
    return(EXIT_FAILURE);
}