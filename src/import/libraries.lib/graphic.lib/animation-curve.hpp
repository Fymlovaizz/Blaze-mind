#ifndef __BM_LIBRARIES_GRAPHIC_ANIMATION_CURVE_HPP__
#define __BM_LIBRARIES_GRAPHIC_ANIMATION_CURVE_HPP__

#include <functional>
#include <cmath>

namespace BM {
    namespace graphic {
        float None(float t) { return t == 0.0f ? 0.0f : 1.0f; }

        float Stepped(float t) { return t < 0.5f ? 0.0f : 1.0f; }

        float Linear(float t) {  return t; }

        float Ease(float t) { return t * t; }

        float Circle(float t) { return 1.0f - std::sqrt(1.0f - t * t); }

        float Elastic(float t) { return std::pow(2.0f, -10.0f * t) * std::sin((t - 0.075f) * (2.0f * M_PI) / 0.3f) + 1.0f; }

        float Back(float t) { return t * t * ((1.70158f + 1) * t - 1.70158f); }

        float Bounce(float t) {
            if (t < 1 / 2.75f) {
                return 7.5625f * t * t;
            } 
            else if (t < 2 / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            } 
            else if (t < 2.5 / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            } 
            else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        }

        float Exponential(float t) { return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f)); }

        float Wave(float t) { return t * (4 * t * (2 * t - 3) + 5); }

        float Warp(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

        float transform(const std::function<float(float)>& curve, float time, bool inverse = false, bool reverse = false, bool debug = true) {
            if (time < 0.0f || time > 1.0f) {
                #pragma region Developing
                if (debug) return -1.0f;
                else time = time < 0.0f ? 0.0f : 1.0f;
            }

            if (inverse) time = 1.0f - time;
            return inverse ^ reverse ? 1.0f - curve(time) : curve(time);
        }
    }
}
#endif