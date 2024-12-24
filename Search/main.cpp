#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <limits>

template<class _Tp>
class CompareWrapper {
public:
    typedef _Tp InnerType;
    typedef CompareWrapper<_Tp> WrapperType;

private:

    InnerType value;
    static size_t cmp_count;

public:
    CompareWrapper(const InnerType& wrapped):
    value(wrapped) { }

    bool operator== (const WrapperType& other) const {
        ++cmp_count;
        return this->value == other.value;
    }
    bool operator!= (const WrapperType& other) const {
        return !operator==(other);
    }

    bool operator> (const WrapperType& other) const {
        ++cmp_count;
        return this->value > other.value;
    }
    bool operator<= (const WrapperType& other) const {
        return !operator>(other);
    }

    bool operator< (const WrapperType& other) const {
        ++cmp_count;
        return this->value < other.value;
    }
    bool operator>= (const WrapperType& other) const {
        return !operator<(other);
    }

    static void clear() { cmp_count = 0; }
    static size_t count() { return cmp_count; }
    const InnerType& operator*() const { return value; }
};
template<class _Tp>
size_t CompareWrapper<_Tp>::cmp_count = 0;

namespace timer {

typedef std::chrono::microseconds us;
typedef std::chrono::milliseconds ms;
typedef std::chrono::nanoseconds ns;
typedef std::chrono::seconds s;

template <class TimeInterval>
class Timer {
private:
    typedef std::chrono::time_point<std::chrono::steady_clock> timetick;

public:
    Timer() {  }

    inline void start() {
        t_start = std::chrono::steady_clock::now();
    }
    inline size_t end() {
        t_end = std::chrono::steady_clock::now();
        interval = std::chrono::duration_cast<TimeInterval>(t_end - t_start);
        return interval.count();
    }

    private:
    timetick t_start, t_end;
    TimeInterval interval;
};

}

timer::Timer<timer::ns> tick_timer;

void orderedQuery(
    const std::vector<CompareWrapper<int>>& array,
    const std::vector<CompareWrapper<int>>& query,
    const char* name
) {
    size_t duration, count, pos;
    double size = query.size();

    CompareWrapper<int>::clear();
    tick_timer.start();
    for (size_t i = 0; i < query.size(); ++i) {
        pos = std::numeric_limits<size_t>::max();
        for (size_t index = 0; index < array.size(); ++index) {
            if (array[index] == query[i]) {
                pos = index;
                break;
            }
        }
    }
    duration = tick_timer.end();
    count = CompareWrapper<int>::count();
    std::cout << '[' << name << "]:\n查找 " << query.size() << " 次耗时："
        << duration << " ns.\n平均比较次数：" << 
        static_cast<double>(count) / size << " 次\n";
}

void binaryQuery(
    const std::vector<CompareWrapper<int>>& array,
    const std::vector<CompareWrapper<int>>& query,
    const char* name
) {
    size_t duration, count;
    double size = query.size();

    CompareWrapper<int>::clear();
    tick_timer.start();
    for (auto& q: query) {
        size_t left = 0;
        size_t right = array.size() - 1;
        size_t pos = std::numeric_limits<size_t>::max();
        while (left < right) {
            size_t mid = (left + right) >> 1;
            if (array[mid] < q) left = mid + 1;
            else right = mid;
        }
    }
    duration = tick_timer.end();
    count = CompareWrapper<int>::count();
    std::cout << '[' << name << "]:\n查找 " << query.size() << " 次耗时："
        << duration << " ns.\n平均比较次数：" << 
        static_cast<double>(count) / size << " 次\n";

}

size_t _recursiveQuery(
    const std::vector<CompareWrapper<int>>& arr,
    CompareWrapper<int> target,
    size_t left, size_t right
) {
    if (left >= right) return std::numeric_limits<size_t>::max();
    size_t mid = (left + right) >> 1;
    if (arr[mid] < target) 
        return _recursiveQuery(arr, target, mid + 1, right);
    else 
        return _recursiveQuery(arr, target, left, mid);
}

void recursiveBinaryQuery(
    const std::vector<CompareWrapper<int>>& array,
    const std::vector<CompareWrapper<int>>& query,
    const char* name
) {
    size_t duration, count, pos;
    double size = query.size();

    CompareWrapper<int>::clear();
    tick_timer.start();
    for (auto& q: query) {
        pos = _recursiveQuery(array, q, 0, array.size() - 1);
    }
    duration = tick_timer.end();
    count = CompareWrapper<int>::count();
    std::cout << '[' << name << "]:\n查找 " << query.size() << " 次耗时："
        << duration << " ns.\n平均比较次数：" << 
        static_cast<double>(count) / size << " 次\n";
}

int main() {
    std::random_device device;
    std::default_random_engine rnd(device());
    std::cout << std::fixed << std::setprecision(3);

    size_t n;
    std::cin >> n;

    std::vector<CompareWrapper<int>> success, fail;
    for (size_t i = 0; i <= n - 1; ++i) {
        success.emplace_back(i * 2 + 1);
        fail.emplace_back(i * 2);
    }

    size_t m;
    std::cin >> m;

    std::uniform_int_distribution<size_t> range(0, n - 1);
    std::vector<CompareWrapper<int>> success_query, fail_query;
    for (size_t i = 0; i < m; ++i) {
        size_t index = range(rnd);
        success_query.push_back(success[index]);
        index = range(rnd);
        fail_query.push_back(fail[index]);
    }

    orderedQuery(success, success_query, "顺序成功查找");
    orderedQuery(success, fail_query, "顺序失败查找");
    std::cout << "=====================================\n";

    binaryQuery(success, success_query, "二分成功查找");
    binaryQuery(success, fail_query, "二分失败查找");
    std::cout << "=====================================\n";

    recursiveBinaryQuery(success, success_query, "递归二分成功查找");
    recursiveBinaryQuery(success, fail_query, "递归二分失败查找");
    std::cout << "=====================================\n";

    return 0;
}