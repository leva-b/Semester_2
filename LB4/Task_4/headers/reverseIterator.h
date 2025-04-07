#pragma once
#include "iterator.h"

template <typename T>
class ReverseIterator : public Iterator<T>
{
public:
    ReverseIterator(const Iterator<T> iterator) : Iterator<T>(iterator) {}
    using Iterator<T>::Iterator;

    ReverseIterator<T> &operator++()
    {
        Iterator<T>::operator--();
        return *this;
    }

    ReverseIterator<T> &operator--()
    {
        Iterator<T>::operator++();
        return *this;
    }

    T &operator*() const
    {
        Iterator<T> tmp = *this;
        return *(--tmp);
    }
};