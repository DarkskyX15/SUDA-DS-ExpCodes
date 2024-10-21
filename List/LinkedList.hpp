/*
 * @Date: 2024-09-14 15:57:45
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-26 20:22:02
 */

#ifndef _LINKED_LIST_HPP_
#define _LINKED_LIST_HPP_

#include <initializer_list>
#include <iterator>

/**
 * 以下为 MonoLinkedList 的定义和实现
 * 以及其所需的工具实现
 */

template<class T>
struct MonoLinkedListNode {
    T data_field;
    MonoLinkedListNode<T>* next;
};

template<class T>
class MLLForwardIterator {
private:
    typedef MLLForwardIterator<T> self;
    typedef MonoLinkedListNode<T> node;
protected:
    node* ptr;
public:
    MLLForwardIterator(node* ptr) { this->ptr = ptr; }
    node* raw() const { return ptr; }
    
    // LegacyIterator
    MLLForwardIterator(const self& iter) = default;
    self& operator= (const self& iter) = default;
    ~MLLForwardIterator() = default;
    T& operator* () const { return ptr->data_field; }
    self& operator++ () {
        if (ptr != nullptr) ptr = ptr->next;
        return *this;
    }

    // EqualityComparable
    bool operator== (const self& iter) const { return ptr == iter.ptr; }

    // LegacyInputIterator
    bool operator!= (const self& iter) const { return ptr != iter.ptr; }
    T* operator-> () { return &(ptr->data_field); }

    // LegacyForwardIterator
    MLLForwardIterator() { ptr = nullptr; }
    self operator++ (int) {
        self temp(*this);
        if (ptr != nullptr) ptr = ptr->next;
        return temp;
    }
};

template<class T>
class LoopedMLLForwardIterator: public MLLForwardIterator<T> {
private:
    typedef LoopedMLLForwardIterator<T> self;
    typedef MonoLinkedListNode<T> node;
    node* head;
    bool loop;
public:
    LoopedMLLForwardIterator(node* ptr, node* head):
    MLLForwardIterator<T>(ptr) {
        this->head = head;
        this->loop = true;
    }

    LoopedMLLForwardIterator():
    MLLForwardIterator<T>(), head(nullptr), loop(true) {}

    LoopedMLLForwardIterator(const self& iter):
    MLLForwardIterator<T>(iter.ptr) {
        this->head = iter.head;
        this->loop = iter.loop;
    }

    self& operator= (const self& iter) {
        this->ptr = iter.ptr;
        this->head = iter.head;
        this->loop = iter.loop;
    }
    ~LoopedMLLForwardIterator() = default;

    bool notReach() const { return this->loop; }
    void reset() { loop = true; }

    self& operator++ () {
        if (this->ptr == nullptr) return *this;
        if (!loop) return *this;
        if (this->ptr->next != nullptr) {
            this->ptr = this->ptr->next;
        } else {
            loop = false;
            this->ptr = this->head;
        }
        return *this;
    }
    self operator++ (int) {
        self value(*this);
        if (this->ptr == nullptr) return value;
        if (!loop) return value;
        if (this->ptr->next != nullptr) {
            this->ptr = this->ptr->next;
        } else {
            loop = false;
            this->ptr = this->head;
        }
        return value;
    }
};

template<class Tp>
struct std::iterator_traits<MLLForwardIterator<Tp>> {
    typedef std::forward_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp* pointer;
    typedef Tp& reference;
};

/**
 * 循环单链表由于其迭代器的特殊性不做iterator_traits的实现
 * （太菜了不会）
 */

template<class T>
class MonoLinkedList {
private:
    typedef MonoLinkedListNode<T> node;
    typedef MonoLinkedList<T> self;
protected:
    node *head, *_end;
    size_t length;

    void remove_nodes() {
        node *iter = head, *temp = nullptr;
        while (iter != nullptr) {
            temp = iter->next;
            delete iter;
            iter = temp;
        }
    }
    void copy_from(const self& src) {
        length = src.length;
        if (length == 0) {
            head = nullptr;
            _end = nullptr;
        } else {
            node* from = src.head;
            node* here = nullptr;
            size_t index = 0;
            while (from != nullptr) {
                if (index == 0) {
                    head = new node{from->data_field, nullptr};
                    here = head;
                } else {
                    here->next = new node {from->data_field, nullptr};
                    here = here->next;
                }
                from = from->next;
                ++index;
            }
            _end = here;
        }
    }

public:
    typedef MLLForwardIterator<T> iterator;

