#ifndef DRV_SERIAL_H
#define DRV_SERIAL_H

/* Serial communication parameters. */
#define BAUD 9600
#define BAUD_TOL 2

#include <stdio.h>
#include <util/setbaud.h>

/* Function prototypes. */
void serial_init(void);
int serial_putchar(char, FILE *stream);
char serial_getchar(void);

#endif // DRV_SERIAL_H
