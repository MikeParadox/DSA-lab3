#ifndef BST_H
#define BST_H

#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <print>
#include <queue>
#include <random>
#include <stack>
#include <vector>

using std::println;

template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
class BinarySearchTree
{
    struct Node;

public:
    class iterator;

    using key_type = T;
    using key_compare = Compare;
    using value_type = T;
    using value_compare = Compare;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using const_iterator = iterator;

    class iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = BinarySearchTree::value_type;
        using difference_type = BinarySearchTree::difference_type;
        using pointer = BinarySearchTree::const_pointer;
        using reference = BinarySearchTree::const_reference;


        iterator(Node* n) : _current{n} {}

        value_type& operator*() { return _current->data; }
        Node* operator->() { return _current; }
        Node* current() { return _current; }
        iterator operator++()
        {
            if (_current->is_nill) return _current;

            if (!_current->right->is_nill)
            {
                _current = _current->right;
                while (!_current->left->is_nill) _current = _current->left;
                return _current;
            }
            else
            {
                if (_current->parent->is_nill) _current = _current->parent;

                else
                {
                    auto temp = _current->parent;
                    if (temp->left == _current)
                    {
                        _current = temp;
                        return temp;
                    }

                    while (temp->data < _current->data)
                    {
                        temp = temp->parent;
                        if (temp->is_nill)
                        {
                            _current = temp;
                            return temp;
                        }
                    }
                    _current = temp;
                }
            }
            return _current;
        }
        auto operator<=>(const iterator& rhs) const = default;

    private:
        Node* _current;
    };

    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree& rhs);
    BinarySearchTree(const std::initializer_list<T>& lst);
    ~BinarySearchTree();

    BinarySearchTree& operator=(const BinarySearchTree& rhs);

    bool empty() const { return !_size; }
    std::size_t size() const { return _size; }
    bool equal(const BinarySearchTree& rhs) const;


    iterator begin() { return _end->left; }
    const_iterator begin() const { return _end->left; }
    iterator end() { return _end; }
    const_iterator end() const { return _end; }
    iterator lower_bound(const value_type& value);
    iterator upper_bound(const value_type& value);
    iterator find(value_type value) { return find(_end->right, value); }
    iterator find_min() { return begin(); }
    iterator find_max() { return --end(); }
    iterator find_prev(iterator it) { return --it; }
    iterator find_next(iterator it) { return ++it; }

    iterator insert(value_type value);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear();
    void swap(BinarySearchTree& rhs) noexcept;

    void print_infix() const;
    void print_reverse_infix() const;
    void print_layers() const;

    // print method to visualise a tree
    void print_post() { print_post(_end->right); }

private:
    std::size_t _size{0};
    Node* _end{static_cast<Node*>(operator new(sizeof(Node)))};

    const_iterator insert(value_type value, Node* pos);
    void print_infix(Node* n) const;
    void print_post(Node* n, std::size_t depth = 0);
    iterator find(Node* cur, value_type value);
    bool compare(Node* a, Node* b) const;
    bool is_leaf(Node* n) const
    {
        return !n->is_nill && n->left->is_nill && n->right->is_nill;
    }

    struct Node
    {
        value_type data;
        Node* parent;
        Node* left;
        Node* right;
        bool is_nill{false};
    };
};

