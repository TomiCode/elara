#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"

// Initialize the UART registers.
void serial_init(uint16_t ubrr)
{
  // Setup the baudrate.
  UBRR0H = ubrr >> 8;
  UBRR0L = ubrr;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // Frame format setup.
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

// Transmit one byte (frame).
void serial_send(const uint8_t data)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  // White until the data was send.
  while(!(UCSR0A & _BV(TXC0)));
}

// Receive one frame.
uint8_t serial_recv(void)
{
  while(!(UCSR0A & _BV(RXC0)));
  return UDR0;
}
