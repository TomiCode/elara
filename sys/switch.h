#ifndef SWITCH_H
#define SWITCH_H

#define SW_STATE_OFF    0
#define SW_STATE_ON     1

void switch_handle(void);
void switch_update_state(uint8_t state);
uint8_t switch_current_state(void);

#endif // SWITCH_H