    /// @brief 空构造
    MonoLinkedList():
    head(nullptr), _end(nullptr), length(0) {}

    /// @brief 以初始化列表初始化
    /// @param init_list 初始化列表
    MonoLinkedList(const std::initializer_list<T>& init_list) {
        length = init_list.size();
        size_t index = 0;
        node* here = nullptr;
        for (auto obj: init_list) {
            if (index == 0) {
                head = new node{obj, nullptr};
                here = head;
            } else {
                here->next = new node{obj, nullptr};
                here = here->next;
            }
            ++index;
        }
        _end = here;
    }
    MonoLinkedList(const T* array[], size_t size) {
        length = size;
        if (size == 0) {
            head = nullptr;
            _end = nullptr;
        } else {
            size_t index = 0;
            head = new node{array[index], nullptr};
            node* here = head;
            while (--size) {
                ++index;
                here->next = new node{array[index], nullptr};
                here = here->next;
            }
            _end = here;
        }
    }
    MonoLinkedList(const T& value, size_t size) {
        length = size;
        if (size == 0) {
            head = nullptr;
            _end = nullptr;
        } else {
            head = new node{value, nullptr};
            node* here = head;
            while (--size) {
                here->next = new node{value, nullptr};
                here = here->next;
            }
            _end = here;
        }
    }

    MonoLinkedList(const self& lst) {
        copy_from(lst);
    }
    self& operator= (const self& lst) {
        if (head != nullptr) remove_nodes();
        copy_from(lst);
    }
    MonoLinkedList(self&& lst) {
        head = lst.head;
        _end = lst._end;
        length = lst.length;
        lst.head = nullptr;
    }
    self& operator= (self&& lst) {
        if (head != nullptr) remove_nodes();
        head = lst.head;
        _end = lst._end;
        length = lst.length;
        lst.head = nullptr;
        return *this;
    }

    ~MonoLinkedList() {
        remove_nodes();
    }
    
    /// @brief 向链表末尾添加元素
    /// @param element 要添加的元素
    void push_back(const T& element) {
        if (head == nullptr) {
            head = new node{element, nullptr};
            _end = head;
            length = 1;
        } else {
            _end->next = new node{element, nullptr};
            _end = _end->next;
            ++length;
        }
    }

    /// @brief 向链表头部添加元素
    /// @param element 要添加的元素
    void push_front(const T& element) {
        if (head == nullptr) {
            head = new node{element, nullptr};
            _end = head;
            length = 1;
        } else {
            head = new node{element, head};
            ++length;
        }
    }

    /// @brief 删除链表头部元素
    bool pop_front() {
        if (head == nullptr) return false;
        node* temp = head;
        head = head->next;
        delete temp;
        --length;
        return true;
    }

    /// @brief 删除指定迭代器后的元素
    /// @param iter 指定的迭代器
    bool remove(iterator iter) {
        if (iter == _end) return false;
        node* raw_ptr = iter.raw();
        if (raw_ptr == nullptr) return false;
        node* fellow = raw_ptr->next->next;
        delete raw_ptr->next;
        raw_ptr->next = fellow;
        --length;
        return true;
    }

    /// @brief 在迭代器位置之后插入元素
    /// @param iter 给定的迭代器
    /// @param obj 元素
    /// @return 指向插入元素的迭代器
    iterator insert(iterator iter, const T& obj) {
        node* raw_ptr = iter.raw();
        node* new_node = new node{obj, raw_ptr->next};
        raw_ptr->next = new_node;
        ++length;
        return iterator(new_node);
    }

    /// @brief 反转单链表
    void reverse() {
        node *here = head, *temp1 = nullptr, *temp2 = nullptr;
        _end = head;
        while (here != nullptr) {
            temp2 = here->next;
            here->next = temp1;
            temp1 = here;
            here = temp2;
        }
        head = temp1;
    }
    
    iterator begin() const { return iterator(this->head); }
    iterator end() const { return iterator(nullptr); }
    T& front() const { return head->data_field; }
    T& back() const { return _end->data_field; }
    size_t size() const { return this->length; }
};

template<class T>
class LoopedMonoLinkedList: public MonoLinkedList<T> {
public:
    typedef LoopedMLLForwardIterator<T> iterator;
    LoopedMonoLinkedList(std::initializer_list<T> init_lst):
    MonoLinkedList<T>(init_lst) {}

