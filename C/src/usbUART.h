#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef USBUART_H
#define	USBUART_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef enum {
    AWAITING_COMMAND = 0,
    AWAITING_DATA_LEN = 1,
    AWAITING_BLANK = 2,
    AWAITING_X_DATA = 3,
    AWAITING_Y_DATA = 4,
    AWAITING_FRAME_NUMBER = 5,
    AWAITING_PERIOD_NUMBER = 6,
} COMMAND_STATUS;

typedef enum {
    DATA_ARRAY_LEN = 1024,     
    MAX_GIF_FRAMES = 5,
    COMMAND_LEN = 2,
            
    //RX_LEN = DATA_ARRAY_LEN*MAX_GIF_FRAMES,
    RX_LEN = DATA_ARRAY_LEN,
    TX_LEN = 1,
} BUFFER_LENS;

typedef enum{
    WRITE_BUFFER_0 = 0x7730, //"w0"
    WRITE_BUFFER_1 = 0x7731,
    WRITE_BUFFER_2 = 0x7732,
    WRITE_BUFFER_3 = 0x7733,
    READ_BUFFER_0 = 0x7230, //"r0"
    READ_BUFFER_1 = 0x7231,
    READ_BUFFER_2 = 0x7232,
    READ_BUFFER_3 = 0x7233,
    READ_BUFFER_DISPLAY = 0x7264, //"rd"
};
    
extern volatile bool usbFlag;
extern volatile uint16_t lenBuffer;
extern volatile char commandBuff[COMMAND_LEN];
extern volatile char rxBuff[RX_LEN];
extern volatile COMMAND_STATUS commandStatus;

void usbReadCallback(uintptr_t context);
void usbUartInit();
void sendAck();
void usbStateMachine();


#ifdef	__cplusplus
}
#endif

#endif	/* USBUART_H */