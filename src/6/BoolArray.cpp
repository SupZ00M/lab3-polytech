#include "BoolArray.hpp"
#include <stdexcept>
#include <cstring>
#include <utility>


void BoolArray::clear_unused_bits() {
    if (nbits == 0) return;
    std::size_t used = nbits % 8;
    if (used == 0) return;   // последний байт заполнен полностью
    // Зануляем старшие (8 - used) биты последнего байта
    std::uint8_t mask = static_cast<std::uint8_t>((1u << used) - 1u);
    data[nbits / 8] &= mask;
}


BoolArray::BoolArray(std::size_t n, bool fill)
    : data(nullptr), nbits(n) {
    if (n == 0) return;
    std::size_t nbytes = (n + 7) / 8;
    data = new std::uint8_t[nbytes];
    std::memset(data, fill ? 0xFF : 0x00, nbytes);
    clear_unused_bits();   // если fill == true — занулим хвост
}

BoolArray::BoolArray(const BoolArray& other)
    : data(nullptr), nbits(other.nbits) {
    if (nbits == 0) return;
    std::size_t nbytes = (nbits + 7) / 8;
    data = new std::uint8_t[nbytes];
    std::memcpy(data, other.data, nbytes);
}

BoolArray::BoolArray(BoolArray&& other) noexcept
    : data(other.data), nbits(other.nbits) {
    other.data  = nullptr;
    other.nbits = 0;
}

BoolArray::~BoolArray() {
    delete[] data;
}


BoolArray& BoolArray::operator=(const BoolArray& other) {
    if (this == &other) return *this;
    delete[] data;
    data  = nullptr;
    nbits = other.nbits;
    if (nbits == 0) return *this;
    std::size_t nbytes = (nbits + 7) / 8;
    data = new std::uint8_t[nbytes];
    std::memcpy(data, other.data, nbytes);
    return *this;
}

BoolArray& BoolArray::operator=(BoolArray&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data        = other.data;
    nbits       = other.nbits;
    other.data  = nullptr;
    other.nbits = 0;
    return *this;
}


BoolArray::BitReference BoolArray::operator[](std::size_t i) {
    if (i >= nbits)
        throw std::out_of_range("BoolArray::operator[]: index out of range");
    return BitReference(&data[i / 8], i % 8);
}

bool BoolArray::operator[](std::size_t i) const {
    if (i >= nbits)
        throw std::out_of_range("BoolArray::operator[]: index out of range");
    return (data[i / 8] >> (i % 8)) & 1u;
}


void BoolArray::resize(std::size_t new_size, bool fill) {
    if (new_size == nbits) return;


    BoolArray tmp(new_size, fill);

  
    std::size_t common = (new_size < nbits) ? new_size : nbits;
    for (std::size_t i = 0; i < common; ++i)
        tmp[i] = static_cast<bool>((*this)[i]);

  
    *this = std::move(tmp);
}


void BoolArray::print(std::ostream& os) const {
    os << "[";
    for (std::size_t i = 0; i < nbits; ++i) {
        if (i > 0) os << ", ";
        os << ((*this)[i] ? "true" : "false");
    }
    os << "]";
}
