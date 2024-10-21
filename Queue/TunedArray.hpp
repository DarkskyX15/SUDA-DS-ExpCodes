/*
 * @Date: 2024-09-09 14:16:57
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-14 20:43:59
 */

#ifndef _ARRAY_LIST_HPP_
#define _ARRAY_LIST_HPP_

#include <climits>
#include <initializer_list>
#include <iterator>
#define INITIAL_SIZE 16ULL
#define MAX_ENLARGE_STEP 65536ULL

/**
 * 以下为 ArrayList 的随机访问迭代器实现，以及迭代器特征的补全
 * 定义STL迭代器特征以使容器支持算法库
 */

template <class Element>
class ArrayListRandomAccessIterator {
private:
    typedef ArrayListRandomAccessIterator<Element> self;
    Element* ptr;
public:
    explicit ArrayListRandomAccessIterator(Element* ptr)
    { this->ptr = ptr; }

    // LegacyIterator
    ~ArrayListRandomAccessIterator() = default;
    ArrayListRandomAccessIterator(const self& iter) = default;
    ArrayListRandomAccessIterator(self&& iter) = default;
    self& operator= (self&& iter) = default;
    self& operator= (const self& iter) = default;

    self& operator++ () { ++ptr; return *this; }

    // LegacyOutputIterator
    Element& operator* () const { return *ptr; }
    
    // EqualityComparable
    bool operator== (const self& iter) const {
        return this->ptr == iter.ptr;
    }

    // LegacyInputIterator
    bool operator!= (const self& iter) const {
        return this->ptr != iter.ptr;
    }
    Element* operator-> () const { return ptr; }

    // LegacyForwardIterator
    ArrayListRandomAccessIterator() { ptr = nullptr; }
    self operator++ (int) {
        self temp(*this);
        ++ptr;
        return temp;
    }

    // LegacyBidirectionalIterator
    self& operator-- () { --ptr; return *this; }
    self operator-- (int) {
        self temp(*this);
        --ptr;
        return temp;
    }

    // LegacyRandomAccessIterator
    self& operator+= (std::ptrdiff_t diff) {
        ptr += diff;
        return *this;
    }
    self& operator-= (std::ptrdiff_t diff) {
        ptr -= diff;
        return *this;
    }
    self operator- (std::ptrdiff_t diff) const {
        self temp(*this);
        temp.ptr -= diff;
        return temp;
    }
    self operator+ (std::ptrdiff_t diff) const {
        self temp(*this);
        temp.ptr += diff;
        return temp;
    }
    friend self operator+ (std::ptrdiff_t diff, const self& iter) {
        self temp(iter);
        temp.ptr += diff;
        return temp;
    }
    std::ptrdiff_t operator- (const self& iter) const {
        return static_cast<std::ptrdiff_t>(this->ptr - iter.ptr);
    }
    Element& operator[] (std::ptrdiff_t diff) const { return *(ptr + diff); }
    bool operator< (const self& iter) const {
        return (ptr < iter.ptr);
    }
    bool operator> (const self& iter) const {
        return (ptr > iter.ptr);
    }
    bool operator>= (const self& iter) const {
        return (ptr >= iter.ptr);
    }
    bool operator<= (const self& iter) const {
        return (ptr <= iter.ptr);
    }

    Element* raw() const { return ptr; }
};

template <class Element>
struct std::iterator_traits<ArrayListRandomAccessIterator<Element>> {
    typedef std::random_access_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef Element value_type;
    typedef Element* pointer;
    typedef Element& reference;
};

/**
 * 以下是 ArrayList 定义和实现
 */

template <class Element>
class ArrayList {
protected:
    static Element fallback;
    Element* lst_ptr;
    size_t length;
    size_t capability;

    void enlargeList() {
        size_t step = std::min(capability, MAX_ENLARGE_STEP);
        step += capability;
        if (lst_ptr == nullptr) return ;

        Element *new_ptr = new Element[step]();
        for (size_t i = 0; i < length; ++i) {
            new_ptr[i] = std::move(lst_ptr[i]);
        }
        delete[] lst_ptr;
        lst_ptr = new_ptr;

        capability = step;
    }

public:
    typedef ArrayListRandomAccessIterator<Element> iterator;

    /// @brief 空构造
    ArrayList() {
        lst_ptr = new Element[INITIAL_SIZE]();
        length = 0;
        capability = INITIAL_SIZE;
    }

    /// @brief 以数组初始化
    /// @param array 数组指针 
    /// @param size 数组大小
    ArrayList(const Element array[], size_t size) {
        lst_ptr = new Element[size + INITIAL_SIZE]();
        length = size;
        capability = size + INITIAL_SIZE;
        for (size_t i = 0; i < size; ++i) {
            lst_ptr[i] = array[i];
        }
    }

    /// @brief 以默认值初始化
    /// @param element 默认值
    /// @param size 长度
    ArrayList(const Element& element, size_t size) {
        lst_ptr = new Element[size + INITIAL_SIZE]();
        capability = size + INITIAL_SIZE;
        length = size;
        for (size_t i = 0; i < size; ++i) {
            lst_ptr[i] = element;
        }
    }
    
