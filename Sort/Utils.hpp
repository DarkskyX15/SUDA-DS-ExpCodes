
#ifndef _COMPARE_HPP_
#define _COMPARE_HPP_

#include <cstddef>
#include <utility>
#include <chrono>

template<class _Tp>
class CompareWrapper {
public:
    typedef _Tp InnerType;
    typedef CompareWrapper<_Tp> WrapperType;

private:

    InnerType value;
    static size_t cmp_count;
    static size_t move_count;

public:
    CompareWrapper(const InnerType& wrapped):
    value(wrapped) { }
    CompareWrapper(const WrapperType& other):
    value(other.value) {  }
    CompareWrapper(WrapperType&& other):
    value(std::move(other.value)) {  }

    WrapperType& operator=(const WrapperType& other) {
        value = other.value;
        ++move_count;
        return *this;
    }
    WrapperType& operator=(WrapperType&& other) {
        value = std::move(other.value);
        ++move_count;
        return *this;
    }

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

    static void clearCompare() { cmp_count = 0; }
    static void clearMove() { move_count = 0; }
    static size_t countCompare() { return cmp_count; }
    static size_t countMove() { return move_count; }
    const InnerType& operator*() const { return value; }
};
template<class _Tp>
size_t CompareWrapper<_Tp>::cmp_count = 0;
template<class _Tp>
size_t CompareWrapper<_Tp>::move_count = 0;

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

#endif /* COMPARE_HPP */
