/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== mainThread ========
 */

#define VERSION "2.1"
#define ASSIGNMENT 2
#define AUTHOR "Shelton Foust"
#define MSG_LEN 50



void *mainThread(void *arg0)
{

    UART_Handle uart;
    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL)
    {
        /* UART_open() failed */
        while (1);
    }


    char        inputChar;
    char        inputLine[MSG_LEN + 1];
    inputLine[0] = 0;

    int         lineLength = 0;
    while (1)
    {
        UART_read(uart, &inputChar, 1);
        UART_write(uart, &inputChar, 1);

        if (lineLength > MSG_LEN)
        {
            char output[] = "\n\rOVERFLOW\n\r";
            UART_write(uart, &output, strlen(output));
            lineLength = 0;
            inputLine[lineLength] = 0;
        }
        else if (inputChar == '\n' || inputChar == '\r') // Enter pressed
        {
            char output[MSG_LEN];
            sprintf(output, "\n\r");
            UART_write(uart, &output, strlen(output));


            if      (stringStartsWith(inputLine, "-about"))
                aboutPrint(uart);
            else if (stringStartsWith(inputLine, "-help"))
                helpPrint(uart, inputLine);
            else if (stringStartsWith(inputLine, "-print"))
                printPrint(uart, inputLine, strlen(inputLine));
            else // Invalid operation
            {
                char output[MSG_LEN];
                sprintf(output, "Operation \"%s\" not valid.\n\r\n\r", inputLine);
                UART_write(uart, &output, strlen(output));
            }
            lineLength = 0;
            inputLine[lineLength] = 0;

        }
        else if (inputChar == '\b' || inputChar == 127) // Backspace or Delete
        {
            if(lineLength > 0)
            {
                inputLine[lineLength] = 0;
                lineLength--;
            }
        }
        else // Add character to line
        {
            inputLine[lineLength] = inputChar;
            inputLine[lineLength + 1] = 0;
            lineLength++;
        }

    }
}


int stringStartsWith(char *input, char *commandName)
{
    if(strlen(input) >= strlen(commandName))
    {
        char substring[strlen(commandName) + 1];
        strncpy(substring, input, strlen(commandName));
        substring[strlen(commandName)] = 0;

        return (!strcmp(substring, commandName));
    }
    return 0;
}


void aboutPrint(UART_Handle uart)
{
    char output[MSG_LEN];
    sprintf(output, "Author: %s\n\r", AUTHOR);
    UART_write(uart, &output, strlen(output));
    sprintf(output, "Version: %s\n\r", VERSION);
    UART_write(uart, &output, strlen(output));
    sprintf(output, "Assignment: %d\n\r", ASSIGNMENT);
    UART_write(uart, &output, strlen(output));
    sprintf(output, "Compile Timestamp: %s %s\n\r\n\r", __TIME__, __DATE__);
    UART_write(uart, &output, strlen(output));
}


void helpPrint(UART_Handle uart, char* input)
{
    char output[MSG_LEN];
    sprintf(output, "\t%s\n\r", "-about");
    UART_write(uart, &output, strlen(output));
    sprintf(output, "\t%s\n\r\n\r", "-help");
    UART_write(uart, &output, strlen(output));
}

void printPrint(UART_Handle uart, char* inputPtr, int inputLength)
{
    char afterCommand[MSG_LEN];
    char output[MSG_LEN];
    char input[MSG_LEN];
    sprintf(input, "%s", inputPtr);
    input[inputLength] = 0;
    output[0] = 0;
    if (strlen("-print") == strlen(input))
    {
        sprintf(output, "\n\r");
        UART_write(uart, &output, strlen(output));
        return;
    }
    int lenAfterCommand = strlen(input) - strlen("-print");
    strncpy(afterCommand, input + strlen("-print"), lenAfterCommand);
    afterCommand[lenAfterCommand] = 0;
    if (afterCommand[0] == ' ')
    {
        if (strlen(afterCommand) == 1)
        {
            sprintf(output, "\n\r");
            UART_write(uart, &output, strlen(output));
            return;
        }
        char tempString[MSG_LEN];
        strncpy(tempString, afterCommand, strlen(afterCommand));
        strncpy(afterCommand, tempString + 1, strlen(tempString) - 1);
        afterCommand[strlen(tempString) - 1] = 0;
    }
   sprintf(output, "%s\n\r", afterCommand);
   UART_write(uart, &output, strlen(output));
   return;
}

