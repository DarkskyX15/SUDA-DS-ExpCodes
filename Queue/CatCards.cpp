/*
 * @Date: 2024-10-14 15:14:30
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-21 15:51:36
 */
#include "Queue.hpp"
#include <iostream>
#include <random>

void printStatus(CirQueue<int>& ap, CirQueue<int>& bp, ArrayList<int>& t) {
    std::cout << "Cards on desk: ";
    for (auto i : t) std::cout << i << ' ';
    std::cout << "\nCards in A's hands: " << ap ;
    std::cout << "\nCards in B's hands: " << bp << '\n' ;
    std::cout << "-------------------------\n";
}

void collect(ArrayList<int>& t, CirQueue<int>& q, int target) {
    bool found = false;
    int index;
    for (index = 0; index < t.size() - 1; ++index) {
        if (t[index] == target) {
            found = true;
            break;
        }
    }
    if (!found) return ;
    for (int i = index; i < t.size(); ++i) q.push(t[i]);
    t.resize(index);
}

int main() {
    
    CirQueue<int> a_pool, b_pool;
    ArrayList<int> table;
    std::random_device rand_source;
    std::mt19937 mt_rand(rand_source());
    std::uniform_int_distribution<int> dis(1, 10);

    int n = 5;
    for (int i = 0; i < n; ++i) {
        a_pool.push(dis(mt_rand));
        b_pool.push(dis(mt_rand));
    }

    printStatus(a_pool, b_pool, table);
    int pa, pb, index;
    while(a_pool && b_pool) {
        table.push_back(pa = a_pool.pop());
        collect(table, a_pool, pa);
        table.push_back(pb = b_pool.pop());
        collect(table, b_pool, pb);
        printStatus(a_pool, b_pool, table);
    }

    return 0;
}