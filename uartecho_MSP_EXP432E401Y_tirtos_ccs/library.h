#ifndef LIBRARY_H_
#define LIBRARY_H_

#define NUMERROR 4
#define VERSION "4.1"
#define ASSIGNMENT 4
#define AUTHOR "Shelton Foust"
#define MAXLEN 365
#define NUMGPIO 8
#define NUMCALLBACK 4

#define MSG_OVERFLOW 0
#define INVALID_OPERATION 1
#define WRONG_TYPE 2
#define BAD_NUMBER 3
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateSwi.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <stdbool.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs


typedef struct _callback
{
    int index;
    int count;
    bool hwi_flag;
    char payload[MAXLEN];
} Callback;

typedef struct _BiosStruct
{
    Task_Handle uart0ReadTask;
    Task_Handle messageTask;
    Semaphore_Handle uart0ReadSemaphore;
    Swi_Handle timer0Swi;
    GateSwi_Handle callbackGate;
} BiosStruct;

typedef struct _GlobalStruct
{
    int timer0Period;
    int errorCounts[NUMERROR];
    Callback callbacks[NUMCALLBACK];
    BiosStruct bios;
    UART_Handle uart;
    Timer_Handle timer0;
} GlobalStruct;









// Structs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GlobalStruct Glo;



void aboutParse();
void errorParse(char* input);
void errorBase();
void gpioBase();
void helpBase();
void memoryReadParse(char* input);
void printParse(char* input);
void readGPIOprint(int gpio);
void writeGPIOprint(int gpioNum, char* remainder);
void removeSpaces(char* noSpaces, const char* input);
void gpioParse(char* input);
void helpParse(char* input);
int startsWith(char *input, char *commandName);



#endif
