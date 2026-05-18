#include "spi.h"
#include "debugUART.h"
#include "usbUART.h"
#include "rotary.h"
#include "frameRegisters.h"
#include "definitions.h"

volatile uint16_t sigIndex = 0;
volatile uint16_t sigFrameIndex = 0;
volatile uint16_t sigPeriodIndex = 0;
volatile bool tcCond = false;
volatile bool testvar = true;

void sendSpiData(uint8_t dataX, uint8_t dataY)
{
    uint16_t dataX16bit = (x_min+(dataX*(x_max-x_min))/x_len);
    uint16_t dataY16bit = (y_min+(dataY*(y_max-y_min))/y_len);
    
    uint8_t MSBx = (uint8_t)(0b00010000 | (dataX16bit >> 8));
    uint8_t LSBx = (uint8_t)(dataX16bit & 0xFF);
    uint8_t MSBy = (uint8_t)(0b10010000 | (dataY16bit >> 8));
    uint8_t LSBy = (uint8_t)(dataY16bit & 0xFF);
    
    static uint8_t spiDataX[2]; 
    static uint8_t spiDataY[2];
    
    spiDataX[0] = MSBx;
    spiDataX[1] = LSBx;
    spiDataY[0] = MSBy;
    spiDataY[1] = LSBy;
    
    spiStartCom();
    SERCOM0_SPI_Write(spiDataX, 2);
    spiStopCom();
    
    spiStartCom();
    SERCOM0_SPI_Write(spiDataY, 2);
    spiStopCom();
}

void tcCallback(uintptr_t context)
{
    tcCond=true;
}

void handleDacAndBlank()
{
    if(tcCond==true)
    {
        //handles gifs
        /*if(rotaryNumber == 3)
        {
            if(sigIndex >= reg4.len[sigFrameIndex])
            {
                sigIndex = 0;
                sigPeriodIndex++;
                if(sigPeriodIndex >= reg4.periodNb)
                {
                    sigPeriodIndex = 0;
                    sigFrameIndex++;
                    if(sigFrameIndex >= reg4.frameNb)
                    {
                        sigFrameIndex = 0;
                    }
                }
            }
            if(reg4.blank[sigFrameIndex][sigIndex] == 1)
            {
                blankScreen();
            }
            else
            {
                unblankScreen();
            }
            sendSpiData(reg4.x[sigFrameIndex][sigIndex], reg4.y[sigFrameIndex][sigIndex]);
            sigIndex++;
        }
        //handles static images
        else
        {
            if(regDisplay.blank[sigIndex] == 1)
            {
                blankScreen();
            }
            else
            {
                unblankScreen();
            }
            sendSpiData(regDisplay.x[sigIndex], regDisplay.y[sigIndex]);
            sigIndex++;
            if(sigIndex >= regDisplay.len)
            {
                sigIndex = 0;
            }
        }*/
        
        //handles static images
        if(regDisplay.blank[sigIndex] == 1)
        {
            blankScreen();
        }
        else
        {
            unblankScreen();
        }
        sendSpiData(regDisplay.x[sigIndex], regDisplay.y[sigIndex]);
        sigIndex++;
        if(sigIndex >= regDisplay.len)
        {
            sigIndex = 0;
        }
        tcCond=false;
    }
}

void updateDisplayBuffer()
{
    switch(rotaryNumber){
        case 0:
            memcpy(regDisplay.x, reg1.x, reg1.len);
            memcpy(regDisplay.y, reg1.y, reg1.len);
            memcpy(regDisplay.blank, reg1.blank, reg1.len);
            regDisplay.len = reg1.len;
            break;
        case 1:
            memcpy(regDisplay.x, reg2.x, reg2.len);
            memcpy(regDisplay.y, reg2.y, reg2.len);
            memcpy(regDisplay.blank, reg2.blank, reg2.len);
            regDisplay.len = reg2.len;
            break;
        case 2:
            memcpy(regDisplay.x, reg3.x, reg3.len);
            memcpy(regDisplay.y, reg3.y, reg3.len);
            memcpy(regDisplay.blank, reg3.blank, reg3.len);
            regDisplay.len = reg3.len;
            break;
        default:
            break;
    }
}

void spiStartCom()
{
    CS_Clear();
}

void spiStopCom()
{
    CS_Set();
}

void blankScreen()
{
    BLANK_Clear();
}

void unblankScreen()
{
    BLANK_Set();
}

void spiSetup()
{
    TC3_TimerCallbackRegister(tcCallback, 0);
    TC3_TimerStart();
    spiStopCom();
}