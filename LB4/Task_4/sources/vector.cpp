#include "../headers/reverseIterator.h"
#include "../headers/vector.h"
#include <stdexcept>
#include <cstdint>

template <typename T>
Vector<T>::Vector(size_t capacity) : capacity_(capacity) {}

template <typename T>
void Vector<T>::assign(Iterator<T> first, Iterator<T> last)
{
    delete[] this->arr_;
    size_ = (&last)-- > first;
    while (size_ > capacity_)
    {
        capacity_ *= 2;
    }
    this->arr_ = new T[size_];
    for (int i = 0; i < size_; i++)
    {
        this->arr_[i] = &first[i];
    }
}

template <typename T>
T &Vector<T>::at(size_t position)
{
    if (position >= size_)
    {
        throw "Index out of range";
    }
    return this->arr_[position];
}

template <typename T>
T &Vector<T>::back()
{
    return this->arr_[size_ - 1];
}

template <typename T>
Iterator<T> Vector<T>::begin()
{
    return Iterator(this->arr_);
}

template <typename T>
Iterator<T> Vector<T>::end()
{
    return Iterator(this.arr_ + size_);
}

template <typename T>
size_t Vector<T>::capacity() const
{
    return capacity_;
}

template <typename T>
Iterator<T> Vector<T>::cbegin() const
{
    return Iterator(this.arr_);
}

template <typename T>
Iterator<T> Vector<T>::cend() const
{
    return Iterator(this.arr_ + size_);
}

template <typename T>
void Vector<T>::clear()
{
    delete[] this->arr_;
    this->arr_ = nullptr;
    size_ = 0;
    capacity_ = 16;
}

template <typename T>
T *Vector<T>::data()
{
    return this->arr_;
}

template <typename T>
template <typename... Args>
Iterator<T> Vector<T>::emplace(
    Iterator<T> position,
    Args &&...args)
{
    if (&position >= this->arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_)
    {
        reserve(capacity_ * 2);
    }
    for (auto i = end(); i >= position; i--)
    {
        i.arr_ = *(&i - 1);
    }
    size_++;
    new (&position) T(args...);
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
Iterator<T> Vector<T>::erase(Iterator<T> first, Iterator<T> last)
{
    if (last = nullptr)
    {
        last = first;
    }
    if (end() <= last)
    {
        throw "Index out of range";
    }

    last++;
    Iterator<T> start = first + 1;
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
    return *this->arr_;
}

template <typename T>
Iterator<T> Vector<T>::insert(
    const Iterator<T> position,
    T &&value)
{
    if (&position >= this->arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_)
    {
        reserve(capacity_ * 2);
    }
    for (auto i = end(); i >= position; i--)
    {
        i.arr_ = *(&i - 1);
    }
    size_++;
    *(&position) = value;
    return position;
}

template <typename T>
Iterator<T> Vector<T>::insert(
    const Iterator<T> position,
    size_t count,
    const T &value)
{
    if (&position >= this->arr_ + size_)
    {
        throw "Index out of range";
    }
    if (size_ == capacity_ + count)
    {
        reserve((capacity_ + count) * 2);
    }
    for (auto i = end() - 1 + count; i >= position + count; i--)
    {
        i.arr_ = *(&i - 1);
    }
    size_++;
    for (auto i = position; i < position + count; i++)
        *(&i) = value;
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
        reserve(capacity * 2);
    }
    this->arr_[size_] = std::move(value);
    size_++;
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
        data[i] = this->arr_[i];
    }
    delete this->arr_;
    this->arr_ = data;
}

template <typename T>
void Vector<T>::resize(const size_t new_size, const T &value)
{
    if (new_size <= size_)
    {
        for (int i = new_size; i < size_; i++)
        {
            this->arr_[i].~T();
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
            new (this.arr_ + i) T(value);
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

    T *temp_data = this->arr_;
    size_t temp_size = size_;
    size_t temp_capacity = capacity_;

    this->arr_ = other.arr_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.arr_ = temp_data;
    other.size_ = temp_size;
    other.capacity_ = temp_capacity;
}