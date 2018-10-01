#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#include "drv/acs724.h"
#include "drv/serial.h"
#include "drv/nrf24.h"

int main(void)
{
  serial_init(BAUD_115200);
  sei(); 
  
  printf("Hello world!\n");

  return 0;
}
