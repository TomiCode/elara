#include <stdio.h>
#include <stdlib.h>

#include <net.h>
#include "../drv/nrf24.h"
#include "../drv/ws2812.h"

#define NSTATE_NOT_CONN 0
#define NSTATE_VALID    1

extern volatile uint8_t sys_status;

static struct ws2812_color net_error_c = { 0x00, 0x3f, 0x00 };

static uint8_t net_state = NSTATE_NOT_CONN;
static uint8_t net_buffer[32] = {0};

static void net_hello(void)
{
    struct net_hello payload = {
        .header = { MSG_HELLO },
        .device = 0xF0
    };

    nrf24_payload_write(&payload);
}

static int net_process(void)
{
    struct net_header *head = (struct net_header *)net_buffer;
    int status;

    status = nrf24_payload_read(net_buffer);
    if (status < 0) {
        printf("unable to read payload. \r\n");
        return status;
    }

    switch (head->type) {
    case MSG_ACQUIRED:
        printf("acquired device! \r\n");
        break;

    default:
        printf("invalid message. %x \r\n", head->type);
    }
}

void net_task(void)
{
    if (net_state == NSTATE_NOT_CONN) {
        net_hello();
    }
}

void net_handle(void)
{

}


