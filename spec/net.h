#ifndef NET_SPEC_H
#define NET_SPEC_H

#define MSG_HELLO 0x01
#define MSG_ACQUIRED 0x02
#define MSG_PING 0x03
#define MSG_PONG 0x04
#define MSG_SWITCH 0x05

struct net_header {
    uint8_t type;
    uint16_t device;
};

struct net_hello {
    struct net_header header;
    uint8_t hw_type;
    uint32_t version;
};

struct net_acquire {
    struct net_header header;
    uint8_t addr[5];
};

struct net_ping {
    struct net_header header;
    uint8_t state;
};

struct net_switch {
    struct net_header header;
    uint8_t state;
};

#endif // NET_SPEC_H
