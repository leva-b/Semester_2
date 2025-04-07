#pragma once
#include <cstddef>
template <typename T>
class Iterator
{
protected:
    T *arr_ = nullptr;

public:
    Iterator(const Iterator<T> &other) : arr_(other.arr_) {}
    Iterator(T *p) : arr_(p) {}
    Iterator(T &p) : arr_(&p) {}
    Iterator() {}

    T *operator&() const { return arr_; }

    Iterator &operator++()
    {
        ++arr_;
        return *this;
    }
    Iterator operator++(int)
    {
        Iterator tmp(*this);
        ++arr_;
        return tmp;
    }
    Iterator &operator--()
    {
        --arr_;
        return *this;
    }
    Iterator operator--(int)
    {
        Iterator<T> tmp(*this);
        --arr_;
        return tmp;
    }
    Iterator operator+(const long long number)
    {
        return arr_ + number;
    }

    size_t operator-(const Iterator other) const { return arr_ - other.arr_; }
    Iterator operator-(const size_t number) const { return arr_ - number; }
    bool operator>(const Iterator &other) const { return arr_ > other.arr_; }
    bool operator<(const Iterator &other) const { return arr_ < other.arr_; }
    bool operator>=(const Iterator &other) const { return arr_ >= other.arr_; }
    bool operator<=(const Iterator &other) const { return arr_ <= other.arr_; }
    bool operator==(const Iterator &other) const { return arr_ == other.arr_; }
    bool operator!=(const Iterator &other) const { return arr_ != other.arr_; }
};

using A1 = Iterator<int>;
