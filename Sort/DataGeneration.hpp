
#ifndef _DATA_GENERATION_HPP_
#define _DATA_GENERATION_HPP_

#include "Utils.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <cstddef>
#include <climits>
#include <algorithm>
#include <format>

void writeFile(
    const std::string& filename,
    const std::vector<int>& container
) {
    std::ofstream writer(filename, std::ios::out);
    for (int number: container) {
        writer << number << '\n';
    }
    writer.close();
}

void generate(const std::vector<size_t>& sizes) {
    std::random_device device;
    std::default_random_engine rand_engine(device());
    std::uniform_int_distribution<int> distribute(INT_MIN, INT_MAX);
    for (size_t size: sizes) {
        std::vector<int> container;
        container.reserve(size);
        for (size_t index = 0; index < size; ++index) {
            container.push_back(distribute(rand_engine));
        }
        writeFile(
            std::format("size_{}_random.db", size),
            container
        );
        std::sort(container.begin(), container.end(), std::less<int>());
        writeFile(
            std::format("size_{}_increase.db", size),
            container
        );
        std::sort(container.begin(), container.end(), std::greater<int>());
        writeFile(
            std::format("size_{}_decrease.db", size),
            container
        );
    }
}

void loadData(
    const std::string& filename,
    std::vector<CompareWrapper<int>>& container
) {
    std::ifstream reader(filename, std::ios::in);
    int number;
    while (reader >> number) {
        container.emplace_back(number);
    }
    reader.close();
}

#endif
