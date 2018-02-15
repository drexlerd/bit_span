#pragma once

#include <tudocomp/ds/dynamic_t.hpp>
#include <tudocomp/ds/uint_t.hpp>

namespace tdc {

template<typename T>
struct maybe_bit_packed_width_trait{
    class type {
    public:
        constexpr type(uint8_t) {}
        constexpr uint8_t get_width() const {
            return sizeof(T) * CHAR_BIT;
        }
        constexpr bool needs_alignment() const {
            return true;
        }
    };
};

template<size_t N>
struct maybe_bit_packed_width_trait<uint_impl_t<N>>{
    class type {
    public:
        constexpr type(uint8_t) {}
        constexpr uint8_t get_width() const {
            return N;
        }
        constexpr bool needs_alignment() const {
            return false;
        }
    };
};

template<>
struct maybe_bit_packed_width_trait<dynamic_t>{
    class type {
        uint8_t m_width;
    public:
        constexpr type(uint8_t w): m_width(w) {}
        constexpr uint8_t get_width() const {
            return m_width;
        }
        constexpr bool needs_alignment() const {
            return false;
        }
    };
};

template<typename T>
using maybe_bit_packed_width_t = typename maybe_bit_packed_width_trait<T>::type;

}
