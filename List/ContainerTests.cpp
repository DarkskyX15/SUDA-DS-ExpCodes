/*
 * @Date: 2024-09-09 14:19:48
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-26 22:00:56
 */

#include <windows.h>
#include "ArrayList.hpp"
#include "LinkedList.hpp"
#include "TestLib.hpp"
#include "UnitTest.hpp"

using namespace dsl;

template<class Tp>
void performTest(const std::string& name) {
    std::cout << "测试对象：" << name << '\n';
    TestInstance<Tp> test_;
    std::string cmd, __all("ALL");
    test_.show();
    while (true) {
        std::cout << "\n>>> ";
        std::cin >> cmd;
        if (!cmd.size()) continue;
        if (cmd == __all) test_.all();
        else if (cmd[0] == 'Q' || cmd[0] == 'q') {
            std::cout << "\n结束测试。\n";
            break;
        } else test_.test(cmd);
    }
}

int main() {
    SetConsoleOutputCP(65001);
    performTest<ArrayList<int>>("顺序表");
    performTest<OrderedArrayList<int>>("有序顺序表");
    performTest<MonoLinkedList<int>>("单链表");
    performTest<DualLinkedList<int>>("双端链表");
    return 0;
}
