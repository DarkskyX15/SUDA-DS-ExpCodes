/*
 * @Date: 2024-09-14 14:24:26
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-25 17:41:04
 */

#ifndef _TEST_LIB_HPP_
#define _TEST_LIB_HPP_

#define BEGIN_TEST(cmd) auto lbd_func_##cmd=[](target& instance)->void
#define END_TEST(cmd) ;this->cmd_map.emplace(#cmd, lbd_func_##cmd)

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>

namespace dsl {

template<class Target>
class TestMethods {
private:
    typedef std::unordered_map<std::string, std::function<void(Target&)>> umap;
protected:
    umap cmd_map;
public:
    TestMethods(): cmd_map() {}
    TestMethods(const TestMethods& methods) = delete;
    const umap& getRef() const { return this->cmd_map; }
};

template<class Tp>
struct TestTraits {};

template<class Type, class TM = TestTraits<Type>>
class TestInstance {
private:
    typedef std::unordered_map<std::string, std::function<void(Type&)>> umap;
    const umap& map_ref;
    Type instance;
    typename TM::methods test_methods;
public:
    TestInstance():
    instance(),
    test_methods(),
    map_ref(test_methods.getRef()) {}

    template<class ...Args>
    TestInstance(Args ...args):
    instance(std::forward<Args>(args)...), 
    test_methods(), 
    map_ref(test_methods.getRef()) {};

    TestInstance(const TestInstance&) = delete;
    void test(const std::string& cmd) {
        auto iter = map_ref.find(cmd);
        if (iter == map_ref.end()) {
            std::cout << "Unknown test command: " << cmd << '\n';
        } else {
            std::cout << "Test: " << cmd << '\n';
            iter->second(instance);
        }
    }
    void show() const {
        std::cout << "Available test commands: \n";
        for (auto& iter : map_ref) {
            std::cout << iter.first << '\n';
        }
    }
    void all() {
        std::cout << "Performing all available tests: \n";
        for (auto& iter: map_ref) {
            std::cout << "\nTest -> " << iter.first << ": \n";
            iter.second(instance);
        }
    }
};

} // namespace dsl

#endif /* _TEST_LIB_HPP_ */
