#ifndef SWITCH_H
#define SWITCH_H

#define SW_STATE_OFF    0
#define SW_STATE_ON     1

void switch_handle(void);
void switch_update_state(uint8_t state);

#endif // SWITCH_H
