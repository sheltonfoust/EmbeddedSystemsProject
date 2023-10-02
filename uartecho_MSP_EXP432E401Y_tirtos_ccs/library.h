#ifndef LIBRARY_H_
#define LIBRARY_H_

#define NUMERROR 4
#define VERSION "4.1"
#define ASSIGNMENT 4
#define AUTHOR "Shelton Foust"
#define MAXLEN 365
#define NUMGPIO 8

#define MSG_OVERFLOW 0
#define INVALID_OPERATION 1
#define WRONG_TYPE 2
#define BAD_NUMBER 3
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int errorCounts[NUMERROR];
void aboutParse(UART_Handle uart);
void errorParse(UART_Handle uart, char* input);
void errorBase(UART_Handle uart);
void gpioBase(UART_Handle uart);
void helpBase(UART_Handle uart);
void memoryReadParse(UART_Handle uart, char* input);
void printParse(UART_Handle uart, char* input);
void readGPIOprint(UART_Handle uart, int gpio);
void writeGPIOprint(UART_Handle uart, int gpioNum, char* remainder);
void removeSpaces(char* noSpaces, const char* input);
void gpioParse(UART_Handle uart, char* input);
void helpParse(UART_Handle uart, char* input);
int startsWith(char *input, char *commandName);



#endif
