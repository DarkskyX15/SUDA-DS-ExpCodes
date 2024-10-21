
#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include "TunedArray.hpp"
#include <iostream>

template<class T>
class CirQueue {

    friend std::ostream& operator<< (std::ostream& out, const CirQueue<T>& q) {
        if (!q) return out;
        size_t index = q.index_front;
        while (index != q.index_back) {
            out << q.st[index] << ' ';
            ++index;
            if (index >= q.contain_size) index = 0;
        }
        out << q.st[q.index_back];
        return out;
    }

private: 
    ArrayList<T> st;
    size_t index_front, index_back;
    size_t count, contain_size;
public:
    CirQueue():
    st(), index_front(0), index_back(0), count(0), contain_size(0) { }
    CirQueue(size_t reserve_size): st() {
        index_front = 0;
        index_back = 0;
        count = 0;
        contain_size = 0;
        st.reserve(reserve_size);
    }
    CirQueue(std::initializer_list<T> init_list):
    st(init_list), index_front(0), index_back(st.size() - 1),
    count(init_list.size()), contain_size(count) { }

    CirQueue(const CirQueue& q):
    st(q.st), index_front(q.index_front), index_back(q.index_back),
    count(q.count), contain_size(q.contain_size) {}
    CirQueue(CirQueue&& q):
    st(q.st), index_front(q.index_front), index_back(q.index_back),
    count(q.count), contain_size(q.contain_size) {}
    CirQueue<T>& operator=(const CirQueue& q) {
        st = q.st; index_front = q.index_front;
        index_back = q.index_back; 
        count = q.count; contain_size = q.contain_size;
        return *this;
    }
    CirQueue<T>& operator=(CirQueue&& q) {
        st = q.st; index_front = q.index_front;
        index_back = q.index_back; 
        count = q.count; contain_size = q.contain_size;
        return *this;
    }

    T& front() { return st.at(index_front); }
    const T& front() const { return st.at(index_front); }
    T& back() { return st.at(index_back); }
    const T& back() const { return st.at(index_back); }

    void push(const T& element) {
        if (count >= contain_size) {
            if (index_back == contain_size - 1 || contain_size == 0) {
                st.push_back(element);
            } else {
                st.insert(index_back + 1, element);
                if (index_front > index_back) ++index_front;
            }
            if (contain_size) ++index_back;
            ++count;
            ++contain_size;
        } else {
            index_back = (index_back + 1) % contain_size;
            st[index_back] = element;
            ++count;
        }
    }

    T pop() {
        if (index_front == contain_size - 1) {
            T temp(st[index_front]);
            index_front = 0;
            --count;
            return temp;
        } else {
            T temp(st[index_front++]);
            --count;
            return temp;
        }
    }

    operator bool() const { return count == 0 ? false : true; }
    size_t size() const { return count; }
};

#endif /* _QUEUE_HPP_ */