    template<class ...Args>
    LoopedMonoLinkedList(Args ...args):
    MonoLinkedList<T>(std::forward<Args>(args)...) {}

    iterator begin() const {
        return iterator(this->head, this->head);
    }
    iterator end() const {
        return iterator(nullptr, this->head);
    }
};

/**
 * 以下为 DualLinkedList 的定义和实现
 * 以及其所需的工具实现
 */

template<class T>
struct DualLinkedListNode {
    DualLinkedListNode(
        const T& val,
        DualLinkedListNode<T>* __before, 
        DualLinkedListNode<T>* __after) {
        data_field = val;
        before = __before;
        after = __after;
    }
    T data_field;
    DualLinkedListNode<T> *before, *after;
};

template<class T>
class DLLBidirectionalIterator {
private:
    typedef DLLBidirectionalIterator<T> self;
    typedef DualLinkedListNode<T> node;
protected:
    node* ptr;
public:
    DLLBidirectionalIterator(node* ptr) { this->ptr = ptr;}
    node* raw() const { return this->ptr; }

    // LegacyIterator
    DLLBidirectionalIterator(const self& iter) = default;
    self& operator= (const self& iter) = default;
    ~DLLBidirectionalIterator() = default;
    T& operator* () const { return ptr->data_field; }
    self& operator++ () {
        if (ptr != nullptr) ptr = ptr->after;
        return *this;
    }

    // EqualityComparable
    bool operator== (const self& iter) const { return ptr == iter.ptr; }

    // LegacyInputIterator
    bool operator!= (const self& iter) const { return ptr != iter.ptr; }
    T* operator-> () { return &(ptr->data_field); }

    // LegacyForwardIterator
    DLLBidirectionalIterator() { ptr = nullptr; }
    self operator++ (int) {
        self temp(*this);
        if (ptr != nullptr) ptr = ptr->after;
        return temp;
    }

    // LegacyBidirectionalIterator
    self& operator-- () {
        if (ptr != nullptr) ptr = ptr->before;
        return *this;
    }
    self operator-- (int) {
        self temp(*this);
        if (ptr != nullptr) ptr = ptr->before;
        return temp;
    }
};

template<class T>
class DLLReverseBidirectionalIterator: public DLLBidirectionalIterator<T> {
private:
    typedef DLLReverseBidirectionalIterator<T> self;
    typedef DualLinkedListNode<T> node;
public:
    DLLReverseBidirectionalIterator(node* ptr):
    DLLBidirectionalIterator<T>(ptr) {}

    DLLReverseBidirectionalIterator(const self& iter) = default;
    self& operator= (const self& iter) = default;

    self& operator++ () {
        if (this->ptr != nullptr) this->ptr = this->ptr->before;
        return *this;
    }
    self operator++ (int) {
        self temp(*this);
        if (this->ptr != nullptr) this->ptr = this->ptr->before;
        return temp;
    }

    // LegacyBidirectionalIterator
    self& operator-- () {
        if (this->ptr != nullptr) this->ptr = this->ptr->after;
        return *this;
    }
    self operator-- (int) {
        self temp(*this);
        if (this->ptr != nullptr) this->ptr = this->ptr->after;
        return temp;
    }
};

template<class Tp>
struct std::iterator_traits<DLLBidirectionalIterator<Tp>> {
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef Tp value_type;
    typedef Tp* pointer;
    typedef Tp& reference;
};

template<class T>
class DualLinkedList {
private:
    typedef DualLinkedListNode<T> node;
    typedef DualLinkedList<T> self;
    node *head, *tail;
    size_t length;

    void rm_mem() {
        if (head == nullptr) return ;
        node* temp = nullptr;
        while (head != nullptr) {
            temp = head->after;
            delete head;
            head = temp;
        }
    }

    void copy_from(const self& lst) {
        length = lst.length;
        if (length == 0) {
            head = nullptr;
            tail = nullptr;
        } else {
            node* here = nullptr;
            node* from = lst.head;
            size_t index = 0;
            while (from != nullptr) {
                if (index == 0) {
                    head = new node(from->data_field, nullptr, nullptr);
                    here = head;
                } else {
                    here->after = new node(from->data_field, here, nullptr);
                    here = here->after;
                }
                from = from->after;
                ++index;
            }
            tail = here;
        }
    }

public:
    typedef DLLBidirectionalIterator<T> iterator;
    typedef DLLReverseBidirectionalIterator<T> reverse_iterator;
    
