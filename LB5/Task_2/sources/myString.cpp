#include "../headers/myString.h"
#include <clocale>
#include <stdexcept>
#include <iostream>

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
    for (size_t i = 0; i <= length_other; i++)
    {
        str_[i] = other[i];
    }
    return *this;
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

void *String::memcpy(void *s1, const void *s2, size_t n)
{
    if (s1 == nullptr || s2 == nullptr)
    {
        return s1;
    }
    char *dest = static_cast<char *>(s1);
    const char *src = static_cast<const char *>(s2);
    for (size_t i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
    return s1;
}

void *String::memmove(void *s1, const void *s2, size_t n)
{
    if (s1 == nullptr || s2 == nullptr || s1 == s2 || n == 0)
    {
        return s1;
    }

    char *dest = static_cast<char *>(s1);
    const char *src = static_cast<const char *>(s2);
    if (s1 > s2 && s1 < src + n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            dest[i] = src[i];
        }
    }
    else
    {
        for (size_t i = n; i > 0; --i)
        {
            dest[i - 1] = src[i - 1];
        }
    }

    return s1;
}

char *String::strcpy(char *s1, const char *s2)
{
    if (s1 == nullptr || s2 == nullptr)
    {
        return s1;
    }
    size_t size_2 = strlen(s2);
    for (size_t i = 0; i <= size_2; i++)
    {
        s1[i] = s2[i];
    }
    return s1;
}

char *String::strncpy(char *s1, const char *s2, size_t n)
{
    if (s1 == nullptr || s2 == nullptr)
    {
        return s1;
    }
    size_t i;
    for (i = 0; i < n && s2[i] != '\0'; i++)
    {
        s1[i] = s2[i];
    }
    for (; i < n; i++)
    {
        s1[i] = '\0';
    }
    return s1;
}

char *String::strcat(char *s1, const char *s2)
{
    if (s1 == nullptr || s2 == nullptr)
    {
        return s1;
    }
    char *ptr = s1 + strlen(s1);
    while ((*ptr++ = *s2++) != '\0')
        ;
    return s1;
}

char *String::strncat(char *s1, const char *s2, size_t n)
{
    if (s1 == nullptr || s2 == nullptr || n == 0)
    {
        return s1;
    }
    char *ptr = s1 + strlen(s1);
    size_t i;
    for (i = 0; i < n && s2[i] != '\0'; i++)
    {
        *ptr++ = s2[i];
    }
    *ptr = '\0';
    return s1;
}

int String::memcmp(const void *s1, const void *s2, size_t n)
{
    const char *ptr_1 = static_cast<const char *>(s1);
    const char *ptr_2 = static_cast<const char *>(s2);
    for (size_t i = 0; i < n; i++)
    {
        if (ptr_1[i] - ptr_2[i] != 0)
            return ptr_1[i] - ptr_2[i];
    }
    return 0;
}

int String::strcmp(const char *s1, const char *s2)
{
    std::cout << "custom" << std::endl;
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int String::strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;
    while (*s1 && *s2 && *s1 == *s2 && n--)
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

size_t String::strspn(const char *str, const char *accept)
{
    size_t count = 0;

    while (*str != '\0')
    {
        const char *a = accept;
        bool found = false; // теперь тип bool

        while (*a != '\0')
        {
            if (*a == *str)
            {
                found = true;
                break;
            }
            a++;
        }

        if (!found)
        {
            break;
        }

        count++;
        str++;
    }

    return count;
}

char *String::strpbrk(const char *str, const char *accept)
{
    for (; *str != '\0'; str++)
    {
        for (const char *a = accept; *a != '\0'; a++)
        {
            if (*a == *str)
            {
                return (char *)str;
            }
        }
    }
    return NULL;
}

int String::strcoll(const char *s1, const char *s2)
{
    // setlocale(LC_COLLATE, "ru_RU.UTF_+-8");

    return strcmp(s1, s2);
}

size_t String::strxfrm(char *s1, const char *s2, size_t n)
{
    size_t len = strlen(s2);
    if (len + 1 > n)
    {
        len = n - 1; // Обрезаем
    }
    strncpy(s1, s2, len);
    s1[len] = '\0';
    return len + 1; // Возвращаем количество символов с учетом нуль-терминатора
}

char *String::strtok(char *s1, const char *s2)
{
    static char *saved_ptr = NULL;
    char *tocken_start;
    if (s1 != NULL)
    {
        saved_ptr = s1;
    }
    else if (saved_ptr == NULL)
    {
        return NULL;
    }

    char *token_end = strpbrk(saved_ptr, s2);
    if (token_end != NULL)
    {
        *token_end = '\0';
        saved_ptr = token_end + 1;
    }
    else
    {
        saved_ptr = NULL;
    }

    return saved_ptr - (token_end ? 1 : 0);
}

void *String::memset(void *s, int c, size_t n)
{
    if (s == nullptr)
    {
        return s;
    }
    unsigned char *src = static_cast<unsigned char *>(s);
    while (n--)
    {
        *src++ = static_cast<unsigned char>(c);
    }
    return s;
}

char *String::strerror(int errnum)
{
    static char *errors[] =
        {
            "No error",
            "Operation not permitted",
            "No such file or directory",
            "No such process",
            "Interrupted system call",
            "Input/output error",
        };

    if (errnum >= 0 && errnum < sizeof(errors) / sizeof(errors[0]))
    {
        return errors[errnum];
    }
    else
    {
        return "Unknown error";
    }
}

size_t String::strlen(const char *s)
{
    size_t length = 0;
    while (s[length++] != '\0')
        ;
    return --length;
}

String &String::operator+(const String &other)
{
    // strncat(other.str_.get(), other.length_);
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