#pragma once
#include <cmath>

namespace DiffX {
    template <typename T>
    struct Dual {
        T v;
        T d;

        Dual() : v(0), d(1) {}
        Dual(T val = 0, T der = 0) : v(val), d(der) {}

        Dual operator+(const Dual& other) const { return {v + other.v, d + other.d}; }
        Dual operator-(const Dual& other) const { return {v - other.v, d - other.d}; }
        Dual operator*(const Dual& other) const { return {v * other.v, v * other.d + d * other.v}; }
        Dual operator/(const Dual& other) const { 
            return {v / other.v, (d * other.v - v * other.d) / (other.v * other.v)}; 
        }
        Dual operator-() const { return {-v, -d}; }
        Dual operator+(T scalar) const { return {v + scalar, d}; }
        Dual operator*(T scalar) const { return {v * scalar, d * scalar}; }
    };

    template <typename T>
    inline Dual<T> exp(const Dual<T>& a) {
        T val = std::exp(a.v);
        return Dual<T>(val, val * a.d); // Chain rule: f'(g(x)) * g'(x)
    }

    template <typename T>
    inline Dual<T> sin(const Dual<T>& a) {
        return Dual<T>(std::sin(a.v), std::cos(a.v) * a.d);
    }

    template <typename T>
    inline Dual<T> cos(const Dual<T>& a) {
        return Dual<T>(std::cos(a.v), -std::sin(a.v) * a.d);
    }
}