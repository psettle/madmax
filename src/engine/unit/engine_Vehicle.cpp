/**
 * Vehicle unit base class
 */
#include "engine_Vehicle.hpp"

namespace engine {

static double kImpulseCoeff = 0.5;
static double kMinImpulse = 30.0;

void Vehicle::Thrust(Vector const& target, double power) {
  double distance = Vector::Distance(position(), target);

  if (std::abs(distance) <= kEpsilon) {
    return;
  }

  double coef = power / (mass() * distance);

  velocity_ += (target - position()) * coef;
}

void Vehicle::RunTime(double t) { Move(velocity_ * t); }

void Vehicle::Bounce(Vehicle& a, Vehicle& b) {
  double mass_a = a.mass();
  double mass_b = b.mass();
  double mass_coeff = (mass_a + mass_b) / (mass_a * mass_b);

  Vector distance = a.position() - b.position();
  double distance2 = distance * distance;

  Vector deltav = a.velocity() - b.velocity();
  double product = (distance * deltav) / (distance2 * mass_coeff);

  Vector force = distance * product;

  // Update speeds
  a.velocity_ -= force * (1 / mass_a);
  b.velocity_ += force * (1 / mass_b);

  force *= kImpulseCoeff;

  double impulse = std::sqrt(force * force);
  if (impulse > kEpsilon && impulse < kMinImpulse) {
    force *= (kMinImpulse / impulse);
  }

  a.velocity_ -= force * (1 / mass_a);
  b.velocity_ += force * (1 / mass_b);

  // Check if objects intersect and fix
  double diff = (Vector::Distance(a.position(), b.position()) - a.radius() - b.radius()) / 2;
  if (diff <= 0) {
    a.Move(b.position(), diff - kEpsilon);
    b.Move(a.position(), diff - kEpsilon);
  }
}

void Vehicle::Bounce(Vehicle& v, Vector const& centre, double radius) {
  double mass = v.mass();
  double mass_coeff = 1 / mass;
  double distance2 = v.position() * v.position();
  double product = (v.position() * v.velocity()) / (distance2 * mass_coeff);

  Vector force = v.position() * product;

  // Update speeds
  v.velocity_ -= force * mass_coeff;

  force *= kImpulseCoeff;

  double impulse = std::sqrt(force * force);
  if (impulse > kEpsilon && impulse < kMinImpulse) {
    force *= (kMinImpulse / impulse);
  }

  v.velocity_ -= force * mass_coeff;

  // Check if objects intersect and fix
  double diff = (Vector::Distance(v.position(), centre) - v.radius() - radius) / 2;
  if (diff <= 0) {
    v.Move(centre, diff + kEpsilon);
  }
}

}  // namespace engine
