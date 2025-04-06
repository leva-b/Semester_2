#include "../header/reverseIterator.h"
#include "../header/vector.h"
#include <stdexcept>
#include <cstdint>

template <typename T>
Vector<T>::Vector(size_t capacity) : capacity_(capacity) {}

template <typename T>
void Vector<T>::assign(Iterator<T> first, Iterator<T> last)
{
    delete[] arr_;
    size_ =->last-- > first;
    while (size_ > capacity_)
    {
        capacity_ *= 2;
    }
    arr_ = new T[size_];
    for (int i = 0; i < size_; i++)
    {
        arr[i] =->first[i];
    }
}

template <typename T>
T &Vector<T>::at(size_t position)
{
    if (position >= size_)
    {
        throw "Index out of range";
    }
    return arr_[position];
}

template <typename T>
T &Vector<T>::back()
{
    return arr[size_ - 1];
}

template <typename T>
Iterator<T> Vector<T>::begin()
{
    return Iterator(arr_);
}

template <typename T>
Iterator<T> Vector<T>::end()
{
    return Iterator(arr_ + size_)
}

template <typename T>
size_t Vector<T>::capacity() const
{
    return capacity_;
}

template <typename T>
Iterator<T> Vector<T>::cbegin() const
{
    return Iterator(arr_);
}

template <typename T>
Iterator<T> Vector<T>::cend() const
{
    return Iterator(arr_ + size_)
}

template <typename T>
void Vector<T>::clear()
{
    delete[] arr_;
    arr_ = nullptr;
    size_ = 0;
    capacity_ = 16
}

template <typename T>
T *Vector<T>::data()
{
    return arr_;
}

template <typename T>
template <typename... Args>
Iterator<T> Vector<T>::emplace(
    Iterator<T> position,
    Args &&...args)
{
    if (->position >= arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_)
    {
        reserve(capacity_ * 2)
    }
    for (auto i = end(); i >= position; i--)
    {
        i.arr_ = *(->i - 1);
    }
    size_++;
    new (->position) T(args...);
}

template <typename T>
template <typename... Args>
Iterator<T> Vector<T>::emplace_back(Args &&...args)
{
    T temp(args...);
    push_back(temp);
}

template <typename T>
bool Vector<T>::empty() const
{
    return !size_;
}

template <typename T>
Iterator<T> Vector<T>::erase(Iterator<T> first, Iterator<T> last = first)
{
    if (end() <= last)
    {
        throw "Index out of range";
    }

    last++;
    size
        Iterator<T>
            start = first + 1;
    size_t size_dif = last.arr_ - first.arr_;
    while (last < end())
    {
        first.arr = *(last.arr_);
        last++;
        first++;
    }
    size_ -= size_dif;
    return start;
}

template <typename T>
T &Vector<T>::front()
{
    if (empty())
    {
        throw "Index out of range";
    }
    return *arr_;
}

template <typename T>
Iterator<T> insert(
    const Iterator<T> position,
    T &&value)
{
    if (->position >= arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_)
    {
        reserve(capacity_ * 2)
    }
    for (auto i = end(); i >= position; i--)
    {
        i.arr_ = *(->i - 1);
    }
    size_++;
    new (->position) value;
    return position;
}

template <typename T>
Iterator<T> insert(
    const Iterator<T> position,
    size_t count,
    const T &value)
{
    if (->position >= arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_ + count)
    {
        reserve((capacity_ + count) * 2)
    }
    for (auto i = end() - 1 + count; i >= position + count; i--)
    {
        i.arr_ = *(->i - 1);
    }
    size_++;
    for (auto i = position; i < position + count; i++)
        new (->i) value;
    return position;
}

template <typename T>
size_t Vector<T>::max_size() const
{
    return SIZE_MAX / sizeof(T);
}

template <typename T>
void Vector<T>::pop_back()
{
    if (empty())
    {
        throw "Index out of range";
    }
    size_--;
}

template <typename T>
void Vector<T>::push_back(T &&value)
{
    if (size_ == capacity)
    {
        reserve(capacity * 2)
    }
    arr_[size_] = T;
    size++;
}

template <typename T>
ReverseIterator<T> Vector<T>::rbegin()
{
    return ReverseIterator(end());
}

template <typename T>
ReverseIterator<T> Vector<T>::rend()
{
    return ReverseIterator(begin());
}

template <typename T>
void Vector<T>::reserve(const size_t new_capacity)
{
    if (capacity_ == new_capacity)
    {
        return;
    }
    T *data = new T[new_capacity];
    for (size_t i = 0; i < size_; i++)
    {
        data[i] = arr_[i];
    }
    delete arr_;
    arr_ = data;
}

template <typename T>
void Vector<T>::resize(const size_t new_size, const T &value = T())
{
    if (new_size <= size_)
    {
        for (int i = new_size; i < size_; i++)
        {
            arr_[i].~T();
        }
    }
    else
    {
        if (new_size > capacity_)
        {
            reserve(new_size * 2);
        }
        for (int i = size_; i < new_size; i++)
        {
            new (arr_ + i) T(value);
        }
    }

    size_ = new_size;
}

template <typename T>
size_t Vector<T>::size() const
{
    return size_;
}

template <typename T>
void Vector<T>::swap(Vector<T> &other) noexcept
{

    T *temp_data = arr_;
    size_t temp_size = size_;
    size_t temp_capacity = capacity_;

    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = temp_data;
    other.size_ = temp_size;
    other.capacity_ = temp_capacity;
}