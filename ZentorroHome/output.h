#ifndef OUTPUT_H
#define OUTPUT_H
#include <Arduino.h>

const char* bin2tristate(const char* bin);
char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);
void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol);


#endif
