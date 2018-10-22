#ifndef SYSTEM_H
#define SYSTEM_H

void system_init(void);
void system_wdt(void);

void set_relay_on(void);
void set_relay_off(void);
uint8_t relay_state(void);

#endif
