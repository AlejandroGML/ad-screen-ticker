#ifndef AD_ENGINE_HPP_
#define AD_ENGINE_HPP_

#include <stdint.h>
#include "ad_types.hpp"

class AdEngine {
public:
    static constexpr uint8_t kMaxAdvertisers = 8;

    AdEngine(const Advertiser* advertisers,
             uint8_t advertiser_count,
             uint32_t total_paid_amount,
             uint32_t initial_seed = 1U) noexcept;

    void reseed(uint32_t seed) noexcept;
    uint8_t pick_next_advertiser() noexcept;
    const AdMessage& next_message_for(uint8_t advertiser_index) noexcept;

private:
    uint32_t next_random() noexcept;

    const Advertiser* advertisers_;
    uint8_t advertiser_count_;
    uint32_t total_paid_amount_;
    uint32_t random_state_;
    uint8_t next_message_index_[kMaxAdvertisers];
};

#endif