    /// @brief 以初始化列表初始化
    /// @param init_lst 初始化列表
    ArrayList(const std::initializer_list<Element>& init_lst) {
        length = init_lst.size();
        capability = length + INITIAL_SIZE;
        lst_ptr = new Element[length + INITIAL_SIZE]();
        size_t index = 0;
        for (auto element : init_lst) {
            lst_ptr[index] = element;
            ++index;
        }
    }

    ~ArrayList() {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
            lst_ptr = nullptr;
        }
    }

    /// @brief 复制赋值
    /// @param array_list 列表 
    /// @return *this
    ArrayList& operator= (const ArrayList& array_list) {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
        }
        lst_ptr = new Element[array_list.capability]();
        this->capability = array_list.capability;
        this->length = array_list.length;
        for (size_t i = 0; i < length; ++i) lst_ptr[i] = array_list.lst_ptr[i];
        return *this;
    }
    /// @brief 拷贝构造
    /// @param array_list 列表 
    ArrayList(const ArrayList& array_list):
    lst_ptr(nullptr), capability(0), length(0) {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
        }
        lst_ptr = new Element[array_list.capability]();
        this->capability = array_list.capability;
        this->length = array_list.length;
        for (size_t i = 0; i < length; ++i) lst_ptr[i] = array_list.lst_ptr[i];
    }

    /// @brief 移动赋值
    /// @param array_list 右值列表
    /// @return *this
    ArrayList& operator= (ArrayList&& array_list) {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
        }
        lst_ptr = array_list.lst_ptr;
        this->capability = array_list.capability;
        this->length = array_list.length;
        array_list.lst_ptr = nullptr;
        return *this;
    }
    /// @brief 移动构造
    /// @param array_list 右值列表 
    ArrayList(ArrayList&& array_list):
    lst_ptr(nullptr), capability(0), length(0) {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
        }
        lst_ptr = array_list.lst_ptr;
        this->capability = array_list.capability;
        this->length = array_list.length;
        array_list.lst_ptr = nullptr;
    }

    bool push_back(const Element& element) {
        bool resized = false;
        if (length >= capability) {
            enlargeList();
            resized = true;
        }
        lst_ptr[length] = element;
        ++length;
        return resized;
    }

    /// @brief 预留空间
    /// @param size 空间大小
    void reserve(size_t size) {
        if (size <= length) return ;
        if (lst_ptr == nullptr) return ;
        Element* new_ptr = new Element[size]();
        for (size_t i = 0; i < length; ++i) {
            new_ptr[i] = lst_ptr[i];
        }
        delete[] lst_ptr;
        lst_ptr = new_ptr;
        capability = size;
    }

    bool resize(size_t size) {
        size_t save = size;
        size = std::min(size, ULLONG_MAX - INITIAL_SIZE);
        size += INITIAL_SIZE / 2;
        if (size <= capability) {
            length = save;
            return false;
        } else {
            enlargeList();
            length = save;
            return true;
        }
    }

    /// @brief 从列表中移除指定数量的元素
    /// @param pos 开始位置
    /// @param cnt 移除数量
    bool remove(size_t pos, size_t cnt) {
        if (pos >= this->length || cnt > this->length) return false;
        if (pos + cnt - 1 >= this->length) return false;
        for (size_t i = pos; i + cnt < this->length; ++i) {
            lst_ptr[i] = lst_ptr[i + cnt];
        }
        return this->resize(this->length - cnt);
    }
    /// @brief 从列表中移除指定数量的元素
    /// @param iter 开始迭代器
    /// @param cnt 移除数量
    bool remove(iterator iter, size_t cnt) {
        auto pos = static_cast<size_t>(iter - this->begin());
        return this->remove(pos, cnt);
    }

    /// @brief 清空列表
    void clear() {
        if (lst_ptr != nullptr) {
            delete[] lst_ptr;
        }
        length = 0;
        capability = INITIAL_SIZE;
        lst_ptr = new Element[INITIAL_SIZE]();
    }

    /// @brief 插入元素
    /// @param pos 下标
    /// @param ele 元素
    void insert(size_t pos, const Element& ele) {
        if (pos >= this->length) return;
        this->resize(this->length + 1);
        for (size_t i = this->length - 1; i > pos; --i) {
            lst_ptr[i] = lst_ptr[i - 1];
        }
        lst_ptr[pos] = ele;
    }
    /// @brief 插入元素
    /// @param pos 迭代器
    /// @param ele 元素
    void insert(iterator pos, const Element& ele) {
        if (pos >= this->end()) return ;
        size_t cnt = this->end() - pos;
        this->push_back(ele);
        auto iter = this->end().raw();
        while (cnt--) {
            iter -= 1;
            auto temp = *iter;
            *iter = *(iter - 1);
            *(iter - 1) = temp;
        }
    }

    Element& operator[] (size_t index) { return this->at(index); }
    Element& at(size_t index) {
        if (index >= length) return fallback;
        return lst_ptr[index];
    }
    iterator begin() const { return iterator(lst_ptr); }
    iterator end() const { return iterator(lst_ptr + length); }
    iterator back() const { return iterator(lst_ptr + length - 1); }

    // 常函数

    size_t size() const { return this->length; }
    size_t capacity() const { return this->capability; }
    const Element& operator[] (size_t index) const { return this->at(index); }
    const Element& at(size_t index) const {
        if (index >= length) return fallback;
        return lst_ptr[index];
    }

};
template<class Element>
Element ArrayList<Element>::fallback;


#endif /* _ARRAY_LIST_HPP_ */