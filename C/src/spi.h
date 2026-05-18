#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SIG_LEN 355

typedef enum{
    x_min = 1420,
    x_max = 2700,
    x_len = 255,
            
    y_min = 1720,
    y_max = 2380,
    y_len = 255
}CRT_12BIT_BOUNDARY;
    
extern volatile uint16_t sigIndex;
extern volatile uint16_t sigFrameIndex;
extern volatile uint16_t sigPeriodIndex;
extern volatile bool tcCond;
extern volatile bool testvar;
    
void sendSpiData(uint8_t dataX, uint8_t dataY);
void irqCallback(uintptr_t context);
void handleDacAndBlank();
void spiStartCom();
void spiStopCom();
void blankScreen();
void unblankScreen();
void spiSetup();
void updateDisplayBuffer();

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

