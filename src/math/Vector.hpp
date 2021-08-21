/**
 * 2D vector @double precision
 */
#pragma once
#include <math.h>

class Vector {
 public:
  Vector(double x, double y) : x_(x), y_(y) {}
  Vector(Vector const& other) : x_(other.x_), y_(other.y_) {}

  double x() const { return x_; }
  double y() const { return y_; }

  static double Distance(Vector const& a, Vector const& b) {
    double dx = a.x_ - b.x_;
    double dy = a.y_ - b.y_;
    return std::sqrt(dx * dx + dy * dy);
  }

  static void Round(double& x) {
    int s = x < 0 ? -1 : 1;
    x = s * std::floor(s * x);
  }

  void Round() {
    Round(x_);
    Round(y_);
  }

 private:
  double x_;
  double y_;
};
