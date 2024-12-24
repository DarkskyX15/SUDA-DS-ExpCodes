
#include <iostream>
#include <string>
#include <stack>
#include "Graph.hpp"

using dsl::graph::defines::UpdateStrategy;
using namespace dsl::graph;

struct Person {
    std::string id_card;
    std::string name;
    size_t tel;

    bool operator==(const Person& p) const { return id_card == p.id_card; }
    bool operator!=(const Person& p) const { return !operator==(p); }
};

// select key from custom data type
template<>
struct utils::key_selector<Person> {
    typedef std::string key_type;
    static constexpr const std::string& key(const Person& p) {
        return p.name;
    }
    static constexpr std::string& key(Person& p) {
        return p.name;
    }
    template<class R1, class R2, class R3>
    static constexpr R2&& arg(
        R1&&  ,
        R2&& s,
        R3&&
    ) { return static_cast<R2&&>(s); }
};

int main() {
    SimpleGraph<
        Person, bool, false, size_t,
        HashListStorage<size_t, bool, false>,
        DefaultIndexProvider<Person, size_t>
    > g;

    std::string card1 = "idCardNo1";
    const std::string name1 = "A";

    // emplace node
    g.emplaceNode(card1, name1, 515);
    g.emplaceNode("idCardNo2", "B", 1145);
    g.emplaceNode("idCardNo3", "C", 515);
    g.emplaceNode("idCardNo4", "D", 1115);
    g.emplaceNode("idCardNo5", "E", 5520);
    g.emplaceNode("idCardNo6", "F", 1220);

    // add edge
    g.addEdgeByKey(name1, "B")
     .addEdgeByKey("B", "F")
    //  .addEdgeByKey("B", "E")
     .addEdgeByKey("B", "C")
     .addEdgeByKey("C", "D")
     .addEdgeByKey("D", "F");

    // walk
    for (auto idx: g.allIndexes()) {
        auto& x = g[idx];
        std::cout << "{ ID Card: " << x.id_card
            << " Name: " << x.name
            << " TEL: " << x.tel
            << " }\n";
    }

    auto start = g.const_access(g.find("A"));
    auto dest = g.const_access(g.find("E"));

    typedef decltype(start) __accessor;
    std::unordered_map<const Person*, const Person*> previous;
    std::unordered_set<size_t> visited;
    std::queue<__accessor> q;
    visited.insert(start.raw());
    previous.emplace(&*start, nullptr);
    q.emplace(start);

    bool solved = false;
    while(!q.empty()) {
        auto& acc = q.front();
        if (acc == dest) {
            solved = true;
            break;
        }
        acc.updateAdjacent(UpdateStrategy::forth);
        auto ptr = &*acc;
        for (auto [idx, vp, wp]: acc.listForth()) {
            if (!visited.contains(idx)) {
                previous.emplace(vp, ptr);
                visited.emplace(idx);
                q.push(acc.next(idx));
            }
        }
        q.pop();
    }

    if (!solved) {
        std::cout << "Can not reach!";
        return 0;
    }

    const Person* walker = &*dest;
    std::stack<const Person*> rev;
    while (walker != nullptr) {
        rev.push(walker);
        walker = previous[walker];
    }

    std::cout << "START";
    while (rev.size()) {
        std::cout << " -> " << rev.top()->name ;
        rev.pop();
    }

    return 0;
}