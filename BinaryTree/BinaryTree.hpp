
#ifndef _DSL_BINARY_TREE_HPP_
#define _DSL_BINARY_TREE_HPP_ 1

#include <concepts>
#include <vector>
#include <stack>
#include <queue>
#include <cstddef>
#include <stdint.h>
#include <functional>

namespace dsl {

namespace concepts {

template<class T>
concept BTreeNodeCompatible =
    std::semiregular<T> &&
    std::equality_comparable<T>
;

template<class T>
concept BTreeCompatible = 
    std::copyable<T> &&
    requires (T node, T other) {
        // typedef limitation
        typename T::value_type;
        // member limitations
        requires std::same_as<decltype(node.data), typename T::value_type>;
        requires std::same_as<decltype(node.left), T*>;
        requires std::same_as<decltype(node.right), T*>;
        {node.isLeaf()} -> std::same_as<bool>;
        // data -> equality_comparable 
        {node.data == other.data} -> std::same_as<bool>;
    } && requires (typename T::value_type v) {
        // constructors
        requires std::default_initializable<T>;
        {T(v)} -> std::same_as<T>;
        {T(v, nullptr)} -> std::same_as<T>;
        {T(v, nullptr, nullptr)} -> std::same_as<T>;
    }
;

} // namespace dsl::concepts

template<concepts::BTreeNodeCompatible Val>
struct BinaryTreeNode {
    // typedefs
    typedef Val value_type;

    value_type data;
    BinaryTreeNode<value_type> *left, *right;
    // constructors
    BinaryTreeNode():
    data(), left(nullptr), right(nullptr) { }
    explicit
    BinaryTreeNode(
        const value_type& value, 
        BinaryTreeNode* __left = nullptr,
        BinaryTreeNode* __right = nullptr
    ): data(value), left(__left), right(__right) { }
    BinaryTreeNode(const BinaryTreeNode&) = default;
    BinaryTreeNode& operator=(const BinaryTreeNode&) = default;
    BinaryTreeNode(BinaryTreeNode&&) = default;
    BinaryTreeNode& operator=(BinaryTreeNode&&) = default;

