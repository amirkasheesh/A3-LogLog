#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include "HashFuncGen.h"

class HyperLogLog {
 private:
    std::vector<uint8_t> registers;
    size_t bit;
    HashFuncGen func;
    double coefficient(size_t b) {
        if (b == 1) {
            return 0.3512;
        } else if (b == 2) {
            return 0.5324;
        } else if (b == 3) {
            return 0.6235;
        } else if (b == 4) {
            return 0.673;
        } else if (b == 5) {
            return 0.697;
        } else if (b == 6) {
            return 0.709;
        }
        double degree = 1;
        for (size_t i = 0; i < b; ++i) {
            degree *= 2;
        }
        return 0.7213 / (1 + (1.079 / degree));
    }

 public:
    explicit HyperLogLog(size_t b, int s) : bit(b), func(s) {
        size_t degree = 1;
        for (int i = 0; i < b; ++i) {
            degree *= 2;
        }
        registers.resize(degree);
    }

    void add(const std::string& str) {
        size_t hash = func(str);

        size_t idx = hash >> (32 - bit);
        size_t val = hash << bit;
        uint8_t lz = (val == 0) ? 32 : __builtin_clz(val) + 1;

        registers[idx] = std::max(registers[idx], lz);
    }

    double count() {
        double first = 0.0;
        for (size_t i = 0; i < registers.size(); ++i) {
            double temp_two = 1;
            for (int j = 0; j < registers[i]; ++j) {
                temp_two *= 2;
            }
            first += 1.0 / temp_two;
        }
        double second = coefficient(bit);

        return (1 / first) * second * static_cast<double>(registers.size() * registers.size());
    }
};
