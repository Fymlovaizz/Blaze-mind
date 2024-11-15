// Cung cấp các hàm lấy số ngẫu nhiên nhanh chóng

#ifndef __BM_LIBRARIES_MATH_RANDOM_HPP__
#define __BM_LIBRARIES_MATH_RANDOM_HPP__

#include <libraries.lib/math.lib/vector/vector2.hpp>
#include <libraries.lib/math.lib/any.hpp>
#include <iostream>
#include <random>

namespace BM {
namespace math {

namespace random {
    std::random_device rd;              // Sinh số ngẫu nhiên từ phần cứng (nếu khả dụng)
    std::mt19937 gen(rd());             // Bộ sinh số ngẫu nhiên Mersenne Twister

    std::uniform_real_distribution<> dist_float_0_1(0.0, 1.0); // Phân phối số thực từ 0 đến 1
}

bool getRandomBool(float probability = 0.5f, bool warning = false) {
    if (warning && (probability < 0.0f || probability > 1.0f)) {
        throw std::invalid_argument("Probability must be between 0 and 1.");
    }

    return random::dist_float_0_1(random::gen) < probability;
}

float getRandom_0_1() {
    return random::dist_float_0_1(random::gen);
}

float getRandomFloat(float min, float max, bool warning = false) {
    if (min > max) {
        if (warning) throw std::invalid_argument("Min cannot be greater than Max.");
        else {
            float c = max;
            max = min;
            min = max;
        }
    }

    std::uniform_real_distribution<float> dist(min, max);
    return dist(random::gen);
}

int getRandomInt(int min, int max, bool warning = false) {
    if (min > max) {
        if (warning) throw std::invalid_argument("Min cannot be greater than Max.");
        else {
            max = max ^ min;
            min = max ^ min;
            max = max ^ min;
        }
    }

    std::uniform_int_distribution<> dist(min, max);
    return dist(random::gen);
}

Vector2<float> getRandomGeneticVector_Float(const Vector2<float>& baseVector, float mutationLevel = 5.f) {
    return { baseVector.x + getRandomFloat(-mutationLevel, mutationLevel), baseVector.y + getRandomFloat(-mutationLevel, mutationLevel) };
}

#ifdef SFML_COLOR_HPP
sf::Color getRandomGeneticColor(const sf::Color& baseColor, int mutationLevel = 5, bool changeAlpha = true, int minAlpha = 90) {
    // Đảm bảo mức độ đột biến không vượt giới hạn
    if (mutationLevel < 0) mutationLevel = 0;
    if (mutationLevel > 127) mutationLevel = 127;

    // Sinh giá trị ngẫu nhiên với giới hạn mức đột biến
    auto mutate = [mutationLevel](int baseValue) -> int {
        int mutation = getRandomInt(-mutationLevel, mutationLevel); // [-mutationLevel, +mutationLevel]
        int newValue = baseValue + mutation;
        if (newValue < 0) newValue = 0;      // Giới hạn trong khoảng [0, 255]
        if (newValue > 255) newValue = 255; // Giới hạn trong khoảng [0, 255]
        return newValue;
    };

    // Tạo màu mới bằng cách đột biến từng kênh (R, G, B, A)
    return sf::Color(
        mutate(baseColor.r), // Đột biến kênh đỏ (Red)
        mutate(baseColor.g), // Đột biến kênh xanh lá (Green)
        mutate(baseColor.b), // Đột biến kênh xanh dương (Blue)
        max(minAlpha, changeAlpha? mutate(baseColor.a): baseColor.a)  // Đột biến độ trong suốt (Alpha)
    );
}
#endif

}
}

#endif
