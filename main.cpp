#include <avr/interrupt.h>

#include "ad_catalog.hpp"
#include "ad_engine.hpp"
#include "app_controller.hpp"
#include "lcd_driver.hpp"
#include "millis.h"

int main(void) {
    millis_init();
    sei();

    hd44780 lcd;

    const Advertiser* advertisers = ad_catalog::advertisers();
    const uint8_t advertiser_count = ad_catalog::advertiser_count();
    const uint32_t total_paid = ad_catalog::total_paid_amount();

    uint32_t seed = millis_get();
    if (seed == 0U) {
        seed = 1U;
    }

    AdEngine engine(advertisers, advertiser_count, total_paid, seed);
    AppController app(lcd, engine, advertisers, advertiser_count);

    app.run_forever();

    return 0;
}