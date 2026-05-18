#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "usbUART.h"

#ifndef FRAMEREGISTERS_H
#define	FRAMEREGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define REG_NUMBER 3

typedef struct{
    uint8_t x[DATA_ARRAY_LEN];
    uint8_t y[DATA_ARRAY_LEN];
    uint8_t blank[DATA_ARRAY_LEN];
    uint16_t len;
}frameReg;

typedef struct{
    uint8_t x[MAX_GIF_FRAMES][DATA_ARRAY_LEN];
    uint8_t y[MAX_GIF_FRAMES][DATA_ARRAY_LEN];
    uint8_t blank[MAX_GIF_FRAMES][DATA_ARRAY_LEN];
    uint16_t len[MAX_GIF_FRAMES];
    
    uint16_t frameNb;
    uint16_t periodNb;
}gifReg;

extern volatile frameReg reg1;
extern volatile frameReg reg2;
extern volatile frameReg reg3;
extern volatile frameReg regDisplay;
//extern volatile gifReg reg4;

#ifdef	__cplusplus
}
#endif

#endif	/* FRAMEREGISTERS_H */

