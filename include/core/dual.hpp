#pragma once
#include <cmath>
#include <iostream>

namespace DiffX {
    struct Dual {
        float v;
        float d;

        Dual(float val = 0, float der = 0) : v(val), d(der) {}

        Dual operator+(const Dual& other) const { return {v + other.v, d + other.d}; }
        Dual operator-(const Dual& other) const { return {v - other.v, d - other.d}; }
        Dual operator*(const Dual& other) const { return {v * other.v, v * other.d + d * other.v}; }
        Dual operator/(const Dual& other) const { 
            return {v / other.v, (d * other.v - v * other.d) / (other.v * other.v)}; 
        }
        Dual operator-() const { return {-v, -d}; }
        Dual operator+(float scalar) const { return {v + scalar, d}; }
        Dual operator*(float scalar) const { return {v * scalar, d * scalar}; }
    };

    inline Dual exp(const Dual& a) {
        float val = std::exp(a.v);
        return Dual(val, val * a.d); // Chain rule: f'(g(x)) * g'(x)
    }

    inline Dual sin(const Dual& a) {
        return Dual(std::sin(a.v), std::cos(a.v) * a.d);
    }

    inline Dual cos(const Dual& a) {
        return Dual(std::cos(a.v), -std::sin(a.v) * a.d);
    }

    std::ostream& operator<<(std::ostream& os, const Dual& d) {
        os << d.v << " (" << ((d.d>=0.0f) ? "+" : "") << d.d << ")";
        return os;
    }
}