template<class T, class Compare>
using iterator = typename BinarySearchTree<T, Compare>::iterator;

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::BinarySearchTree()
{
    _end->is_nill = true;
    _end->parent = _end->right;
    _end->left = _end->right = _end;
    _end->right = _end;
    _end->right->left = _end->right->right = _end->right->parent = _end;
    _end->left = _end->right;
    _end->left->left = _end->left->right = _end;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::BinarySearchTree(const BinarySearchTree& rhs)
    : BinarySearchTree()
{
    for (const auto& x : rhs) { insert(x); }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::BinarySearchTree(
    const std::initializer_list<T>& lst)
    : BinarySearchTree()
{
    for (const auto& x : lst) insert(x);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::~BinarySearchTree()
{
    clear();
    delete _end;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>&
BinarySearchTree<T, Compare>::operator=(const BinarySearchTree& rhs)
{
    auto temp{rhs};
    swap(temp);

    return *this;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::iterator
BinarySearchTree<T, Compare>::find(Node* cur, value_type value)
{
    if (cur->data == value) return cur;
    if (!cur || cur == end()) return end();

    if (value < cur->data) return find(cur->left, value);
    else return find(cur->right, value);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
iterator<T, Compare>
BinarySearchTree<T, Compare>::lower_bound(const value_type& value)
{
    if (empty()) return _end;

    auto temp = _end->right;

    while (!temp->is_nill)
    {
        if (temp->data == value) break;
        if (temp->data < value) temp = temp->right;
        else
        {
            if (!temp->left->is_nill && temp->left->data <= value)
                temp = temp->left;
            break;
        }
    }

    return temp;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
iterator<T, Compare>
BinarySearchTree<T, Compare>::upper_bound(const value_type& value)
{
    if (empty()) return _end;

    auto temp = _end->right;

    while (!temp->is_nill)
    {
        if (temp->data == value) break;
        if (temp->data > value) temp = temp->left;
        else
        {
            if (!temp->right->is_nill && temp->right->data >= value)
                temp = temp->right;
            break;
        }
    }

    return temp;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::clear()
{
    erase(begin(), end());
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::iterator
BinarySearchTree<T, Compare>::insert(value_type value)
{
    if (_end->right->is_nill)
    {
        _end->right = new Node{value, _end, _end, _end};
        _end->left = _end->right;
        _end->parent = _end->right;
        ++_size;
        return _end->right;
    }
    return insert(value, _end->right);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::iterator
BinarySearchTree<T, Compare>::insert(value_type value, Node* pos)
{
    if (pos->data == value) return pos;

    if (pos->data > value)
    {
        if (pos->left->is_nill)
        {
            pos->left = new Node{value, pos, _end, _end};
            ++_size;
            if (value < _end->left->data) _end->left = pos->left;
            return pos->left;
        }
        return insert(value, pos->left);
    }
    else
    {
        if (pos->right->is_nill)
        {
            pos->right = new Node{value, pos, _end, _end};
            ++_size;
            _end->parent = pos->right;
            return pos->right;
        }
        return insert(value, pos->right);
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::iterator
BinarySearchTree<T, Compare>::erase(iterator pos)
{
    if (pos->is_nill) return pos;
    --_size;

    if (pos->left->is_nill && pos->right->is_nill)
    {
        if (!pos->parent->is_nill)
            pos->parent->left == pos ? pos->parent->left = _end
                                     : pos->parent->right = _end;
        else _end->right = _end;

        if (pos == begin()) _end->left = pos->parent;
        auto temp = pos->parent;
        delete pos.current();

        return temp;
    }
    else if (pos->left->is_nill || pos->right->is_nill)
    {
        auto child = pos->left->is_nill ? pos->right : pos->left;
        child->parent = pos->parent;
        if (!pos->parent->is_nill)
            pos->parent->left == pos ? pos->parent->left = child
                                     : pos->parent->right = child;
        else _end->right = child;

        if (pos == begin()) _end->left = child;
        auto temp = pos;
        ++temp;
        delete pos.current();

        return temp;
    }
    else
    {
        auto res = pos;
        ++res;
        auto temp = pos->left;
        while (!temp->right->is_nill) temp = temp->right;
        if (pos->parent->is_nill) _end->right = temp;
        else
            pos->parent->left == pos ? pos->parent->left = temp
                                     : pos->parent->right = temp;

        if (pos->left == temp)
        {
            temp->parent = pos->parent;
            temp->right = pos->right;
        }
        else
        {
            if (!temp->left->is_nill)
            {
                temp->left->parent = temp->parent;
                temp->parent->right = temp->left;
            }
            else temp->parent->right = _end;

            temp->right = pos->right;
            temp->left = pos->left;
            temp->left->parent = temp;
            temp->parent = pos->parent;
        }
        temp->right->parent = temp;

        delete pos.current();

        return res;
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
BinarySearchTree<T, Compare>::iterator
BinarySearchTree<T, Compare>::erase(iterator first, iterator last)
{
    while (first != last) first = erase(first);

    return last;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::swap(BinarySearchTree& rhs) noexcept
{
    std::swap(_size, rhs._size);
    std::swap(_end->right, rhs._end->right);
    std::swap(_end->left, rhs._end->left);
    std::swap(_end, rhs._end);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::print_infix() const
{
    print_infix(_end->right);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::print_reverse_infix() const
{
    std::stack<Node*> st;

    Node* curr{_end->right};

    while (curr != _end || st.empty() == false)
    {
        while (!curr->is_nill)
        {
            st.push(curr);
            curr = curr->right;
        }

        curr = st.top();
        st.pop();

        std::print("{} ", curr->data);

        curr = curr->left;
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::print_layers() const
{
    if (_end->right == _end) return;

    std::queue<Node*> q;
    q.push(_end->right);

    while (!q.empty())
    {
        auto level_size = q.size();

        for (int i = 0; i < level_size; ++i)
        {
            Node* curr{q.front()};
            q.pop();

            std::print("{} ", curr->data);
            if (curr->left != _end) { q.push(curr->left); }
            if (curr->right != _end) { q.push(curr->right); }
            if (level_size - 1 == i) std::println();
        }
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
bool BinarySearchTree<T, Compare>::equal(const BinarySearchTree& rhs) const
{
    return compare(_end->right, rhs._end->right);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
bool BinarySearchTree<T, Compare>::compare(Node* a, Node* b) const
{
    if (a->is_nill != b->is_nill) return false;
    if (!a->is_nill)
        return (a->data == b->data) && compare(a->left, b->left) &&
               compare(a->right, b->right);

    return true;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::print_infix(Node* n) const
{
    if (!n || n == _end) return;
    print_infix(n->left);
    std::print("{} ", n->data);
    print_infix(n->right);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void BinarySearchTree<T, Compare>::print_post(Node* n, size_t depth)
{
    if (!n->is_nill)
    {
        print_post(n->right, depth + 1);
        std::string s(depth, ' ');
        std::println("{} {}", s, n->data);
        print_post(n->left, depth + 1);
    }
}

template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
void swap(BinarySearchTree<T, Compare>& a, BinarySearchTree<T, Compare>& b)
{
    a.swap(b);
}

inline void sieve_array(int n)
{
    std::vector<int> v(n);

    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();

    for (int i{2}; i <= std::sqrt(n); ++i)
    {
        if (!v[i])
        {
            for (int j{2}; i * j < n; j++) v[j * i] = 1;
        }
    }

    for (int i{2}, count{0}; i < n && count != n; ++i)
    {
        if (!v[i])
        {
            ++count;
            // std::print("{} ", i);
        }
    }

    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    std::println();

    std::println(
        "Time of sieve on array with {} elems {}", n,
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
            .count());
}

inline void sieve_set(int n)
{

    // std::chrono::steady_clock::time_point begin =
    //     std::chrono::steady_clock::now();

    BinarySearchTree<int> set;
    // std::set<int> set;

    std::vector<int> v(n - 2);

    for (int i{2}; i < n; ++i) v[i - 2] = i;

    auto rng = std::default_random_engine{};
    std::ranges::shuffle(v, rng);
    for (const auto& x : v) set.insert(x);

    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();

    for (auto it{set.begin()}; it != set.end(); ++it)
    {
        auto prime = *it;
        auto del = it;
        ++del;

        while (del != set.end()) // node
        {
            if ((*del) % prime == 0) del = set.erase(del);
            else ++del;
        }
    }

    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    std::println(
        "Time of sieve on set with {} elems {}", n,
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
            .count());

    // set.print_infix();
}




#endif
