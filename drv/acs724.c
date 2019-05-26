#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "../spec/hardware.h"

extern volatile uint8_t sys_status;

// Initialize the ADC registers.
void acs724_init(void)
{
  ADMUX = _BV(REFS0) | IO_ADC_SENSOR;
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  DIDR0 = _BV(7) | _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
}

// Start the ADC conversion.
int16_t acs724_read(void)
{
  uint16_t result;

  ADCSRA |= _BV(ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);

  result = ADCL;
  result |= (ADCH << 8);
  return (result - 511) * 24;
}

// ADC Complete interrupt handler.
// ISR(ADC_vect)
// {
//   sys_status |= 0;
// }
