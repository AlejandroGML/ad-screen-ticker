#ifndef APP_CONTROLLER_HPP_
#define APP_CONTROLLER_HPP_

#include <stdint.h>
#include "ad_engine.hpp"
#include "ad_types.hpp"
#include "lcd_driver.hpp"

class AppController {
public:
    AppController(hd44780& lcd,
                  AdEngine& engine,
                  const Advertiser* advertisers,
                  uint8_t advertiser_count) noexcept;

    void run_forever() noexcept;

private:
    void show_company_name(uint8_t advertiser_index) noexcept;
    void show_message(const AdMessage& message) noexcept;

    hd44780& lcd_;
    AdEngine& engine_;
    const Advertiser* advertisers_;
    uint8_t advertiser_count_;
};

#endif