#include <cstdlib>
#include <iostream>

#define USE_CLASS

#if !defined(USE_CLASS)

template<size_t dp>
constexpr int32_t DoubleToFixed(double d)
{
    return int32_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
}

template<size_t dp>
constexpr double FixedToDouble(int32_t d)
{
    return double(d) / double(1 << dp);
}

#else

template<typename T, typename T2, size_t DP>
class fixed
{
public:
    T value = T(0);

    constexpr fixed() = default;
    constexpr fixed(const double d)
    {
        value = T(d * double(1 << DP) + (d >= 0 ? 0.5 : -0.5));
    }

    constexpr operator double() const
    {
        return double(value) / double(1 << DP);
    }

    // assignment
    constexpr fixed& operator=(const fixed& f) = default;

    // negate unary
    constexpr fixed operator-() const
    {
        return from(-this->value);
    }

    // addition
    constexpr fixed operator+(const fixed& f) const
    {
        return from(this->value + f.value);
    }

    constexpr fixed operator+=(const fixed& f) const
    {
        this->value += f.value;
        return *this;
    }

    // subtraction
    constexpr fixed operator-(const fixed& f) const
    {
        return from(this->value - f.value);
    }

    constexpr fixed operator-=(const fixed& f) const
    {
        this->value -= f.value;
        return *this;
    }

    // multiplication
    constexpr fixed operator*(const fixed& f) const
    {
        return from((T2(this->value) * T2(f.value)) >> DP);
    }

    constexpr fixed operator*=(const fixed& f) const
    {
        this->value = (T2(this->value) * T2(f.value)) >> DP;
        return *this;
    }

    // division
    constexpr fixed operator/(const fixed& f) const
    {
        return from(((T2(this->value) << DP) / T2(f.value)));
    }

    constexpr fixed operator/=(const fixed& f) const
    {
        this->value = ((T2(this->value) << DP) / T2(f.value));
        return *this;
    }

private:
    static constexpr fixed from(T v) { fixed k; k.value = v; return k; }
};

fixed<int8_t, int16_t, 4> constexpr operator "" _fp44(const long double n)
{
    return fixed<int8_t, int16_t, 4>(double(n));
}

template<typename T, typename T2, size_t DP>
std::ostream& operator << (std::ostream& os, const fixed<T, T2, DP>& f) {
    return (os << double(f));
}

#endif

int main()
{
#ifndef USE_CLASS

    constexpr auto DP = 2;

    // 16.16 fixed point type
    constexpr int32_t a = DoubleToFixed<DP>(5.6);
    constexpr int32_t b = DoubleToFixed<DP>(2.7);

    constexpr int32_t c = a + b;
    constexpr int32_t d = a - b;

    constexpr int32_t e = (int64_t(a) * int64_t(b)) >> DP;
    constexpr int32_t f = (int64_t(a) << DP) / int64_t(b);

    constexpr double z = FixedToDouble<DP>(f);

#else

    // using 32 bit integer with 16 bits for decimal
    using fp16_16 = fixed<int32_t, int64_t, 16>;

    constexpr fp16_16 a(5.6);
    constexpr fp16_16 b(2.7);
    
    constexpr double z = double(a * b);

    // using 8 bit integer with 4 bits for decimal
    using fp4_4 = fixed<int8_t, int16_t, 4>;

    constexpr fp4_4 a4(5.6);
    constexpr fp4_4 b4(2.7);

    constexpr fp4_4 circ = fp4_4(2.0) * fp4_4(3.1415) * a4;

    // using literal suffix _fp44 defined above
    constexpr fp4_4 circ2 = 2.0_fp44 * 3.1415_fp44 * b4;
    std::cout << circ2 << std::endl;

    constexpr double z4 = double(a4 * b4);

#endif

    return EXIT_SUCCESS;
}