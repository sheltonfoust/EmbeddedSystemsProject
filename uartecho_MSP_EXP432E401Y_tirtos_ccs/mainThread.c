#include <stdlib.h>
#include "ti_drivers_config.h"

#include "library.h"

void *mainThread(void *arg0)
{

    int error;
    for (error = 0; error < NUMERROR; error++)
    {
        errorCounts[error] = 0;
    }

    UART_Handle uart;
    UART_Params uartParams;

    GPIO_init();
    UART_init();

    GPIO_write(0, 1); // CONFIG_GPIO_0, CONFIG_GPIO_LED_ON
    GPIO_write(1, 1);
    GPIO_write(2, 1);
    GPIO_write(3, 1);
    GPIO_write(4, 1);
    GPIO_write(5, 1);


    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(0, &uartParams); // CONFIG_UART_0, &uartParams

    if (uart == NULL)
    {
        // UART_open() failed
        while (1);
    }




    char        inputChar;
    char        inputLine[MAXLEN + 1];
    inputLine[0] = 0;

    int         lineLength = 0;
    while (1)
    {
        UART_read(uart, &inputChar, 1);
        UART_write(uart, &inputChar, 1);

        if (lineLength > MAXLEN)
        {
            char output[] = "\n\rOVERFLOW\n\r";
            errorCounts[MSG_OVERFLOW]++;
            UART_write(uart, &output, strlen(output));
            lineLength = 0;
            inputLine[lineLength] = 0;
        }
        else if (inputChar == '\n' || inputChar == '\r') // Enter pressed
        {
            char output[MAXLEN];
            sprintf(output, "\n\r");
            UART_write(uart, &output, strlen(output));

            if(strlen(inputLine) == 0)
            {
                // Do nothing if line is empty
            }
            else if (startsWith(inputLine, "-about"))
                aboutParse(uart);
            else if (startsWith(inputLine, "-error"))
                errorParse(uart, inputLine);
            else if (startsWith(inputLine, "-gpio"))
                gpioParse(uart, inputLine);
            else if (startsWith(inputLine, "-help"))
                helpParse(uart, inputLine);
            else if (startsWith(inputLine, "-memr"))
                memoryReadParse(uart, inputLine);
            else if (startsWith(inputLine, "-print"))
                printParse(uart, inputLine);
            else // Invalid operation
            {
                char output[MAXLEN];
                sprintf(output, "Operation \"%s\" not valid.\n\r\n\r", inputLine);
                UART_write(uart, &output, strlen(output));
                errorCounts[INVALID_OPERATION]++;
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
