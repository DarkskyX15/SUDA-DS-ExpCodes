
# ifndef _UNIT_TEST_HPP_
# define _UNIT_TEST_HPP_

#include "TestLib.hpp"
#include "LinkedList.hpp"
#include "ArrayList.hpp"
#include <iostream>

using namespace dsl;

/**
 * 以下是 ArrayList 的测试方法
 */

template<class Element>
class ArrayListTest: public TestMethods<ArrayList<Element>> {
private:
    typedef ArrayList<Element> target;
    static void print(const target& lst) {
        for (auto i: lst) std::cout << i << ' ';
        std::cout << '\n';
    }
public:
    ArrayListTest(): TestMethods<ArrayList<Element>>() { 
        BEGIN_TEST(show) {
            if (!instance.size()) {
                std::cout << "list is now empty.\n";
                return ;
            }
            for (auto i: instance) std::cout << i << ' ';
            std::cout << '\n';
        } END_TEST(show);

        BEGIN_TEST(construct) {
            std::cout << "初始化列表初始化(new_arr)：";
            ArrayList<Element> new_arr{1, 2, 3, 4, 5};
            print(new_arr);

            std::cout << "拷贝构造(copy_arr from new_arr)：";
            ArrayList<Element> copy_arr(new_arr);
            print(copy_arr);

            std::cout << "移动构造(move_arr move from copy_arr)：";
            ArrayList<Element> move_arr(std::move(copy_arr));
            print(move_arr);
            std::cout << "此时原列表内部指针(copy_arr)：" << copy_arr.begin().raw();

            ArrayList<Element> fill_arr(Element(), 10);
            std::cout << "\n默认值构造(fill_arr)：";
            print(fill_arr);
        } END_TEST(construct);

        BEGIN_TEST(iterator) {
            std::cout << "测试迭代器循环：\n";
            ArrayList<Element> iter_arr = {1, 2, 3, 4, 5};
            for (auto iter = iter_arr.begin(); iter != iter_arr.end(); ++iter) {
                std::cout << iter.raw() << "-->" << *iter << '\n';
            }
        } END_TEST(iterator);

        BEGIN_TEST(insert) {
            std::cout << "输入插入位置:\n>>> ";
            int pos; std::cin >> pos;
            std::cout << "输入插入值：\n>>> ";
            int val; std::cin >> val;
            instance.insert(pos, val);
        } END_TEST(insert);

        BEGIN_TEST(push_back) {
            std::cout << "输入插入值：\n>>> ";
            int val; std::cin >> val;
            instance.push_back(val);
        } END_TEST(push_back);

        BEGIN_TEST(clear) {
            instance.clear();
            std::cout << "已清空\n";
        } END_TEST(clear);

        BEGIN_TEST(status) {
            std::cout << "capacity: " << instance.capacity()
                << "\tlength: " << instance.size() << '\n';
        } END_TEST(status);
    }
};

template<class Element>
struct TestTraits<ArrayList<Element>> {
    typedef ArrayListTest<Element> methods;
};

/**
 * 以下是 OrderedArrayList 的测试方法
 */

template<class Element>
class OrderedArrayListTest: public TestMethods<OrderedArrayList<Element>> {
private:
    typedef OrderedArrayList<Element> target;
    static void print(const target& lst) {
        for (auto num: lst) std::cout << num << ' ';
        std::cout << '\n';
    }
public:
    OrderedArrayListTest(): TestMethods<target>() {
        BEGIN_TEST(show) {
            if (!instance.size()) {
                std::cout << "list is now empty.\n";
                return ;
            }
            print(instance);
        } END_TEST(show);

        BEGIN_TEST(insert) {
            std::cout << "输入要插入的值：\n>>> ";
            int x; std::cin >> x;
            instance.insert(x);
        } END_TEST(insert);

        BEGIN_TEST(build) {
            target test_ = {5, 3, 2, 1, 1, 7};
            std::cout << "测试初始化({5, 3, 2, 1, 1, 7})：\n";
            instance = std::move(test_);
            print(instance);
        } END_TEST(build);

        BEGIN_TEST(replace) {
            std::cout << "输入要替换的目标：\n>>> ";
            int x; std::cin >> x;
            std::cout << "替换为：\n>>> ";
            int r; std::cin >> r;
            instance.replace(x, r);
        } END_TEST(replace);

        BEGIN_TEST(status) {
            std::cout << "capacity:" << instance.capacity() << ' '
                    << "length:" << instance.size() << '\n';
        } END_TEST(status);
    }
};

