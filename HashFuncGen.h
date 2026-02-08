#pragma once
#include <string>
#include <random>
#include <chrono>
#include <cstdint>
#include <climits>

class HashFuncGen {
 private:
    int p;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

 public:
    explicit HashFuncGen(int seed) : gen(seed), dist(0, INT_MAX / 2) {
        p = 2 * dist(gen) + 1;
    }

    size_t operator()(const std::string& str) {
        uint32_t result = 0;
        uint32_t grade = 1;
        for (size_t i = 0; i < str.length(); ++i) {
            result += static_cast<char>(str[i]) * grade;
            grade *= p;
        }
        return result;
    }
};
