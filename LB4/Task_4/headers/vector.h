#pragma once
#include "iterator.h"
#include "reverseIterator.h"
#include "pair.h"

template <typename T>
class Vector : public Iterator<T>
{
    size_t capacity_;
    size_t size_;

public:
    Vector(size_t capacity);
    Vector();
    ~Vector();
    void assign(Iterator<T> first, Iterator<T> last);
    T &at(size_t position);
    T &back();
    Iterator<T> begin();
    Iterator<T> end();
    size_t capacity() const;
    Iterator<T> cbegin() const;
    Iterator<T> cend() const;
    void clear();
    T *data();
    template <typename... Args>
    Iterator<T> emplace(
        Iterator<T> position,
        Args &&...args);

    template <typename... Args>
    Iterator<T> emplace_back(Args &&...args);

    Iterator<T> erase(Iterator<T> first, Iterator<T> last = nullptr);

    T &front();

    Iterator<T> insert(
        const Iterator<T> position,
        T &&value);
    Iterator<T> insert(
        const Iterator<T> position,
        size_t count,
        const T &value);

    size_t max_size() const;
    void pop_back();
    void push_back(T &&value);
    ReverseIterator<T> rbegin();
    ReverseIterator<T> rend();
    void reserve(const size_t new_capacity);
    void resize(const size_t new_size, const T &value = T());
    bool empty() const;

    size_t size() const;
    void swap(Vector<T> &right) noexcept;

    T &operator[](size_t index);
};

template class Vector<int>;
template class Vector<Pair<int, double>>;
template struct Pair<Vector<int>, Vector<Pair<int, double>>>;
template class Vector<Pair<Vector<int>, Vector<Pair<int, double>>>>;