#pragma once
template <typename T>
class Iterator
{
protected:
    T *arr_ = nullptr;

public:
    Iterator(T *p = nullptr) : arr_(p) {}
    Iterator(T &p = nullptr) : arr_(&p) {}
    ~Iterator()
    {
        delete[] arr_;
    }
    T &operator*() const { return arr_; }
    T *operator->() const { return arr_; }

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
    bool operator==(const Iterator &other) const { return arr_ == other.arr_; }
    bool operator!=(const Iterator &other) const { return arr_ != other.arr_; }
};
