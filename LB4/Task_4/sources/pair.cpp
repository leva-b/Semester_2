#include "../header/pair.h"

template <typename T1, typename T2>
constexpr Pair<T1, T2>::Pair() : first{}, second{} {}

template <typename T1, typename T2>
constexpr Pair<T1, T2>::Pair(const T1 &Val1, const T2 &Val2)
    : first(Val1), second(Val2) {}

template <typename T1, typename T2>
template <class Other1, class Other2>
constexpr Pair<T1, T2>::Pair(const Pair<Other1, Other2> &Right)
    : first(Right.first), second(Right.second) {}

template <typename T1, typename T2>
template <class Other1, class Other2>
constexpr Pair<T1, T2>::Pair(const Pair<Other1, Other2> &&Right)
    : first(std::move(Right.first)), second(std::move(Right.second)) {}

template <typename T1, typename T2>
template <class Other1, class Other2>
constexpr Pair<T1, T2>::Pair(Other1 &&Val1, Other2 &&Val2)
    : first(std::move(Val1)), second(std::move(Val2)) {}

template <typename T1, typename T2>
Pair<T1, T2> &Pair<T1, T2>::operator=(const Pair &p)
{
    if (this != &p)
    {
        first = p.first;
        second = p.second;
    }
    return *this;
}

template <typename T1, typename T2>
template <class U1, class U2>
Pair<T1, T2> &Pair<T1, T2>::operator=(const Pair<U1, U2> &p)
{
    first = p.first;
    second = p.second;
    return *this;
}

template <typename T1, typename T2>
Pair<T1, T2> &Pair<T1, T2>::operator=(Pair<T1, T2> &&p)
{
}
