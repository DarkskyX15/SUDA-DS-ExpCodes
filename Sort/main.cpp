
#include "Utils.hpp"
#include "SortAlgo.hpp"
#include "DataGeneration.hpp"

#include <iostream>
#include <cstddef>
#include <vector>
#include <functional>
#include <format>
#include <string>
#include <unordered_map>

timer::Timer<timer::ns> tick_timer;
struct RunRecord {
    size_t duration;
    size_t compare;
    size_t move;
    std::string sort_name;
    size_t data_type;
    size_t data_size;

    RunRecord(
        size_t _duration,
        size_t _compare,
        size_t _move,
        size_t _data_size,
        size_t _data_type,
        const std::string& _sort_name
    ):
        duration(_duration), compare(_compare),
        move(_move), sort_name(_sort_name), 
        data_type(_data_type), data_size(_data_size)
    { }
};

typedef std::function<std::string(std::vector<CompareWrapper<int>>&)>
    SortFunction;

RunRecord runSort(
    std::vector<CompareWrapper<int>> container,
    const SortFunction& sortFunc,
    size_t data_type, size_t data_size
) {
    CompareWrapper<int>::clearCompare();
    CompareWrapper<int>::clearMove();
    tick_timer.start();
    auto name = sortFunc(container);
    size_t duration = tick_timer.end();
    return RunRecord(
        duration,
        CompareWrapper<int>::countCompare(),
        CompareWrapper<int>::countMove(),
        data_size,
        data_type,
        name
    );
}

int main() {

    std::vector<std::string> data_types = {
        "Increase", "Decrease", "Random"
    };
    std::vector<size_t> size_list = {50, 500, 5000};
    std::vector<SortFunction> sort_list = {
        quickSort, bubbleSort, insertionSort,
        selectionSort, mergeSort, shellSort
    };
    std::vector<RunRecord> records;
    
    for (size_t j = 0; j < size_list.size(); ++j) {
        for (size_t i = 0; i < data_types.size(); ++i) {
            std::vector<CompareWrapper<int>> container;
            loadData(
                std::format("size_{}_{}.db", size_list[j], data_types[i]),
                container
            );
            for (const SortFunction& sortFunc: sort_list) {
                auto record = runSort(container, sortFunc, i, j);
                records.push_back(record);
            }
        }
    }

    std::vector<std::vector<RunRecord*>> dt_group;
    dt_group.resize(data_types.size());
    for (RunRecord& record: records) {
        dt_group[record.data_type].push_back(&record);
    }

    for (size_t i = 0; i < data_types.size(); ++i) {
        std::cout << 
            std::format("\n========= Data type: {} ==========\n", data_types[i]);
        std::vector<std::vector<RunRecord*>> size_group;
        size_group.resize(size_list.size());
        for (RunRecord* record: dt_group[i]) {
            size_group[record->data_size].push_back(record);
        }
        for (size_t j = 0; j < size_list.size(); ++j) {
            std::cout << "\nArray size: " << size_list[j] <<
                "\nTime rank (ns)\t\t\tCompare rank\t\t\tMove rank\n";
            std::vector<RunRecord*> time_records = size_group[j];
            std::vector<RunRecord*> cmp_records = size_group[j];
            std::vector<RunRecord*> move_records = size_group[j];
            std::sort(
                time_records.begin(), time_records.end(),
                [](RunRecord* left, RunRecord* right) -> bool {
                    return left->duration < right->duration;
                }
            );
            std::sort(
                cmp_records.begin(), cmp_records.end(),
                [](RunRecord* left, RunRecord* right) -> bool {
                    return left->compare < right->compare;
                }
            );
            std::sort(
                move_records.begin(), move_records.end(),
                [](RunRecord* left, RunRecord* right) -> bool {
                    return left->move < right->move;
                }
            );
            for (size_t z = 0; z < time_records.size(); ++z) {
                std::cout << std::left
                    << std::setw(10) << time_records[z]->sort_name << ": " 
                    << std::setw(10) << time_records[z]->duration << "\t\t"
                    << std::setw(10) << cmp_records[z]->sort_name << ": "
                    << std::setw(10) << cmp_records[z]->compare << "\t\t"
                    << std::setw(10) << move_records[z]->sort_name << ": "
                    << std::setw(10) << move_records[z]->move << '\n';
            }
        }
    }

    return 0;
}

