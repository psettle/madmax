/**
 * 2D vector @double precision
 */
#pragma once
#include <math.h>

class Vector {
 public:
  Vector(double x, double y) : x_(x), y_(y){};

  double x() const { return x_; }
  double y() const { return y_; }

  static double Distance(Vector const& a, Vector const& b) {
    double dx = a.x_ - b.x_;
    double dy = a.y_ - b.y_;
    return std::sqrt(dx * dx + dy * dy);
  }

 private:
  double x_;
  double y_;
};
