#pragma once
#include <utility>
template <typename T1, typename T2>
struct Pair
{
    T1 first;
    T2 second;
    Pair() : first(), second() {}
    // Pair(Pair const &);
    // Pair(Pair &&) = default;
    Pair(const T1 &Val1, const T2 &Val2) : first(Val1), second(Val2) {}

    Pair(const Pair<T1, T2> &Right) : first(Right.first), second(Right.second) {}

    Pair(Pair<T1, T2> &&Right) noexcept : first(std::move(Right.first)), second(std::move(Right.second)) {}

    template <class Other1, class Other2>
    Pair(Other1 &&Val1, Other2 &&Val2) : first(std::move(Val1)), second(std::move(Val2)) {}

    Pair<T1, T2> &operator=(const Pair &p)
    {
        if (this != &p)
        {
            first = p.first;
            second = p.second;
        }
        return *this;
    }

    Pair<T1, T2> &operator=(Pair &&other)
    {
        if (this != &other)
        {
            first = std::move(other.first);
            second = std::move(other.second);
            return *this;
        }
        return *this;
    }
};
