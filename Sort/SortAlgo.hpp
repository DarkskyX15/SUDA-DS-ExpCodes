
#ifndef _SORT_ALGO_HPP_
#define _SORT_ALGO_HPP_

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include "Utils.hpp"

std::string bubbleSort(std::vector<CompareWrapper<int>>& container) {
    for (size_t i = 0; i < container.size() - 1; ++i) {
        for (size_t j = 0; j < container.size() - i - 1; ++j) {
            if (container[j] > container[j + 1]) {
                std::swap(container[j], container[j + 1]);
            }
        }
    }
    return "bubble";
}

std::string quickSort(std::vector<CompareWrapper<int>>& container) {
    std::function<void(size_t, size_t)> quickSortImpl = 
    [&quickSortImpl, &container](size_t left, size_t right) -> void {
        if (left >= right) {
            return;
        }
        size_t i = left, j = right;
        while (i < j) {
            while (i < j && container[j] > container[i]) --j;
            if (i != j) {
                std::swap(container[i], container[j]);
                ++i;
            }
            while (i < j && container[i] < container[j]) ++i;
            if (i != j) {
                std::swap(container[i], container[j]);
                --j;
            }
        }
        if (i > 0) quickSortImpl(left, i - 1);
        if (i < right - 1) quickSortImpl(i + 1, right);
    };
    quickSortImpl(0, container.size() - 1);
    return "quick";
}

std::string insertionSort(std::vector<CompareWrapper<int>>& container) {
    for (size_t i = 1; i < container.size(); ++i) {
        CompareWrapper<int> key = container[i];
        size_t j = i - 1;
        size_t npos = std::numeric_limits<size_t>::max();
        while (j != npos && container[j] > key) {
            container[j + 1] = container[j];
            --j;
        }
        if (j == npos) container[0] = key;
        else container[j + 1] = key;
    }
    return "insertion";
}

std::string selectionSort(std::vector<CompareWrapper<int>>& container) {
    for (size_t i = 0; i < container.size() - 1; ++i) {
        size_t min_index = i;
        for (size_t j = i + 1; j < container.size(); ++j) {
            if (container[j] < container[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(container[i], container[min_index]);
        }
    }
    return "selection";
}

std::string mergeSort(std::vector<CompareWrapper<int>>& container) {
    std::function<void(size_t, size_t)> mergeSortImpl = 
    [&mergeSortImpl, &container](size_t left, size_t right) -> void {
        if (left >= right) {
            return;
        }
        size_t mid = (left + right) >> 1;
        mergeSortImpl(left, mid);
        mergeSortImpl(mid + 1, right);
        std::vector<CompareWrapper<int>> temp;
        temp.reserve(right - left + 1);
        size_t i = left, j = mid + 1;
        while (i <= mid && j <= right) {
            if (container[i] < container[j]) {
                temp.push_back(container[i]);
                ++i;
            } else {
                temp.push_back(container[j]);
                ++j;
            }
        }
        while (i <= mid) {
            temp.push_back(container[i]);
            ++i;
        }
        while (j <= right) {
            temp.push_back(container[j]);
            ++j;
        }
        for (size_t index = 0; index < temp.size(); ++index) {
            container[left + index] = temp[index];
        }
    };
    mergeSortImpl(0, container.size() - 1);
    return "merge";
}

std::string shellSort(std::vector<CompareWrapper<int>>& container) {
    for (size_t gap = container.size() >> 1; gap > 0; gap >>= 1) {
        for (size_t i = gap; i < container.size(); ++i) {
            CompareWrapper<int> key = container[i];
            size_t j = i;
            while (j >= gap && container[j - gap] > key) {
                container[j] = container[j - gap];
                j -= gap;
            }
            container[j] = key;
        }
    }
    return "shell";
}

#endif /* _SORT_ALGO_HPP_ */
