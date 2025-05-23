#ifndef MYSTRING_API
#define MYSTRING_API __attribute__((visibility("default")))
#endif
#include <cstdlib>
#include <memory>

class MYSTRING_API String
{
private:
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
    size_t length();
    size_t size();
    String &operator+(const String &other);
    char &operator[](size_t index);
    const char *c_str() const;
    Iterator begin();
    Iterator end();

    static MYSTRING_API void *memcpy(void *s1, const void *s2, size_t n);
    static MYSTRING_API void *memmove(void *s1, const void *s2, size_t n);
    static MYSTRING_API char *strcpy(char *s1, const char *s2);
    static MYSTRING_API char *strncpy(char *s1, const char *s2, size_t n);
    static MYSTRING_API char *strcat(char *s1, const char *s2);
    static MYSTRING_API char *strncat(char *s1, const char *s2, size_t n);
    static MYSTRING_API int memcmp(const void *s1, const void *s2, size_t n);
    static MYSTRING_API int strcmp(const char *s1, const char *s2);
    static MYSTRING_API int strcoll(const char *s1, const char *s2);
    static MYSTRING_API int strncmp(const char *s1, const char *s2, size_t n);
    static MYSTRING_API size_t strxfrm(char *s1, const char *s2, size_t n);
    static MYSTRING_API char *strtok(char *s1, const char *s2);
    static MYSTRING_API void *memset(void *s, int c, size_t n);
    static MYSTRING_API char *strerror(int errnum);
    static MYSTRING_API size_t strlen(const char *s);

    static MYSTRING_API char *strpbrk(const char *str, const char *accept);
    static MYSTRING_API size_t strspn(const char *str, const char *accept);
};
