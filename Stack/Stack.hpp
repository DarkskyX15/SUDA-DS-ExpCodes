/*
 * @Date: 2024-09-30 14:11:52
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-30 15:30:50
 */

#ifndef _STACK_HPP_
#define _STACK_HPP_
#include "TunedArray.hpp"

template<class T>
class Stack {
private:
    typedef ArrayListRandomAccessIterator<T> iterator;
    ArrayList<T> lst;
    iterator top_iter;
public:
    Stack(): lst(), top_iter(lst.back()) {}
    Stack(const std::initializer_list<T>& init_lst):
    lst(init_lst), top_iter(lst.back()) {}
    template<class... Args>
    Stack(Args... args):
    lst(std::forward<Args>(args)...), top_iter(lst.back()) {}

    Stack(const Stack& stack) { lst = stack.lst; top_iter = lst.back(); }
    Stack(Stack&& stack) { lst = std::move(stack.lst); top_iter = stack.top_iter; }
    Stack& operator= (const Stack& stack) {
        lst = stack.lst;
        top_iter = lst.back();
        return *this; 
    }
    Stack& operator= (Stack&& stack) {
        lst = std::move(stack.lst);
        top_iter = stack.top_iter;
        return *this;
    }

    ~Stack() = default;

    void push(const T& element) {
        if (lst.push_back(element)) top_iter = lst.back();
        else ++top_iter;
    }

    T pop() {
        if (empty()) return T();
        T result = *top_iter;
        if (lst.remove(top_iter, 1)) top_iter = lst.back();
        else --top_iter;
        return result;
    }

    operator bool () const { return lst.size() == 0 ? false : true; }
    bool empty() const { return lst.size() == 0 ? true : false; }
    size_t size() const { return lst.size(); }
    T& top() { return *top_iter; }
    const T& const_top() const { return *top_iter; }
};

#endif /* _STACK_HPP_ */
