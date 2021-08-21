/**
 * 2D vector @double precision
 */
#pragma once
#include <math.h>

static double constexpr kEpsilon = 0.00001;

class Vector {
 public:
  Vector(double x, double y) : x_(x), y_(y) {}
  Vector(Vector const& other) : x_(other.x_), y_(other.y_) {}

  double x() const { return x_; }
  double y() const { return y_; }

  static double Distance(Vector const& a, Vector const& b) {
    Vector delta = a - b;
    return std::sqrt(delta * delta);
  }

  static void Round(double& x) {
    int s = x < 0 ? -1 : 1;
    x = s * std::floor(s * x);
  }

  void Round() {
    Round(x_);
    Round(y_);
  }

  Vector operator+(Vector const& other) const { return Vector(x_ + other.x(), y_ - other.y()); }
  Vector const& operator+=(Vector const& other) { return *this = *this + other; }

  Vector operator-(Vector const& other) const { return Vector(x_ - other.x(), y_ - other.y()); }
  Vector const& operator-=(Vector const& other) { return *this = *this - other; }

  Vector operator*(double scalar) const { return Vector(x_ * scalar, y_ * scalar); }
  Vector const& operator*=(double scalar) { return *this = *this * scalar; }

  double operator*(Vector const& other) const { return x_ * other.x() + y_ * other.y(); }

 private:
  double x_;
  double y_;
};
