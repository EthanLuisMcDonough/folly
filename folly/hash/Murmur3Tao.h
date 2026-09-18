//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#pragma once

//-----------------------------------------------------------------------------

#include <cstdint>
#include <cstddef>

//-----------------------------------------------------------------------------

namespace folly {
namespace hash {

std::uint32_t MurmurHash3_Tao(const void *key, std::size_t length);

} // namespace folly
} // namespace hash

//-----------------------------------------------------------------------------
