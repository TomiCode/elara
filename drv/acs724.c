#include <avr/io.h>
#include <avr/interrupt.h>

#include "../hw_pins.h"

// Initialize the ADC registers.
void acs724_init(void)
{
  ADMUX = _BV(REFS0) | PIN_ADC_SENSOR;
}

// Start the ADC conversion.
void acs724_start(void)
{

}

// ADC Complete interrupt handler.
ISR(ADC_vect)
{

}
