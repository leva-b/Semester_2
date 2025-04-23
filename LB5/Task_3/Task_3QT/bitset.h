#ifndef BITSET_H
#define BITSET_H

#pragma once
#include <cstddef>
#include <cstdint>
#include <string>


template <size_t N>
class Bitset
{
public:
    static constexpr size_t BLOCK_SIZE = 32;
    static constexpr size_t BLOCKS_COUNT = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;

    template<size_t M>
    friend Bitset<M> operator|(const Bitset<M>& first, const Bitset<M>& other);

    template<size_t M>
    friend Bitset<M> operator&(const Bitset<M>& first, const Bitset<M>& other);

    template<size_t M>
    friend Bitset<M> operator^(const Bitset<M>& first, const Bitset<M>& other);

    Bitset() = default;
    Bitset(const Bitset &other) = default;
    ~Bitset() {}

    bool operator==(const Bitset &other) const;
    bool operator!=(const Bitset &other) const;
    Bitset operator~() const;
    Bitset &operator&=(const Bitset &other);
    Bitset &operator|=(const Bitset &other);
    Bitset &operator^=(const Bitset &right);
    // Bitset operator<<(size_t pos);
    // Bitset operator>>(size_t pos);

    bool operator[](size_t pos) const;

    bool all();
    bool any() const;
    bool none() const;
    size_t count() const;
    Bitset &flip();
    Bitset &flip(size_t pos);
    Bitset<N> &reset();
    Bitset<N> &reset(size_t pos);
    Bitset<N> &set();
    Bitset<N> &set(
        size_t pos,
        bool val = true);
    size_t size() const;
    bool test(size_t pos) const;
    std::string to_string() const;
    unsigned long to_ulong() const;
    unsigned long long to_ullong() const;

private:
    uint32_t m_data[BLOCKS_COUNT] = {0};
};

template class Bitset<32>;


#endif // BITSET_H
