#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"

/* Serial stream writer. */
static FILE serial_out = FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);

/* Initialize the UART registers. */
void serial_init(void)
{
  /* Baudrate setup. */
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  /* Enable USART and set frame format. */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  /* Assign stdout as serial output. */
  stdout = &serial_out;
}

/* Send a single USART frame. */
int serial_putchar(char data, FILE *stream)
{
  (void)stream;
  /* Wait until there is data in the buffer. */
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data;
  /* Wait for the data to shift out the register. */
  loop_until_bit_is_set(UCSR0A, TXC0);
  return 0;
}

/* Returns the received USART frame. */
char serial_getchar(void)
{
  loop_until_bit_is_set(UCSR0A, RXC0);
  return UDR0;
}