template<class Element>
struct TestTraits<OrderedArrayList<Element>> {
    typedef OrderedArrayListTest<Element> methods;
};

/**
 * 以下是 MonoLinkedList 的测试方法
 */

template<class Tp>
class MonoLinkedListTest: public TestMethods<MonoLinkedList<Tp>> {
private:
    typedef MonoLinkedList<Tp> target;
    static void print(const target& lst) {
        for (auto ele: lst) std::cout << ele << ' ';
        std::cout << '\n';
    }
    static void status(const target& lst) {
        std::cout << "size: " << lst.size() << '\n';
    }
public:
    MonoLinkedListTest(): TestMethods<target>() {
        BEGIN_TEST(construct) {
            std::cout << "初始化列表构造{1, 3, 45, 8, 7}：";
            MonoLinkedList<Tp> init_lst = {1, 3, 45, 8, 7};
            print(init_lst); status(init_lst);
            instance = std::move(init_lst);
        } END_TEST(construct);

        BEGIN_TEST(push_back) {
            std::cout << "插入值：\n>>> ";
            Tp x; std::cin >> x;
            instance.push_back(x);
        } END_TEST(push_back);

        BEGIN_TEST(insert) {
            std::cout << "向begin()后插入，输入值:\n>>> ";
            int x; std::cin >> x;
            instance.insert(instance.begin(), x);
        } END_TEST(insert);

        BEGIN_TEST(remove) {
            std::cout << "删除begin()后元素。\n";
            instance.remove(instance.begin());
        } END_TEST(remove);

        BEGIN_TEST(reverse) {
            instance.reverse();
            std::cout << "已反转。\n";
        } END_TEST(reverse);

        BEGIN_TEST(status) {
            status(instance);
        } END_TEST(status);

        BEGIN_TEST(show) {
            print(instance);
        } END_TEST(show);
    }
};

template<class Tp>
struct TestTraits<MonoLinkedList<Tp>> {
    typedef MonoLinkedListTest<Tp> methods;
};

/**
 * 以下是 DualLinkedList 的测试方法
 */

template<class Tp>
class DualLinkedListTest: public TestMethods<DualLinkedList<Tp>> {
private:
    typedef DualLinkedListTest<Tp> self;
    typedef DualLinkedList<Tp> target;
    static void print(const target& lst) {
        for (auto num: lst) std::cout << num << ' ';
        std::cout << '\n';
    }
    static void status(const target& lst) {
        std::cout << "size: " << lst.size() << '\n';
    }
public:
    DualLinkedListTest(): TestMethods<target>() {
        BEGIN_TEST(insert) {
            std::cout << "插入至++lst.begin(), 输入值：\n>>> ";
            Tp x; std::cin >> x;
            instance.insert(++instance.begin(), x);
        } END_TEST(insert);

        BEGIN_TEST(construct) {
            std::cout << "初始化列表构造{1, 2, 3, 4, 5, 6}: ";
            target init_lst = {1, 2, 3, 4, 5, 6};
            print(init_lst); status(init_lst);
            instance = std::move(init_lst);
        } END_TEST(construct);

        BEGIN_TEST(remove) {
            std::cout << "删除元素(pos=++lst.rbegin())。";
            instance.remove(++instance.rbegin());
        } END_TEST(remove);

        BEGIN_TEST(reverse) {
            instance.reverse();
            std::cout << "已反转。";
        } END_TEST(reverse);

        BEGIN_TEST(show) {
            print(instance);
        } END_TEST(show);

        BEGIN_TEST(status) {
            status(instance);
        } END_TEST(status);

        BEGIN_TEST(push_front) {
            std::cout << "输入值：\n>>> ";
            Tp x; std::cin >> x;
            instance.push_front(x);
        } END_TEST(push_front);

        BEGIN_TEST(push_back) {
            std::cout << "输入值:\n>>> ";
            Tp x; std::cin >> x;
            instance.push_back(x);
        } END_TEST(push_back);
    }
};

template<class Tp>
struct TestTraits<DualLinkedList<Tp>> {
    typedef DualLinkedListTest<Tp> methods;
};

#endif /* _UNIT_TEST_HPP_ */
