#pragma once

#include "maybe_bit_packed.hpp"

namespace tdc {

class bit_layout_element_t {
    uint64_t m_bit_offset = 0;
    uint64_t m_bit_size = 0;

public:
    constexpr bit_layout_element_t(uint64_t bit_offset, uint64_t bit_size):
        m_bit_offset(bit_offset),
        m_bit_size(bit_size) {}

    constexpr uint64_t bit_offset() const { return m_bit_offset; }
    constexpr uint64_t bit_size() const { return m_bit_size; }
};


class bit_layout_t {
    uint64_t m_bit_offset = 0;

public:
    constexpr bit_layout_element_t aligned_elements(size_t align, size_t byte_size, size_t n) {
        static_assert(CHAR_BIT == 8, "sanity check");

        size_t align_val = align * CHAR_BIT;
        size_t align_mask = align_val - 1;

        if ((m_bit_offset & align_mask) != 0) {
            uint64_t align_skip = align_val - (m_bit_offset & align_mask);
            m_bit_offset += align_skip;
        }

        uint64_t bit_offset = m_bit_offset;
        uint64_t bit_size = (8ull * byte_size * n);

        m_bit_offset += bit_size;

        return bit_layout_element_t(bit_offset, bit_size);
    }

    constexpr bit_layout_element_t bit_packed_elements(size_t bit_size, size_t n) {
        uint64_t rbit_offset = m_bit_offset;
        uint64_t rbit_size = bit_size * n;
        m_bit_offset += rbit_size;

        return bit_layout_element_t(rbit_offset, rbit_size);
    }

    template<typename T>
    constexpr bit_layout_element_t aligned_elements(size_t n) {
        return aligned_elements(alignof(T), sizeof(T), n);
    }

    template<typename T>
    constexpr bit_layout_element_t maybe_bit_packed_elements(size_t n, maybe_bit_packed_width_t<T> const& meta) {
        if(meta.needs_alignment()) {
            return aligned_elements<T>(n);
        } else {
            size_t width = meta.get_width();
            return bit_packed_elements(width, n);
        }
    }

    constexpr size_t get_size_in_uint64_t_units() const {
        return (m_bit_offset + 63ull) / 64ull;
    }

};

}
