#pragma once
#include <random>
#include <string>

class RandomStreamGen {
 private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist_len;
    std::uniform_int_distribution<int> dist_index;
    const std::string CHARACTERS = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";

 public:
    explicit RandomStreamGen(int seed) : gen(seed), dist_len(1, 30), dist_index(0, 62) {}

    std::string next() {
        int len = dist_len(gen);
        std::string result;
        result.resize(len);

        for (int i = 0; i < len; ++i) {
            int index = dist_index(gen);
            result[i] = CHARACTERS[index];
        }
        return result;
    }
};
