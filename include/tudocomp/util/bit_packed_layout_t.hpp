#pragma once

#include "maybe_bit_packed.hpp"
#include <tudocomp/ds/IntVector.hpp>
#include <tudocomp/util/metaprogramming.hpp>
#include <tudocomp/ds/IntRepr.hpp>

namespace tdc {namespace int_vector {

class bit_layout_element_t {
    uint64_t m_bit_offset = 0;
    uint64_t m_bit_size = 0;
    uint64_t m_bit_element_size = 0;

public:
    constexpr bit_layout_element_t() {}
    constexpr bit_layout_element_t(uint64_t bit_offset, uint64_t bit_size, uint64_t element_size):
        m_bit_offset(bit_offset),
        m_bit_size(bit_size),
        m_bit_element_size(element_size) {}

    constexpr uint64_t bit_offset() const { return m_bit_offset; }
    constexpr uint64_t bit_size() const { return m_bit_size; }
    constexpr uint64_t bit_element_size() const { return m_bit_element_size; }
};

/// TODO: This is temporary and can be removed if the int vector implementations
/// get more general
template<typename T, typename X = tdc::void_t<>>
struct maybe_bit_packed_pointer_trait_t {
    using pointer_t = T*;

    static constexpr pointer_t construct_relative_to(uint64_t* base_ptr,
                                                     uint64_t bit_offset,
                                                     uint64_t bit_element_size) {
        DCHECK_EQ(bit_offset % 8, 0);
        DCHECK_EQ(bit_element_size % 8, 0);
        auto offset = bit_offset / 8;
        return reinterpret_cast<T*>(reinterpret_cast<char*>(base_ptr) + offset);
    }
};

template<typename T>
struct maybe_bit_packed_pointer_trait_t<T, tdc::void_t<typename IntRepr<T>::IntPtrBase>> {
    using pointer_t = IntPtr<T>;

    using ptr_base_t = IntPtrBase<pointer_t>;

    static constexpr pointer_t construct_relative_to(uint64_t* base_ptr,
                                                     uint64_t bit_offset,
                                                     uint64_t bit_element_size) {
        // Find the uint64_t in which the int pointer starts
        base_ptr += bit_offset / 64;
        bit_offset = bit_offset % 64;

        auto int_ptr_base = ptr_base_t(base_ptr, bit_offset, bit_element_size);
        auto int_ptr = pointer_t(int_ptr_base);

        return int_ptr;
    }

};

template<typename T>
using maybe_bit_packed_pointer_t = typename maybe_bit_packed_pointer_trait_t<T>::pointer_t;

template<typename T>
class maybe_bit_packed_layout_element_t: public bit_layout_element_t {
public:
    using pointer_t = maybe_bit_packed_pointer_t<T>;

    constexpr pointer_t ptr_relative_to(uint64_t* base_ptr) const {
        return maybe_bit_packed_pointer_trait_t<T>::construct_relative_to(base_ptr, bit_offset(), bit_element_size());
    }

    using bit_layout_element_t::bit_layout_element_t;
    constexpr maybe_bit_packed_layout_element_t(bit_layout_element_t&& base):
        bit_layout_element_t(std::move(base)) {}
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

        return bit_layout_element_t(bit_offset, bit_size, 8ull * byte_size);
    }

    constexpr bit_layout_element_t bit_packed_elements(size_t bit_size, size_t n) {
        uint64_t rbit_offset = m_bit_offset;
        uint64_t rbit_size = bit_size * n;
        m_bit_offset += rbit_size;

        return bit_layout_element_t(rbit_offset, rbit_size, bit_size);
    }

    template<typename T>
    constexpr maybe_bit_packed_layout_element_t<T> aligned_elements(size_t n) {
        return aligned_elements(alignof(T), sizeof(T), n);
    }

    template<typename T>
    constexpr maybe_bit_packed_layout_element_t<T> maybe_bit_packed_elements(size_t n, maybe_bit_packed_width_t<T> const& meta) {
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

}}
