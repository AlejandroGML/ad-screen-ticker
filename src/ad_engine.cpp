#include "ad_engine.hpp"

namespace {

constexpr uint32_t kFallbackSeed = 0xA5A5A5A5UL;
constexpr AdMessage kFallbackMessage = {"No ad available", MessageEffect::Plain};

} // namespace

AdEngine::AdEngine(const Advertiser* advertisers,
                   uint8_t advertiser_count,
                   uint32_t total_paid_amount,
                   uint32_t initial_seed) noexcept
    : advertisers_(advertisers),
      advertiser_count_(advertiser_count),
      total_paid_amount_(total_paid_amount),
      random_state_(initial_seed == 0U ? kFallbackSeed : initial_seed),
      next_message_index_{} {
    if ((advertisers_ == nullptr) || (advertiser_count_ == 0U)) {
        advertiser_count_ = 0U;
        total_paid_amount_ = 0U;
    } else if (advertiser_count_ > kMaxAdvertisers) {
        advertiser_count_ = kMaxAdvertisers;
    }

    if (advertiser_count_ > 0U) {
        uint32_t computed_total = 0U;
        for (uint8_t i = 0; i < advertiser_count_; ++i) {
            computed_total += advertisers_[i].paid_amount;
        }
        total_paid_amount_ = computed_total;
    }

    for (uint8_t i = 0; i < kMaxAdvertisers; ++i) {
        next_message_index_[i] = 0U;
    }
}

void AdEngine::reseed(uint32_t seed) noexcept {
    random_state_ = (seed == 0U) ? kFallbackSeed : seed;
}

uint32_t AdEngine::next_random() noexcept {
    random_state_ = (1664525UL * random_state_) + 1013904223UL;
    return random_state_;
}

uint8_t AdEngine::pick_next_advertiser() noexcept {
    if ((advertisers_ == nullptr) || (advertiser_count_ == 0U) || (total_paid_amount_ == 0U)) {
        return 0U;
    }

    const uint32_t ticket = next_random() % total_paid_amount_;
    uint32_t cumulative = 0U;

    for (uint8_t i = 0; i < advertiser_count_; ++i) {
        cumulative += advertisers_[i].paid_amount;
        if (ticket < cumulative) {
            return i;
        }
    }

    return static_cast<uint8_t>(advertiser_count_ - 1U);
}

const AdMessage& AdEngine::next_message_for(uint8_t advertiser_index) noexcept {
    if ((advertisers_ == nullptr) || (advertiser_index >= advertiser_count_)) {
        return kFallbackMessage;
    }

    const Advertiser& advertiser = advertisers_[advertiser_index];
    if ((advertiser.messages == nullptr) || (advertiser.message_count == 0U)) {
        return kFallbackMessage;
    }

    uint8_t message_index = next_message_index_[advertiser_index];
    if (message_index >= advertiser.message_count) {
        message_index = 0U;
    }

    const AdMessage& selected = advertiser.messages[message_index];

    uint8_t next_index = static_cast<uint8_t>(message_index + 1U);
    if (next_index >= advertiser.message_count) {
        next_index = 0U;
    }

    next_message_index_[advertiser_index] = next_index;
    if (selected.text == nullptr) {
        return kFallbackMessage;
    }

    return selected;
}