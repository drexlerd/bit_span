Bit-Span
========

Bit-Span is a bit-compact integer vector, meaning that it can store
integers with a fixed bit-width `n`, where `n` can range from `1` to `64`
(which is in contrast to standard C/C++ arrays allowing only integers with a bit-width of 8 - char, 16 - short, 32 - int, 64 - long long, etc.).

The main class of this library is called `tdc::int_vector::IntVector<T>`, where T is either a plain C/C++ integer type (char,short,int,long,long long, etc.), or `uint_t<n>` representing numbers with bit-width `n`, where `n` can range from `1` to `64`.
The interface of this class tries to ressemble the interface of `std::vector<T>`.

Further information can be found in the [tudocomp documentation](http://tudocomp.org/documentation/index.html#arbitrary-width-integer-vectors)


# Dependencies

Same as [compact sparse hash](https://github.com/tudocomp/compact_sparse_hash)

# License

Same as [compact sparse hash](https://github.com/tudocomp/compact_sparse_hash)
