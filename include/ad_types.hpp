#ifndef AD_TYPES_HPP_
#define AD_TYPES_HPP_

#include <stdint.h>

enum class MessageEffect : uint8_t {
    Plain = 0,
    Blink = 1
};

struct AdMessage {
    const char* text;
    MessageEffect effect;
};

struct Advertiser {
    const char* name;
    uint16_t paid_amount;
    const AdMessage* messages;
    uint8_t message_count;
};

#endif