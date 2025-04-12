#include <cstdlib>
#include <memory>
#pragma once

class String
{
private:
    static constexpr struct
    {
        const char *original;
        const char *transformed;
    } russian_transforms[] = {
        {"А", "а"}, {"Б", "б"}, /* ... */ {"Я", "я"}, {"а", "а"}, {"б", "б"}, /* ... */ {"я", "я"}, {"Ё", "е"}, {"ё", "е"}};
    enum
    {
        min_size = 0,
        min_capacity = 16,
        factor = 2
    };
    size_t length_;
    size_t capacity_;
    std::unique_ptr<char[]> str_;
    class Iterator
    {
    public:
        Iterator(char *ptr);

        Iterator();
        Iterator(const Iterator &other);
        Iterator &operator=(const Iterator &other);
        char &operator*() const;
        char *operator->() const;
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        bool operator<=(const Iterator &other) const;
        bool operator>=(const Iterator &other) const;
        bool operator>(const Iterator &other) const;
        bool operator<(const Iterator &other) const;
        Iterator &operator+(size_t number) const;
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator--();
        Iterator operator--(int);
        char *get_ptr();

    private:
        char *ptr;
    };

    void reserve(const size_t new_capacity);
    void resize(const size_t new_size);
    Iterator begin();
    Iterator end();

public:
    String();
    String(const char *str);
    String(const String &&other);
    ~String() = default;
    String &operator=(const String *other);
    String &operator=(const String &&other);

    String &assign(String &other);
    String &assign(const char *other);

    Iterator erase(Iterator first, Iterator last = nullptr);
    Iterator insert(const Iterator posistion, const String &value);
    void pop_back();
    void push_back(char value);
    void append(const String &other);

    String *memmove(const char *s2, size_t n);
    // char *strcpy(char *s1, const char *s2);
    String &strncpy(const char *s, size_t n);
    String &strcat(const char *s2);
    String &strncat(const char *s2, size_t n);

    int memcmp(const void *s2, size_t n);
    int strcmp(const char *s2);

    int strcoll(const char *s2);
    int strncmp(const char *s2, size_t n);
    size_t strxfrm(const char *s1, size_t n);
    char *strtok(const char *s2);
    void *memset(void *s, int c, size_t n);
    char *strerror(int errnum);

    size_t length();
    size_t size();
    String &operator+(const String &other);
    char &operator[](size_t index);
    const char *c_str() const;
};