    /// @brief 空构造
    DualLinkedList():
    head(nullptr), tail(nullptr), length(0) {}

    /// @brief 以初始化列表初始化
    /// @param init_lst 初始化列表
    DualLinkedList(const std::initializer_list<T>& init_lst) {
        length = init_lst.size();
        if (length == 0) {
            head = nullptr;
            tail = nullptr;
        } else {
            node* here = nullptr;
            size_t index = 0;
            for (auto value: init_lst) {
                if (index == 0) {
                    head = new node(value, nullptr, nullptr);
                    here = head;
                } else {
                    here->after = new node(value, here, nullptr);
                    here = here->after;
                }
                ++index;
            }
            tail = here;
        }
    }

    /// @brief 以默认值初始化
    /// @param value 默认值
    /// @param size 列表长度
    DualLinkedList(const T& value, size_t size) {
        length = size;
        if (length == 0) {
            head = nullptr;
            tail = nullptr;
        } else {
            node* here = nullptr;
            for (size_t index = 0; index < size; ++index) {
                if (index == 0) {
                    head = new node(value, nullptr, nullptr);
                    here = head;
                } else {
                    here->after = new node(value, here, nullptr);
                    here = here->after;
                }
            }
            tail = here;
        }
    }

    /// @brief 以数组初始化
    /// @param array 数组指针
    /// @param size 数组大小
    DualLinkedList(const T array[], size_t size) {
        length = size;
        if (length == 0) {
            head = nullptr;
            tail = nullptr;
        } else {
            node* here = nullptr;
            for (size_t index = 0; index < size; ++index) {
                if (index == 0) {
                    head = new node(array[index], nullptr, nullptr);
                    here = head;
                } else {
                    here->after = new node(array[index], here, nullptr);
                    here = here->after;
                }
            }
            tail = here;
        }
    }

    DualLinkedList(const self& lst) { copy_from(lst); }
    DualLinkedList(self&& lst) {
        length = lst.length;
        head = lst.head;
        tail = lst.tail;
        lst.head = nullptr;
    }
    self& operator= (const self& lst) {
        rm_mem();
        copy_from(lst);
    }
    self& operator= (self&& lst) {
        rm_mem();
        length = lst.length;
        head = lst.head;
        tail = lst.tail;
        lst.head = nullptr;
        return *this;
    }
    ~DualLinkedList() { rm_mem(); }

    /// @brief 向链表尾部插入数据
    /// @param value 数据值
    void push_back(const T& value) {
        if (length == 0) {
            head = new node(value, nullptr, nullptr);
            tail = head;
        } else {
            tail->after = new node(value, tail, nullptr);
            tail = tail->after;
        }
        ++length;
    }

    /// @brief 向链表前端插入数据
    /// @param value 数据值
    void push_front(const T& value) {
        if (length == 0) {
            head = new node(value, nullptr, nullptr);
            tail = head;
        } else {
            head->before = new node(value, nullptr, head);
            head = head->before;
        }
        ++length;
    }

    /// @brief 在指定迭代器的前面插入数据
    /// @param iter 迭代器
    /// @param value 数据值
    void insert(iterator iter, const T& value) {
        node *raw_ptr = iter.raw(), *temp;
        if (raw_ptr == nullptr) push_back(value);
        else if (raw_ptr == head) push_front(value);
        else {
            temp = raw_ptr->before;
            raw_ptr->before = new node(value, raw_ptr->before, raw_ptr);
            temp->after = raw_ptr->before;
            ++length;
        }
    }

    /// @brief 删除迭代器所指的数据
    /// @param iter 迭代器
    void remove(iterator iter) {
        node* raw_ptr = iter.raw();
        if (raw_ptr == nullptr) return ;
        raw_ptr->before->after = raw_ptr->after;
        raw_ptr->after->before = raw_ptr->before;
        delete raw_ptr;
        --length;
    }

    /// @brief 翻转链表
    void reverse() {
        node *here = head, *temp = tail;
        tail = head;
        head = temp;
        while (here != nullptr) {
            temp = here->after;
            here->after = here->before;
            here->before = temp;
            here = temp;
        }
    }

    size_t size() const { return this->length; }
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(nullptr); }
    reverse_iterator rbegin() const { return reverse_iterator(tail); }
    reverse_iterator rend() const { return reverse_iterator(nullptr); }
    
};

#endif /* _LINKED_LIST_HPP_ */
