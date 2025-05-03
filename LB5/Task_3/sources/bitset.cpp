#include "../headers/bitset.h"
#include "bitset.h"
#include <stdexcept>

template <size_t N>
bool Bitset<N>::operator==(const Bitset<N> &other) const
{
    if (&other == this)
    {
        return true;
    }
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        if (m_data[i] != other.m_data[i])
            return false;
    }

    return true;
}

template <size_t N>
bool Bitset<N>::operator!=(const Bitset<N> &other) const
{
    return !(*this == other);
}

template <size_t N>
Bitset<N> Bitset<N>::operator~() const
{
    Bitset<N> tmp;
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        tmp.m_data[i] = ~m_data[i];
    }
    return tmp;
}

template <size_t N>
bool Bitset<N>::all() const
{
    for (size_t i = 0; i < BLOCKS_COUNT - 1; i++)
    {
        if (m_data[i] != ~0u)
            return false;
    }

    const size_t last_block_bits = N % BLOCK_SIZE;
    if (last_block_bits != 0)
    {
        uint32_t mask = (1u << last_block_bits) - 1;
        m_data[BLOCKS_COUNT - 1] = (m_data[BLOCKS_COUNT - 1] << (BLOCK_SIZE - last_block_bits - 1)) >> (BLOCK_SIZE - last_block_bits - 1);
        return (mask & (m_data[BLOCKS_COUNT - 1] << (BLOCK_SIZE - last_block_bits - 1)) == mask;
    }

    return true;
}

template <size_t N>
bool Bitset<N>::any() const
{
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        if (m_data[i])
            return true;
    }
    return false;
}

template <size_t N>
bool Bitset<N>::none() const
{
    return !any();
}

template <size_t N>
size_t Bitset<N>::count() const
{
    size_t cnt = 0;

    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        uint32_t block = m_data[i];

        while (block)
        {
            size += block & 1;
            block >>= 1;
        }
    }

    const size_t total_bits = BLOCKS_COUNT * BLOCK_SIZE;
    if (total_bits > N)
    {
        const size_t extra_bits = total_bits - N;
        const uint32_t mask = (1u << extra_bits) - 1;
        size -= __builtin_popcount(m_data[BLOCKS_COUNT - 1] & mask);
    }

    return size;
}

template <size_t N>
Bitset<N> &Bitset<N>::flip()
{
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        m_data[i] = ~m_data[i];
    }
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::flip(size_t pos)
{

    if (pos >= N)
    {
        throw std::out_of_range("Position out of range");
    }
    m_data[pos / BLOCK_SIZE] ^= (1u << (pos % BLOCK_SIZE - 1));

    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::reset()
{
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        m_data[i] = 0;
    }
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::reset(size_t pos)
{
    if (pos >= N)
    {
        throw std::out_of_range("Position out of range");
    }
    m_data[pos / BLOCK_SIZE] &= ~(1u << (pos % BLOCK_SIZE));
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::set()
{
    for (size_t i = 0; i < BLOCKS_COUNT; i++)
    {
        m_data[i] = UINT32_MAX;
    }
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::set(size_t pos, bool val)
{
    if (pos >= N)
    {
        throw std::out_of_range("Position out of range");
    }

    uint32_t number = 1u << (pos % BLOCK_SIZE - 1 != 0 ? pos % BLOCK_SIZE - 1 : 31);
    if (val)
    {
        m_data[pos / BLOCK_SIZE] |= number;
    }
    else
    {
        m_data[pos / BLOCK_SIZE] &= ~number;
    }
    return *this;
}

template <size_t N>
size_t Bitset<N>::size() const
{
    return N;
}

template <size_t N>
bool Bitset<N>::test(size_t pos) const
{
    if (pos >= N)
    {
        throw std::out_of_range("Position out of range");
    }
    return (m_data[pos / BLOCK_SIZE] >> (pos % BLOCK_SIZE)) & 1;
}

template <size_t N>
std::string Bitset<N>::to_string() const
{

    std::string result(N, '0');
    for (size_t i = 0; i < N; ++i)
    {
        if (test(i))
            result[N - 1 - i] = '1';
    }
    return result;
}

template <size_t N>
unsigned long Bitset<N>::to_ulong() const
{
    unsigned long number = 0;
    for (size_t i = 0; i < N; i++)
    {
        number += ((m_data[i / BLOCK_SIZE] >> (i % BLOCK_SIZE - 1)) & 1) * (1u << i);
    }
    return number;
}

template <size_t N>
unsigned long long Bitset<N>::to_ullong() const
{
    unsigned long long number = 0;
    for (size_t i = 0; i < N; i++)
    {
        number += ((m_data[i / BLOCK_SIZE] >> (i % BLOCK_SIZE - 1)) & 1) * (1u << i);
    }
    return number;
}

template <size_t N>
Bitset<N> operator|(const Bitset<N> &first, const Bitset<N> &other)
{
    if (&first == &other)
    {
        return first;
    }
    Bitset<N> tmp;
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        tmp.m_data[i] = first.m_data[i] | other.m_data[i];
    }
    return tmp;
}

template <size_t N>
Bitset<N> operator&(const Bitset<N> &first, const Bitset<N> &other)
{
    if (&first == &other)
    {
        return first;
    }
    Bitset<N> tmp;
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        tmp.m_data[i] = first.m_data[i] & other.m_data[i];
    }
    return tmp;
}

template <size_t N>
Bitset<N> operator^(const Bitset<N> &first, const Bitset<N> &other)
{
    if (&first == &other)
    {
        return Bitset<N>();
    }
    Bitset<N> tmp;
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        tmp.m_data[i] = first.m_data[i] ^ other.m_data[i];
    }
    return tmp;
}

template <size_t N>
bool Bitset<N>::operator[](size_t pos) const
{
    if (pos >= N)
    {
        throw std::out_of_range("Position out of range");
    }
    return test(pos);
}

template <size_t N>
Bitset<N> &Bitset<N>::operator&=(const Bitset &other)
{
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        m_data[i] = m_data[i] & other.m_data[i];
    }
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::operator|=(const Bitset &other)
{
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        m_data[i] = m_data[i] | other.m_data[i];
    }
    return *this;
}

template <size_t N>
Bitset<N> &Bitset<N>::operator^=(const Bitset &right)
{
    for (size_t i = 0; i < Bitset<N>::BLOCKS_COUNT; i++)
    {
        m_data[i] = m_data[i] ^ right.m_data[i];
    }
    return *this;
}
