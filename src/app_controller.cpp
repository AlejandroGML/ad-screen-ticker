#include "app_controller.hpp"
#include "millis.h"

namespace {

constexpr const char* kFallbackCompanyName = "Unknown company";
constexpr const char* kFallbackMessageText = "Message unavailable";
constexpr millis_t kCompanyNameDurationMs = 5000U;
constexpr millis_t kMessageDurationMs = 10000U;

} // namespace

AppController::AppController(hd44780& lcd,
                             AdEngine& engine,
                             const Advertiser* advertisers,
                             uint8_t advertiser_count) noexcept
    : lcd_(lcd),
      engine_(engine),
      advertisers_(advertisers),
      advertiser_count_(advertiser_count) {
}

void AppController::show_company_name(uint8_t advertiser_index) noexcept {
    if ((advertisers_ == nullptr) || (advertiser_index >= advertiser_count_)) {
        lcd_.write_text(kFallbackCompanyName);
        millis_wait_ms(kCompanyNameDurationMs);
        return;
    }

    const char* name = advertisers_[advertiser_index].name;
    lcd_.write_text(name == nullptr ? kFallbackCompanyName : name);
    millis_wait_ms(kCompanyNameDurationMs);
}

void AppController::show_message(const AdMessage& message) noexcept {
    const char* text = (message.text == nullptr) ? kFallbackMessageText : message.text;

    if (message.effect == MessageEffect::Blink) {
        lcd_.blink_text(text);
        return;
    }

    lcd_.write_text(text);
    millis_wait_ms(kMessageDurationMs);
}

void AppController::run_forever() noexcept {
    if ((advertisers_ == nullptr) || (advertiser_count_ == 0U)) {
        lcd_.write_text("Catalog error");
        while (true) {
            millis_wait_ms(1000U);
        }
    }

    while (true) {
        uint8_t advertiser_index = engine_.pick_next_advertiser();
        if (advertiser_index >= advertiser_count_) {
            advertiser_index = 0U;
        }

        show_company_name(advertiser_index);

        const AdMessage& message = engine_.next_message_for(advertiser_index);
        show_message(message);
    }
}