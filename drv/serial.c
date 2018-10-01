#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"

static FILE serial_out = FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);

// Initialize the UART registers.
void serial_init(uint16_t ubrr)
{
  // Setup the baudrate.
  UBRR0H = ubrr >> 8;
  UBRR0L = ubrr;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // Frame format setup.
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  // Assign stdout as serial output.
  stdout = &serial_out;
}

// Transmit one byte (frame).
int serial_putchar(char data, FILE *stream)
{
  (void)stream;
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data;
  // White until the data was send.
  loop_until_bit_is_set(UCSR0A, TXC0);
  return 0;
}

// Receive one frame.
char serial_getchar(void)
{
  loop_until_bit_is_set(UCSR0A, RXC0);
  return UDR0;
}
