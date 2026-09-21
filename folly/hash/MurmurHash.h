/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>

#include <folly/CPortability.h>
#include <folly/lang/Bits.h>
#include <folly/portability/Constexpr.h>

namespace folly {
namespace hash {

namespace detail {

static inline std::uint32_t rotl32 ( std::uint32_t x, std::int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

static FOLLY_ALWAYS_INLINE std::uint32_t getblock32 ( const std::uint32_t * p, int i )
{
  return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

static FOLLY_ALWAYS_INLINE std::uint32_t fmix32 ( std::uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

} // namespace detail

/*
 * Taobench murmur3
 *
 * https://en.wikipedia.org/wiki/MurmurHash
 */
constexpr std::uint64_t murmurHash64(const char* key, std::size_t length, std::uint64_t seed) noexcept {
  const std::uint8_t * data = (const std::uint8_t*)key;
  const int nblocks = length / 4;

  std::uint32_t h1 = seed;

  std::uint32_t c1 = 0xcc9e2d51;
  std::uint32_t c2 = 0x1b873593;

  //----------
  // body

  const std::uint32_t * blocks = (const std::uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
  {
    std::uint32_t k1 = detail::getblock32(blocks,i);

    k1 *= c1;
    k1 = detail::rotl32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = detail::rotl32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const std::uint8_t * tail = (const std::uint8_t*)(data + nblocks*4);

  std::uint32_t k1 = 0;

  switch(length & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = detail::rotl32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= length;

  h1 = detail::fmix32(h1);

  //*(std::uint32_t*)out = h1;
  return h1;
}

} // namespace hash
} // namespace folly
