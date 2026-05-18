#include "rotary.h"
#include "spi.h"
#include "frameRegisters.h"
#include "definitions.h"

volatile bool rotaryStatus = false;
volatile uint8_t rotaryNumber = 0;

void rotaryCallback(uintptr_t context)
{
    EIC_InterruptDisable(EIC_PIN_14);
    rotaryStatus = true;
}

void irqInit()
{
    EIC_CallbackRegister(EIC_PIN_14, rotaryCallback, 0);
    EIC_InterruptEnable(EIC_PIN_14);
}

void handleRotaryEncoder()
{
    if(rotaryStatus == true)
    {
        sigIndex = 0;
        sigFrameIndex = 0;
        sigPeriodIndex = 0;
        
        if(RE2_Get() == 1)
        {
            rotaryNumber = (rotaryNumber+1)%REG_NUMBER;
        }
        else
        {
            rotaryNumber = (rotaryNumber+REG_NUMBER-1)%REG_NUMBER;
        }
        debugPrint("\r\nROTARY NUMBER : %d\r\n", rotaryNumber);
        
        updateDisplayBuffer();
        
        EIC_InterruptEnable(EIC_PIN_14);
        rotaryStatus = false;
    }
}