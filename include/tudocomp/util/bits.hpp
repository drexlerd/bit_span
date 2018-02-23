#pragma once

#include <cstdint>

#include "sdsl_bits.hpp"

namespace tdc {
    using sdsl_bits = tdc_sdsl::bits;

    /// \brief Computes the highest set bit in an integer variable
    inline constexpr uint_fast8_t bits_hi(uint64_t x) {
        return x == 0 ? 0 : 64 - __builtin_clzll(x);
    }

    /// \brief Computes the number of bits required to store the given integer
    /// value.
    ///
    /// This is equivalent to the binary logarithm rounded up to the next integer.
    ///
    /// Examples:
    /// - `bits_for(0b0) == 1`
    /// - `bits_for(0b1) == 1`
    /// - `bits_for(0b10) == 2`
    /// - `bits_for(0b11) == 2`
    /// - `bits_for(0b100) == 3`
    ///
    /// \param n The integer to be stored.
    /// \return The amount of bits required to store the value (guaranteed to be
    /// greater than zero).
    inline constexpr uint_fast8_t bits_for(size_t n) {
        return n == 0 ? 1U : bits_hi(n);
    }

    inline void fast_move_right(const uint64_t*& word, uint8_t& offset, const uint8_t len)
    {
        offset += len;
        word += (offset / 64); // advance pointer if offset got greater than 63
        offset &= 0x3F;        // bound offset to mod 64
    }

    inline void fast_move_left(const uint64_t*& word, uint8_t& offset, const uint8_t len)
    {
        offset -= len;
        word -= (int8_t(offset) < 0); // advance pointer if offset got less than 0
        offset &= 0x3F;               // bound offset to mod 64
    }
}
