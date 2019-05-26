#ifndef SWITCH_H
#define SWITCH_H

#define SW_STATE_OFF    0
#define SW_STATE_ON     1

#define SW_COLOR_ON 0x01
#define SW_COLOR_OFF 0x02

void switch_handle(void);
void switch_update_state(uint8_t state);

uint8_t switch_current_state(void);
void switch_set_color(uint8_t type, struct ws2812_color *color);

#endif // SWITCH_H
