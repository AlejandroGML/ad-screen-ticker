#include "ad_catalog.hpp"

namespace {

constexpr AdMessage kCoolCarsMessages[] = {
    {"Drive me crazy!", MessageEffect::Plain},
    {"30-day moneyback guarantee", MessageEffect::Plain}
};

constexpr AdMessage kPiePeopleMessages[] = {
    {"Making people float", MessageEffect::Blink},
    {"They are hot and jammy", MessageEffect::Plain}
};

constexpr AdMessage kWashomaniaMessages[] = {
    {"Clean customers happy customers", MessageEffect::Blink}
};

constexpr AdMessage kHolyBurgersMessages[] = {
    {"Free drinks included in menu", MessageEffect::Plain},
    {"Burgermania! 2 for 59 SEK", MessageEffect::Blink},
    {"Badabim... Bada BURGER!", MessageEffect::Blink}
};

constexpr AdMessage kGymboJamesMessages[] = {
    {"No more excuses! Only GAINS!", MessageEffect::Blink},
    {"Pro Gymbono - 14 days free", MessageEffect::Plain}
};

constexpr Advertiser kAdvertisers[] = {
    {"CoolCars LLC", 5000U, kCoolCarsMessages, 2U},
    {"Pie People", 2500U, kPiePeopleMessages, 2U},
    {"Washomania", 4900U, kWashomaniaMessages, 1U},
    {"Holy Burgers", 1750U, kHolyBurgersMessages, 3U},
    {"Gymbo James", 6700U, kGymboJamesMessages, 2U}
};

constexpr uint8_t kAdvertiserCount = static_cast<uint8_t>(sizeof(kAdvertisers) / sizeof(kAdvertisers[0]));
constexpr uint32_t kTotalPaidAmount = 20850U;

} // namespace

namespace ad_catalog {

const Advertiser* advertisers() noexcept {
    return kAdvertisers;
}

uint8_t advertiser_count() noexcept {
    return kAdvertiserCount;
}

uint32_t total_paid_amount() noexcept {
    return kTotalPaidAmount;
}

} // namespace ad_catalog