#ifndef DRV_SERIAL_H
#define DRV_SERIAL_H

#include <stdio.h>

#define BAUD_115200   8

void serial_init(uint16_t);
int serial_putchar(char, FILE *stream);
char serial_getchar(void);

#endif // DRV_SERIAL_H
