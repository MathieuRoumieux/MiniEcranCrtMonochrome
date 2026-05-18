#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef ROTARY_H
#define	ROTARY_H

#ifdef	__cplusplus
extern "C" {
#endif

extern volatile bool rotaryStatus;
extern volatile uint8_t rotaryNumber;

void rotaryCallback(uintptr_t context);
void handleRotaryEncoder();
void irqInit();

#ifdef	__cplusplus
}
#endif

#endif	/* ROTARY_H */

