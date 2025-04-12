#include "../headers/myString.h"
#include <cstring>
#include <clocale>
#include <stdexcept>

String::String() : length_(min_size), capacity_(min_capacity)
{
    str_ = std::make_unique<char[]>(capacity_);
}

String &String::assign(const char *other)
{
    size_t length_other = strlen(other);
    if (length_other > capacity_)
    {
        reserve(length_other * 2);
    }
}

String::Iterator String::begin()
{
    return Iterator(str_.get());
}

String::Iterator String::end()
{
    return Iterator(str_.get() + size());
}

String::Iterator String::erase(String::Iterator first, String::Iterator last)
{
    if (last == nullptr)
    {
        last = first;
    }
    if (end() <= last)
    {
        throw "Index out of range";
    }

    last++;
    Iterator start = first + 1;
    size_t size_dif = &last - &first;
    while (last < end())
    {
        *(first) = *(last);
        last++;
        first++;
    }
    length_ -= size_dif;
    return start;
}

String::Iterator String::insert(
    Iterator position,
    const String &value)
{
    if (position >= Iterator(str_.get() + length_))
    {
        throw "Index out of range";
    }
    if (capacity_ < length_ + value.length_ + 1)
    {
        reserve((capacity_ + value.length_ + 1) * 2);
    }
    for (auto i = end(); i >= position; i--)
    {
        *(i + value.length_) = *i;
    }
    for (size_t i = position.get_ptr() - begin().get_ptr(); i < (position + value.length_).get_ptr() - begin().get_ptr(); i++)
    {
        new (position.get_ptr()) char(const_cast<String &>(value)[i - (position.get_ptr() - begin().get_ptr())]);
    }
    length_ += value.length_;
    str_[length_] = '\0';
    return position;
}

void String::pop_back()
{
    length_--;
    str_[length_] = '\0';
}

void String::push_back(char value)
{
    if (length_ + 2 >= capacity_)
        resize(capacity_ * factor);
    str_[length_++] = value;
    str_[length_] = '\0';
}

void String::append(const String &other)
{
    for (auto i = const_cast<String &>(other).begin(); i != const_cast<String &>(other).end(); i++)
    {
        push_back(*i);
    }
}

void String::reserve(const size_t new_capacity)
{
    if (capacity_ == new_capacity)
    {
        return;
    }
    else if (capacity_ - new_capacity <= capacity_ * 0.5)
        ;
    char *data = new char[new_capacity];
    for (size_t i = 0; i < length_ + 1; i++)
    {
        new (data + i) char(str_[i]);
    }
    str_.reset(data);
    capacity_ = new_capacity;
}

void String::resize(const size_t new_size)
{
    if (new_size > capacity_)
    {
        reserve(new_size * 2);
    }
    if (new_size < length_)
    {
        str_[new_size] = '\0';
    }
    else
    {
        for (size_t i = length_; i < new_size; i++)
            str_[i] = ' ';
    }
    length_ = new_size;
    str_[new_size] = '\0';
}

size_t String::length() { return length_; }

size_t String::size() { return length_; }

String *String::memmove(const char *s2, size_t n)
{
    if (!s2 || !str_)
    {
        return nullptr;
    }
    if (n > capacity_)
    {
        reserve(n * 2);
    }
    if ((str_.get() > s2 && str_.get() < s2 + n) || (s2 > str_.get() && s2 < str_.get() + n))
    {
        std::unique_ptr<char[]> temp(new char[n]);
        for (size_t i = 0; i < n; ++i)
        {
            temp[i] = s2[i];
        }
        for (size_t i = 0; i < n; ++i)
        {
            str_.get()[i] = temp[i];
        }
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            str_.get()[i] = s2[i];
        }
    }

    length_ = n;
    str_[length_] = '\0';

    return this;
}

String &String::strncpy(const char *s, size_t n)
{
    if (capacity_ <= n)
    {
        resize(n);
    }
    for (size_t i = 0; i < n; i++)
    {
        str_[i] = s[i];
    }
    return *this;
}

String &String::strcat(const char *s2)
{
    size_t old_length = length_;
    if (capacity_ <= length_ + strlen(s2))
    {
        resize(length_ + strlen(s2));
    }
    for (size_t i = old_length; i < length_; i++)
    {
        str_[i] = s2[i - old_length];
    }
    return *this;
}

String &String::strncat(const char *s2, size_t n)
{
    size_t old_length = length_;
    if (capacity_ <= length_ + n)
    {
        resize(length_ + n);
    }
    for (size_t i = old_length; i < length_; i++)
    {
        str_[i] = s2[i - old_length];
    }
    return *this;
}

int String::memcmp(const void *s2, size_t n)
{
    const char *str2 = static_cast<const char *>(s2);

    size_t compare_len = (n > length_ + 1) ? length_ + 1 : n;

    for (size_t i = 0; i < compare_len; ++i)
    {
        if (str_[i] < str2[i])
        {
            return -1;
        }
        else if (str_[i] > str2[i])
        {
            return 1;
        }
    }

    if (length_ < n)
    {
        return -1;
    }
    else if (length_ > n)
    {
        return 1;
    }

    return 0;
}

int String::strcmp(const char *s2)
{
    size_t compare_len = (strlen(s2) > length_) ? length_ : strlen(s2);

    for (size_t i = 0; i < compare_len; ++i)
    {
        if (str_[i] < s2[i])
        {
            return -1;
        }
        else if (str_[i] > s2[i])
        {
            return 1;
        }
    }

    if (length_ < strlen(s2))
    {
        return -1;
    }
    else if (length_ > strlen(s2))
    {
        return 1;
    }

    return 0;
}

size_t String::strxfrm(const char *s1, size_t n)
{
    if (s1 == nullptr)
        return length_ + 1;
}

// int String::strcoll(const char *s2)
// {
//     const char *locale = setlocale(LC_COLLATE, nullptr);
//     if (!locale)
//     {
//         return strcmp(s2);
//     }

//     // Определяем размер буферов
//     size_t len1 = strxfrm(nullptr, s1, 0);
//     size_t len2 = strxfrm(nullptr, s2, 0);

//     // Выделяем память через new[]
//     std::unique_ptr<char[]> buf1(new char[len1 + 1]);
//     std::unique_ptr<char[]> buf2(new char[len2 + 1]);

//     // Преобразуем строки
//     strxfrm(buf1.get(), s1, len1 + 1);
//     strxfrm(buf2.get(), s2, len2 + 1);

//     // Сравниваем
//     return strcmp(buf1.get(), buf2.get());
// }

String &String::operator+(const String &other)
{
    strncat(other.str_.get(), other.length_);
    return *this;
}

const char *String::c_str() const
{
    return str_.get();
}

char &String::operator[](size_t index)
{
    if (index >= length_)
    {
        throw std::out_of_range("Index out of range");
    }
    return str_[index];
}