    inline bool isLeaf() const {
        return (left == nullptr) && (right == nullptr);
    }
};

enum class WalkOrder: uint8_t { PreOrder, InOrder, PostOrder, LayerOrder };

template<
    class Val,
    concepts::BTreeCompatible NodeTp = BinaryTreeNode<Val>
> class BinaryTree {

// typedefs
public:
    typedef NodeTp node_type;
    typedef Val value_type;
    typedef std::function<void(const value_type&)> callback_t;
    typedef BinaryTree<value_type, node_type> self;

private:
    node_type* root;

    void pre_order_walk(const callback_t& func) const {
        std::stack<node_type*> st;
        node_type* here = root;

        while (!st.empty() || here != nullptr) {
            while (here != nullptr) {
                func(here->data);
                st.push(here);
                here = here->left;
            }
            if (!st.empty()) {
                here = st.top();
                st.pop();
                here = here->right;
            }
        }
    }
    void in_order_walk(const callback_t& func) const {
        std::stack<node_type*> st;
        node_type* here = root;

        while (!st.empty() || here != nullptr) {
            while (here != nullptr) {
                st.push(here);
                here = here->left;
            }
            if (!st.empty()) {
                here = st.top();
                st.pop();
                func(here->data);
                here = here->right;
            }
        }
    }
    void post_order_walk(const callback_t& func) const {
        std::stack<node_type*> st;
        node_type* here = root;
        while (!st.empty() || here != nullptr) {
            while(here != nullptr) {
                st.push(here);
                here = here->left;
            }
            if (!st.empty()) {
                node_type* top = st.top();
                node_type* pre_top = nullptr;
                while (!st.empty() && top->right == pre_top) {
                    func(top->data);
                    pre_top = top;
                    st.pop();
                    top = st.empty() ? nullptr: st.top();
                }
                if (top != nullptr) here = top->right;
            }
        }
    }
    void layer_order_walk(const callback_t& func) const {
        std::queue<node_type*> q;
        q.push(root);
        while (!q.empty()) {
            auto ptr = q.front();
            func(ptr->data);
            if (ptr->left != nullptr) q.push(ptr->left);
            if (ptr->right != nullptr) q.push(ptr->right);
            q.pop();
        }
    }

    void rec_pre_walk(const callback_t& func, node_type* here) const {
        func(here->data);
        if (here->left != nullptr) rec_pre_walk(func, here->left);
        if (here->right != nullptr) rec_pre_walk(func, here->right);
    }
    void rec_in_walk(const callback_t& func, node_type* here) const {
        if (here->left != nullptr) rec_in_walk(func, here->left);
        func(here->data);
        if (here->right != nullptr) rec_in_walk(func, here->right);
    }
    void rec_post_walk(const callback_t& func, node_type* here) const {
        if (here->left != nullptr) rec_post_walk(func, here->left);
        if (here->right != nullptr) rec_post_walk(func, here->right);
        func(here->data);
    }

    void clear_up() {
        if (root == nullptr) return ;
        std::queue<node_type*> q;
        q.push(root);
        while (!q.empty()) {
            auto ptr = q.front();
            if (ptr->left != nullptr) q.push(ptr->left);
            if (ptr->right != nullptr) q.push(ptr->right);
            q.pop();
            delete ptr;
        }
        root = nullptr;
    }
    void copy_from(const self& tree) {
        if (tree.root == nullptr) return ;
        clear_up();
        std::queue<node_type*> q, tree_q;
        tree_q.push(tree.root);
        root = new node_type();
        q.push(root);
        
        while (!tree_q.empty()) {
            auto from = tree_q.front();
            auto here = q.front();
            here->data = from->data;
            if (from->left != nullptr) {
                here->left = new node_type();
                q.push(here->left);
                tree_q.push(from->left);
            }
            if (from->right != nullptr) {
                here->right = new node_type();
                q.push(here->right);
                tree_q.push(from->right);
            }
            tree_q.pop(); q.pop();
        }
    }
    void move_from(const self& tree) {
        if (tree.root == nullptr) return ;
        clear_up();
        root = tree.root;
        tree.root = nullptr;
    }
    
    static node_type* pi_build (
        const std::vector<value_type>& p,
        const std::vector<value_type>& i,
        size_t pl,
        size_t pr,
        size_t il,
        size_t ir
    ) {
        if ((pr <= pl) || (ir <= il)) return nullptr;
        size_t head;
        for (size_t index = il; index < ir; ++index) {
            if (i[index] == p[pl]) {
                head = index; break;
            }
        }
        node_type* node = new node_type(i[head]);
        size_t ls = head - il, rs = ir - head - 1; 
        node->left = pi_build(p, i, pl + 1, pl + ls + 1, il, head);
        node->right = pi_build(p, i, pl + ls + 1, pr, head + 1, ir);
        return node;
    }

public:
    // default constructor
    BinaryTree(): root(nullptr) { }
    // copy & move
    BinaryTree(const self& tree):
    root(nullptr) { copy_from(tree); }
    self& operator=(const self& tree) {
        copy_from(tree); return *this;
    }
    BinaryTree(self&& tree):
    root(nullptr) { move_from(tree); }
    self& operator=(self&& tree) {
        move_from(tree); return *this;
    }
    // deconstructor
    ~BinaryTree() { clear_up(); }

    // constructors
    BinaryTree(
        const std::vector<value_type>& pre_order,
        const std::vector<value_type>& in_order
    ): root(nullptr) {
        if (pre_order.size() != in_order.size()) return ;
        if (pre_order.size() <= 0) return ;
        size_t head = 0;
        for (size_t i = 0; i < in_order.size(); ++i) {
            if (in_order[i] == pre_order.front()) {
                head = i; break;
            }
        }
        root = new node_type(pre_order.front());
        root->left = pi_build(
            pre_order, in_order,
            1, head + 1,
            0, head
        );
        root->right = pi_build(
            pre_order, in_order,
            head + 1, pre_order.size(),
            head + 1, in_order.size()
        );
    }

    // walk
    void walk(const callback_t& func, WalkOrder order) const {
        if (root == nullptr) return ;
        switch (order) {
            case WalkOrder::PreOrder :
                pre_order_walk(func); break;
            case WalkOrder::InOrder :
                in_order_walk(func); break;
            case WalkOrder::PostOrder :
                post_order_walk(func); break;
            case WalkOrder::LayerOrder :
                layer_order_walk(func); break;
            default: break;
        }
    }
    void recursiveWalk(const callback_t& func, WalkOrder order) const {
        if (root == nullptr) return ;
        switch (order) {
            case WalkOrder::PreOrder :
            rec_pre_walk(func, root); break;
            case WalkOrder::InOrder :
            rec_in_walk(func, root); break;
            case WalkOrder::PostOrder :
            rec_post_walk(func, root); break;
            default: break;
        }
    }

    // insert
    void insert(const value_type& val) {
        if (root == nullptr) {
            root = new node_type(val);
            return ;
        }
        std::queue<node_type*> q;
        q.push(root);
        while(!q.empty()) {
            auto ptr = q.front();
            if (ptr->left == nullptr) {
                ptr->left = new node_type(val);
                return ;
            }
            q.push(ptr->left);
            if (ptr->right == nullptr) {
                ptr->right = new node_type(val);
                return ;
            }
            q.push(ptr->right);
            q.pop();
        }
    }

    // state
    bool empty() const { return root == nullptr; }
    size_t countNode() const {
        if (root == nullptr) return 0;
        size_t n = 0;
        layer_order_walk([&n](const Val& v) -> void { ++n; });
        return n;
    }
    size_t countLeaf() const {
        if (root == nullptr) return 0;
        size_t n = 0;
        std::queue<node_type*> q;
        q.push(root);
        while (!q.empty()) {
            auto ptr = q.front();
            if (ptr->isLeaf()) ++n;
            if (ptr->left != nullptr) q.push(ptr->left);
            if (ptr->right != nullptr) q.push(ptr->right);
            q.pop();
        }
        return n;
    }
    size_t getHeight() const {
        if (root == nullptr) return 0;
        std::function<size_t(node_type*)> get_height = 
        [&get_height](node_type* node) -> size_t {
            if (node->isLeaf()) return 1;
            size_t ld = 0, rd = 0;
            if (node->left != nullptr) ld = get_height(node->left);
            if (node->right != nullptr) rd = get_height(node->right);
            return std::max(ld, rd) + 1;
        };
        return get_height(root);
    }
    size_t getWidth() const {
        if (root == nullptr) return 0;
        size_t wid = 0;
        std::queue<node_type*> q0, q1;
        q0.push(root);
        std::queue<node_type*>* q = &q0, *to = &q1;
        while (!q->empty()) {
            wid = std::max(q->size(), wid);
            while (!q->empty()) {
                auto ptr = q->front();
                if (ptr->left != nullptr) to->push(ptr->left);
                if (ptr->right != nullptr) to->push(ptr->right);
                q->pop();
            }
            std::swap(q, to);
        }
        return wid;
    }

    // mirror
    bool isMirror(const self& tree) const {
        if (tree.root == nullptr && root == nullptr) return true;
        if (tree.root == nullptr || root == nullptr) return false;
        std::queue<node_type*> q;
        q.push(root); q.push(tree.root);
        while (!q.empty()) {
            auto ptr1 = q.front(); q.pop();
            if (q.empty()) return false;
            auto ptr2 = q.front(); q.pop();
            if (ptr1 == nullptr && ptr2 == nullptr) continue;
            if (ptr1 == nullptr || ptr2 == nullptr) return false;
            if (ptr1->data != ptr2->data) return false;
            q.push(ptr1->left);  q.push(ptr2->right);
            q.push(ptr1->right); q.push(ptr2->left);
        }
        return true;
    }
};

namespace functions {

    template<class Val, class NodeTp>
    void walkBiTree(
        const BinaryTree<Val, NodeTp>& tree,
        const typename BinaryTree<Val, NodeTp>::callback_t& callback,
        WalkOrder order
    ) {
        tree.walk(callback, order);
    }

    template<class Val, class NodeTp>
    void recursiveWalkBiTree(
        const BinaryTree<Val, NodeTp>& tree,
        const typename BinaryTree<Val, NodeTp>::callback_t& callback,
        WalkOrder order
    ) {
        
    }

} // namespace dsl::functions

} // namespace dsl

#endif /*_DSL_BINARY_TREE_HPP_*/
