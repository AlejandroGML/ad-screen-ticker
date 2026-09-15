#ifndef AD_CATALOG_HPP_
#define AD_CATALOG_HPP_

#include <stdint.h>
#include "ad_types.hpp"

namespace ad_catalog {

const Advertiser* advertisers() noexcept;
uint8_t advertiser_count() noexcept;
uint32_t total_paid_amount() noexcept;

} // namespace ad_catalog

#endif