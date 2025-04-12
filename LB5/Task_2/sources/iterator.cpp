#include "../headers/myString.h"

String::Iterator::Iterator() : ptr(nullptr) {}

String::Iterator::Iterator(const Iterator &other) : ptr(other.ptr) {}

String::Iterator &String::Iterator::operator=(const Iterator &other)
{
    ptr = other.ptr;
}

char &String::Iterator::operator*() const
{
    return *ptr;
}

char *String::Iterator::operator->() const
{
    return ptr;
}

bool String::Iterator::operator==(const Iterator &other) const
{
    return ptr == other.ptr;
}

bool String::Iterator::operator!=(const Iterator &other) const
{
    return ptr != other.ptr;
}

bool String::Iterator::operator<=(const Iterator &other) const
{
    return ptr <= other.ptr;
}

bool String::Iterator::operator>=(const Iterator &other) const
{
    return ptr >= other.ptr;
}

bool String::Iterator::operator<(const Iterator &other) const
{
    return ptr < other.ptr;
}

bool String::Iterator::operator>(const Iterator &other) const
{
    return ptr > other.ptr;
}

String::Iterator &String::Iterator::operator+(size_t number) const
{
    Iterator tmp(ptr);
    tmp++;
    return tmp;
}

String::Iterator &String::Iterator::operator++()
{
    ptr++;
    return (*this);
}

String::Iterator String::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ptr++;
    return tmp;
}

String::Iterator &String::Iterator::operator--()
{
    ptr--;
    return *this;
}

String::Iterator String::Iterator::operator--(int)
{
    Iterator tmp = *this;
    ptr--;
    return tmp;
}

String::Iterator::Iterator(char *ptr) : ptr(ptr) {}

char *String::Iterator::get_ptr()
{
    return ptr;
}