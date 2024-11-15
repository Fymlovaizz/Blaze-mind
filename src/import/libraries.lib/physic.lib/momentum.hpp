#ifndef __BM_LIBRARIES_PHYSIC_MOMENTUM_HPP__
#define __BM_LIBRARIES_PHYSIC_MOMENTUM_HPP__

#include <libraries.lib/math.lib/vector.hpp>

namespace BM {
namespace physic {

// Velocity: float
// Mass: float
inline void momentum2(BM::math::Vector2<float>& velocity_A, float mass_A, BM::math::Vector2<float>& velocity_B, float mass_B, bool for_all = true) {
    BM::math::Vector2<float> un = !(velocity_B - velocity_A);
    BM::math::Vector2<float> ut = ~un;

    float v1n = un << velocity_A;
    float v2n = un << velocity_B;

    velocity_A = un * ((v1n * (mass_A - mass_B) + 2 * mass_B * v2n) / (mass_A + mass_B)) + ut * (ut << velocity_A);
    if (for_all)
    velocity_B = un * ((v2n * (mass_B - mass_A) + 2 * mass_A * v1n) / (mass_A + mass_B)) + ut * (ut << velocity_B);
}

// Velocity: double
// Mass: double
inline void momentum2(BM::math::Vector2<double>& velocity_A, double mass_A, BM::math::Vector2<double>& velocity_B, double mass_B, bool for_all = true) {
    BM::math::Vector2<double> un = !(velocity_B - velocity_A);
    BM::math::Vector2<double> ut = ~un;

    double v1n = un << velocity_A;
    double v2n = un << velocity_B;

    velocity_A = un * ((v1n * (mass_A - mass_B) + 2 * mass_B * v2n) / (mass_A + mass_B)) + ut * (ut << velocity_A);
    if (for_all)
    velocity_B = un * ((v2n * (mass_B - mass_A) + 2 * mass_A * v1n) / (mass_A + mass_B)) + ut * (ut << velocity_B);
}

// Velocity: double
// Mass: float
inline void momentum2(BM::math::Vector2<double>& velocity_A, float mass_A, BM::math::Vector2<double>& velocity_B, float mass_B, bool for_all = true) {
    BM::math::Vector2<double> un = !(velocity_B - velocity_A);
    BM::math::Vector2<double> ut = ~un;

    double v1n = un << velocity_A;
    double v2n = un << velocity_B;

    velocity_A = un * ((v1n * (mass_A - mass_B) + 2 * mass_B * v2n) / (mass_A + mass_B)) + ut * (ut << velocity_A);
    if (for_all)
    velocity_B = un * ((v2n * (mass_B - mass_A) + 2 * mass_A * v1n) / (mass_A + mass_B)) + ut * (ut << velocity_B);
}

}
}

#endif
