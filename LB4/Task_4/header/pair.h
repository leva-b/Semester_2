template <typename T1, typename T2>
struct Pair
{
    T1 first;
    T2 second;
    constexpr Pair();
    Pair(const Pair &) = default;
    Pair(Pair &&) = default;
    constexpr Pair(
        const T1 &Val1,
        const T2 &Val2);

    template <class Other1, class Other2>
    constexpr Pair(const Pair<Other1, Other2> &Right);

    template <class Other1, class Other2>
    constexpr Pair(const Pair<Other1, Other2> &&Right);

    template <class Other1, class Other2>
    constexpr Pair(Other1 &&Val1, Other2 &&Val2);

    Pair &operator=(const Pair &p);
    template <class U1, class U2>
    Pair &operator=(const pair<U1, U2> &p);
    Pair &operator=(pair &&p) noexcept(see below);
    template <class U1, class U2>
    Pair &operator=(pair<U1, U2> &&p);

    void swap(pair &p) noexcept(see below);
};
