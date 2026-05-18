#include "usbUART.h"
#include "spi.h"
#include "debugUART.h"
#include "frameRegisters.h"
#include "definitions.h"

volatile bool usbFlag = false;
volatile uint16_t lenBuffer = 0;
volatile uint8_t gifFrameIndex = 0;
volatile char commandBuff[COMMAND_LEN];
volatile char rxBuff[RX_LEN];
static char txBuff[TX_LEN] = {'0'};
volatile COMMAND_STATUS commandStatus = AWAITING_COMMAND;

volatile uint8_t bufferSelect = 0;

void usbReadCallback(uintptr_t context)
{
    usbFlag = true;
}

void usbUartInit()
{
    SERCOM1_USART_ReadCallbackRegister(usbReadCallback, 0);
    SERCOM1_USART_Enable();
    SERCOM1_USART_TransmitterEnable();
    SERCOM1_USART_ReceiverEnable();
    
    SERCOM1_USART_Read(commandBuff, COMMAND_LEN); //arms callback
}

//sends a single byte as an acknowledge command
void sendAck()
{
    SERCOM1_USART_Write(txBuff, TX_LEN);
    while(SERCOM1_USART_WriteIsBusy());
}

void usbStateMachine()
{
    if(usbFlag == true)
    {
        switch(commandStatus){
            case AWAITING_COMMAND:
                uint16_t buff = ((uint16_t)commandBuff[0] << 8) | (uint16_t)commandBuff[1];
                switch(buff){
                    case WRITE_BUFFER_0:
                        //debugPrint("WRITING IN BUFFER 0\r\n");
                        bufferSelect = 0;
                        commandStatus = AWAITING_DATA_LEN;
                        break;
                    case WRITE_BUFFER_1:
                        //debugPrint("WRITING IN BUFFER 1\r\n");
                        bufferSelect = 1;
                        commandStatus = AWAITING_DATA_LEN;
                        break;
                    case WRITE_BUFFER_2:
                        //debugPrint("WRITING IN BUFFER 2\r\n");
                        bufferSelect = 2;
                        commandStatus = AWAITING_DATA_LEN;
                        break;
                    case WRITE_BUFFER_3:
                        //debugPrint("WRITING IN BUFFER 3 (GIF)\r\n");
                        bufferSelect = 3;
                        commandStatus = AWAITING_FRAME_NUMBER;
                        break;
                    case READ_BUFFER_0:
                        debugPrint("READING BUFFER 0\r\n");
                        debugPrint("X DATA : ");
                        for(uint16_t i = 0; i < reg1.len; i++)
                        {
                            debugPrint("%d,", reg1.x[i]);
                        }
                        debugPrint("\r\nY DATA : ");
                        for(uint16_t i = 0; i < reg1.len; i++)
                        {
                            debugPrint("%d,", reg1.y[i]);
                        }
                        break;
                    case READ_BUFFER_1:
                        debugPrint("READING BUFFER 1\r\n");
                        debugPrint("X DATA : ");
                        for(uint16_t i = 0; i < reg2.len; i++)
                        {
                            debugPrint("%d,", reg2.x[i]);
                        }
                        debugPrint("\r\nY DATA : ");
                        for(uint16_t i = 0; i < reg2.len; i++)
                        {
                            debugPrint("%d,", reg2.y[i]);
                        }
                        break;
                    case READ_BUFFER_2:
                        debugPrint("READING BUFFER 2\r\n");
                        debugPrint("X DATA : ");
                        for(uint16_t i = 0; i < reg3.len; i++)
                        {
                            debugPrint("%d,", reg3.x[i]);
                        }
                        debugPrint("\r\nY DATA : ");
                        for(uint16_t i = 0; i < reg3.len; i++)
                        {
                            debugPrint("%d,", reg3.y[i]);
                        }
                        break;
                    /*case READ_BUFFER_3:
                        debugPrint("READING BUFFER 3 (GIF)\r\n");
                        debugPrint("FRAME NB : %d\r\n", reg4.frameNb);
                        for(uint8_t i = 0; i < reg4.frameNb; i++)
                        {
                            debugPrint("FRAME %d\r\n", i);
                            debugPrint("X DATA : ");
                            for(uint16_t j = 0; j < reg4.len[i]; j++)
                            {
                                debugPrint("%d,", reg4.x[i][j]);
                            }
                            debugPrint("\r\nY DATA : ");
                            for(uint16_t j = 0; j < reg4.len[i]; j++)
                            {
                                debugPrint("%d,", reg4.y[i][j]);
                            }
                            debugPrint("\r\n");
                        }
                        break;*/
                    case READ_BUFFER_DISPLAY:
                        debugPrint("READING DISPLAY BUFFER\r\n");
                        debugPrint("X DATA : ");
                        for(uint16_t i = 0; i < regDisplay.len; i++)
                        {
                            debugPrint("%d,", regDisplay.x[i]);
                        }
                        debugPrint("\r\nY DATA : ");
                        for(uint16_t i = 0; i < regDisplay.len; i++)
                        {
                            debugPrint("%d,", regDisplay.y[i]);
                        }
                        break;
                    default:
                        debugPrint("COMMAND NOT RECOGNIZED\r\n");
                        break;  
                }
                break;

            case AWAITING_DATA_LEN:
                switch(bufferSelect){
                    case 0:
                        reg1.len = ((uint16_t)commandBuff[0] << 8) | (uint16_t)commandBuff[1];
                        lenBuffer = reg1.len;
                        if(reg1.len > DATA_ARRAY_LEN)
                        {
                            //debugPrint("ERROR : data length %d > %d\r\n", reg1.len, DATA_ARRAY_LEN);
                            commandStatus = AWAITING_COMMAND;
                        }
                        else
                        {
                            //debugPrint("Data length : %d\r\n", reg1.len);
                            commandStatus = AWAITING_BLANK;
                        }
                        break;
                    case 1:
                        reg2.len = ((uint16_t)commandBuff[0] << 8) | (uint16_t)commandBuff[1];
                        lenBuffer = reg2.len;
                        if(reg2.len > DATA_ARRAY_LEN)
                        {
                            //debugPrint("ERROR : data length %d > %d\r\n", reg2.len, DATA_ARRAY_LEN);
                            commandStatus = AWAITING_COMMAND;
                        }
                        else
                        {
                            //debugPrint("Data length : %d\r\n", reg2.len);
                            commandStatus = AWAITING_BLANK;
                        }
                        break;
                    case 2:
                        reg3.len = ((uint16_t)commandBuff[0] << 8) | (uint16_t)commandBuff[1];
                        lenBuffer = reg3.len;
                        if(reg3.len > DATA_ARRAY_LEN)
                        {
                            //debugPrint("ERROR : data length %d > %d\r\n", reg3.len, DATA_ARRAY_LEN);
                            commandStatus = AWAITING_COMMAND;
                        }
                        else
                        {
                            //debugPrint("Data length : %d\r\n", reg3.len);
                            commandStatus = AWAITING_BLANK;
                        }
                        break;
                    /*case 3:
                        memcpy(reg4.len, rxBuff, 2*reg4.frameNb); //needs lsb first in rxBuff for it to work : reg4.len is uint16_t and rxBuff is a char so uint8_t, hence 2*reg4.frameNb
                        lenBuffer = 0;
                        for(uint8_t i = 0; i < reg4.frameNb; i++)
                        {
                            //debugPrint("Frame %d has data length %d\r\n", i, reg4.len[i]);
                            if(reg4.len[i] > DATA_ARRAY_LEN)
                            {
                                //debugPrint("ERROR : frame %d has data length %d > %d\r\n", i, reg4.len[i], DATA_ARRAY_LEN);
                                commandStatus = AWAITING_COMMAND;
                                break;
                            }
                            lenBuffer += reg4.len[i];
                        }
                        //debugPrint("Total length : %d\r\n", lenBuffer);
                        commandStatus = AWAITING_BLANK;
                        break;*/
                    default:
                        break;
                }
                break;
                
            case AWAITING_BLANK:
                switch(bufferSelect){
                    case 0:
                        memcpy(reg1.blank, rxBuff, reg1.len);
                        commandStatus = AWAITING_X_DATA;
                        break;
                    case 1:
                        memcpy(reg2.blank, rxBuff, reg2.len);
                        commandStatus = AWAITING_X_DATA;
                        break;
                    case 2:
                        memcpy(reg3.blank, rxBuff, reg3.len);
                        commandStatus = AWAITING_X_DATA;
                        break;
                    /*case 3:
                        memcpy(&reg4.blank[gifFrameIndex][0], rxBuff, reg4.len[gifFrameIndex]);
                        gifFrameIndex++;
                        if(gifFrameIndex >= reg4.frameNb)
                        {
                            gifFrameIndex = 0;
                            commandStatus = AWAITING_X_DATA;
                        }
                        break;*/
                    default:
                        break;
                }
                break;

            case AWAITING_X_DATA:
                switch(bufferSelect){
                    case 0:
                        memcpy(reg1.x, rxBuff, reg1.len);
                        commandStatus = AWAITING_Y_DATA;
                        break;
                    case 1:
                        memcpy(reg2.x, rxBuff, reg2.len);
                        commandStatus = AWAITING_Y_DATA;
                        break;
                    case 2:
                        memcpy(reg3.x, rxBuff, reg3.len);
                        commandStatus = AWAITING_Y_DATA;
                        break;
                    /*case 3:
                        memcpy(&reg4.x[gifFrameIndex][0], rxBuff, reg4.len[gifFrameIndex]);
                        gifFrameIndex++;
                        if(gifFrameIndex >= reg4.frameNb)
                        {
                            gifFrameIndex = 0;
                            commandStatus = AWAITING_Y_DATA;
                        }
                        break;*/
                    default:
                        break;
                }
                break;

            case AWAITING_Y_DATA:
                switch(bufferSelect){
                    case 0:
                        memcpy(reg1.y, rxBuff, reg1.len);
                        updateDisplayBuffer();
                        commandStatus = AWAITING_COMMAND;
                        break;
                    case 1:
                        memcpy(reg2.y, rxBuff, reg2.len);
                        updateDisplayBuffer();
                        commandStatus = AWAITING_COMMAND;
                        break;
                    case 2:
                        memcpy(reg3.y, rxBuff, reg3.len);
                        updateDisplayBuffer();
                        commandStatus = AWAITING_COMMAND;
                        break;
                    /*case 3:
                        memcpy(&reg4.y[gifFrameIndex][0], rxBuff, reg4.len[gifFrameIndex]);
                        gifFrameIndex++;
                        if(gifFrameIndex >= reg4.frameNb)
                        {
                            gifFrameIndex = 0;
                            commandStatus = AWAITING_COMMAND;
                        }
                        break;*/
                    default:
                        break;
                }
                sendAck();
                break;
                
            /*case AWAITING_FRAME_NUMBER:
                reg4.frameNb = ((uint16_t)commandBuff[0]<<8) | (uint16_t)commandBuff[1];
                if(reg4.frameNb > MAX_GIF_FRAMES)
                {
                    //debugPrint("ERROR : frame number %d > %d\r\n", reg4.frameNb, MAX_GIF_FRAMES);
                    commandStatus = AWAITING_COMMAND;
                }
                else
                {
                    //debugPrint("Frame number : %d\r\n", reg4.frameNb);
                    commandStatus = AWAITING_PERIOD_NUMBER;
                }
                break;
            
            case AWAITING_PERIOD_NUMBER:
                reg4.periodNb = ((uint16_t)commandBuff[0]<<8) | (uint16_t)commandBuff[1];
                //debugPrint("Frames displayed %d times\r\n", reg4.periodNb);
                commandStatus = AWAITING_DATA_LEN;
                break;*/
            
            default:
                break;
        }

        //rearms callback
        switch(commandStatus){
            case AWAITING_COMMAND:
                SERCOM1_USART_Read(commandBuff, COMMAND_LEN);
                break;

            case AWAITING_DATA_LEN:                
                sigIndex = 0;
                sigFrameIndex = 0;
                sigPeriodIndex = 0;
                
                //if a gif is to be uploaded, the frame length will be an array of lengths of each individual arrays
                if(bufferSelect == 3)
                {
                    //SERCOM1_USART_Read(rxBuff, 2*reg4.frameNb);
                }
                else
                {
                    SERCOM1_USART_Read(commandBuff, COMMAND_LEN);
                }
                
                sendAck();
                break;
                
            case AWAITING_BLANK:
                if(bufferSelect == 3)
                {
                    //SERCOM1_USART_Read(rxBuff, reg4.len[gifFrameIndex]);
                }
                else
                {
                    SERCOM1_USART_Read(rxBuff, lenBuffer);
                }
                
                sendAck();
                break;

            case AWAITING_X_DATA:
                if(bufferSelect == 3)
                {
                    //SERCOM1_USART_Read(rxBuff, reg4.len[gifFrameIndex]);
                }
                else
                {
                    SERCOM1_USART_Read(rxBuff, lenBuffer);
                }
                
                sendAck();
                break;

            case AWAITING_Y_DATA:
                if(bufferSelect == 3)
                {
                    //SERCOM1_USART_Read(rxBuff, reg4.len[gifFrameIndex]);
                }
                else
                {
                    SERCOM1_USART_Read(rxBuff, lenBuffer);
                }
                
                sendAck();
                break;
                
            /*case AWAITING_FRAME_NUMBER:
                SERCOM1_USART_Read(commandBuff, COMMAND_LEN);
                
                sendAck();
                break;
                
            case AWAITING_PERIOD_NUMBER:
                SERCOM1_USART_Read(commandBuff, COMMAND_LEN);
                
                sendAck();
                break;*/

            default:
                SERCOM1_USART_Read(commandBuff, COMMAND_LEN);
                break;
        }
        usbFlag = false;
    }
}