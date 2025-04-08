#ifndef BST_H
#define BST_H

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <print>
#include <queue>
#include <stack>

using std::println;

template<typename T, class Compare = std::less<T>,
         class Allocator = std::allocator<T>>

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
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using AllocType =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

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
        iterator operator--()
        {
            if (_current->is_nill)
            {
                _current = _current->parent;
                return _current;
            }

            if (!_current->left->is_nill)
            {
                _current = _current->left;
                while (!_current->right->is_nill) _current = _current->right;
                return _current;
            }
            else
            {
                auto temp = _current->parent;
                if (temp->right == _current)
                {
                    _current = temp;
                    return temp;
                }

                while (temp->data > _current->data)
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
            return _current;
        }
        auto operator<=>(const iterator& rhs) const = default;

    private:
        Node* _current;
    };

    BinarySearchTree(Compare comp = std::less<T>(),
                     AllocType alloc = std::allocator<T>());
    BinarySearchTree(const BinarySearchTree& rhs);
    BinarySearchTree(const std::initializer_list<T>& lst,
                     Compare comp = std::less<T>(),
                     AllocType alloc = std::allocator<T>());
    template<class Iterator>
    BinarySearchTree(Iterator first, Iterator last,
                     Compare comp = std::less<T>(),
                     AllocType alloc = std::allocator<T>());
    ~BinarySearchTree();

    BinarySearchTree& operator=(const BinarySearchTree& rhs);

    bool empty() const { return !_size; }
    std::size_t size() const { return _size; }
    bool operator==(const BinarySearchTree& rhs) const
    {
        return compare(_end->right, rhs._end->right);
    }
    bool operator!=(const BinarySearchTree& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator<(const BinarySearchTree& rhs) const;
    bool operator>(const BinarySearchTree& rhs) const;
    bool operator<=(const BinarySearchTree& rhs) const;
    bool operator>=(const BinarySearchTree& rhs) const;

    iterator begin() { return _end->left; }
    const_iterator begin() const { return _end->left; }
    iterator end() { return _end; }
    const_iterator end() const { return _end; }
    reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(end());
    }
    reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    iterator lower_bound(const value_type& value) const;
    iterator upper_bound(const value_type& value) const;
    std::pair<iterator, iterator> equal_range(const key_type& k) const
    {
        return std::pair<iterator, iterator>{lower_bound(k), upper_bound(k)};
    }
    iterator find(value_type value) const
    {
        return find(_end->right, std::move(value));
    }
    std::uint8_t count(const value_type& value) const
    {
        return find(_end->right, value) != end();
    }
    iterator find_min() { return begin(); }
    iterator find_max() { return --end(); }
    iterator find_prev(iterator it) { return --it; }
    iterator find_next(iterator it) { return ++it; }

    std::pair<iterator, bool> insert(value_type value);
    // iterator insert(value_type value);
    // std::pair<iterator, bool> insert(value_type&& value);
    template<class Iterator> void insert(Iterator first, Iterator last)
    {
        while (first != last) insert(*first++);
    }
    const_iterator insert(iterator pos, value_type value);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    size_type erase(value_type value);
    void clear();
    void swap(BinarySearchTree& rhs) noexcept;

    AllocType get_allocator() const { return _alloc; }
    key_compare key_comp() const noexcept { return _cmp; }
    value_compare value_comp() const noexcept { return _cmp; }

    void print_infix() const;
    void print_reverse_infix() const;
    void print_layers() const;
    // print method to visualise a tree
    void print_post() { print_post(_end->right); }

private:
    Compare _cmp{Compare()};
    AllocType _alloc;
    std::size_t _size{0};
    Node* _end{make_end()};

    void print_infix(Node* n) const;
    void print_post(Node* n, std::size_t depth = 0);
    iterator find(Node* cur, value_type value) const;
    bool compare(Node* a, Node* b) const;

    struct Node
    {
        value_type data;
        Node* parent;
        Node* left;
        Node* right;
        bool is_nill{false};
    };

    Node* make_end()
    {
        _end = _alloc.allocate(1);

        std::allocator_traits<AllocType>::construct(_alloc, &(_end->parent));
        _end->parent = _end;

        std::allocator_traits<AllocType>::construct(_alloc, &(_end->left));
        _end->left = _end;

        std::allocator_traits<AllocType>::construct(_alloc, &(_end->right));
        _end->right = _end;

        _end->is_nill = true;

        return _end;
    }

    Node* make_node(T&& elem, Node* parent, Node* left, Node* right)
    {
        Node* new_node{_alloc.allocate(1)};

        std::allocator_traits<AllocType>::construct(_alloc,
                                                    &(new_node->parent));
        new_node->parent = parent;

        std::allocator_traits<AllocType>::construct(_alloc, &(new_node->left));
        new_node->left = left;

        std::allocator_traits<AllocType>::construct(_alloc, &(new_node->right));
        new_node->right = right;

        std::allocator_traits<AllocType>::construct(_alloc, &(new_node->data),
                                                    std::forward<T>(elem));
        new_node->is_nill = false;

        return new_node;
    }

    void delete_end(Node* node)
    {
        std::allocator_traits<AllocType>::destroy(_alloc, &(node->parent));
        std::allocator_traits<AllocType>::destroy(_alloc, &(node->left));
        std::allocator_traits<AllocType>::destroy(_alloc, &(node->right));
        std::allocator_traits<AllocType>::deallocate(_alloc, node, 1);
    }

    inline void delete_node(Node* node)
    {
        std::allocator_traits<AllocType>::destroy(_alloc, &(node->data));
        delete_end(node);
    }

    void dispatch(Node* n);
};

template<typename T, class Compare, class Allocator>
using iterator = typename BinarySearchTree<T, Compare, Allocator>::iterator;

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(Compare comp,
                                                          AllocType alloc)
    : _cmp{comp}, _alloc{alloc}
{
    // _end->right->left = _end->right->right = _end->right->parent = _end;
    // _end->left = _end->right;
    // _end->left->left = _end->left->right = _end;
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(
    const BinarySearchTree& rhs)
    : BinarySearchTree()
{
    dispatch(rhs.end()->right);
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(
    const std::initializer_list<T>& lst, Compare comp, AllocType alloc)
    : BinarySearchTree(comp, alloc)
{
    for (const auto& x : lst) insert(x);
}

template<typename T, class Compare, class Allocator>

template<class Iterator>
BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(Iterator first,
                                                          Iterator last,
                                                          Compare comp,
                                                          AllocType alloc)
    : BinarySearchTree(comp, alloc)
{
    std::for_each(first, last, [this](auto x) { this->insert(x); });
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::~BinarySearchTree()
{
    clear();
    delete_end(_end);
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>&
BinarySearchTree<T, Compare, Allocator>::operator=(const BinarySearchTree& rhs)
{
    auto temp{rhs};
    swap(temp);

    return *this;
}

template<typename T, class Compare, class Allocator>

bool BinarySearchTree<T, Compare, Allocator>::operator<(
    const BinarySearchTree& rhs) const
{
    auto it_a = begin();
    auto it_b = rhs.begin();

    while (it_a != end() && it_b != rhs.end())
        if (*it_a < *it_b) return true;

    if (it_b != rhs.end() && it_a == end()) return true;

    return false;
}

template<typename T, class Compare, class Allocator>

bool BinarySearchTree<T, Compare, Allocator>::operator>(
    const BinarySearchTree& rhs) const
{
    return !(*this == rhs || *this < rhs);
}

template<typename T, class Compare, class Allocator>

bool BinarySearchTree<T, Compare, Allocator>::operator<=(
    const BinarySearchTree& rhs) const
{
    return !(*this > rhs);
}

template<typename T, class Compare, class Allocator>

bool BinarySearchTree<T, Compare, Allocator>::operator>=(
    const BinarySearchTree& rhs) const
{
    return !(*this < rhs);
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::iterator
BinarySearchTree<T, Compare, Allocator>::find(Node* cur, value_type value) const
{
    if (cur->is_nill) return _end;
    if (!_cmp(cur->data, value) && !_cmp(value, cur->data)) return cur;

    if (value < cur->data) return find(cur->left, std::move(value));
    else return find(cur->right, std::move(value));
}

template<typename T, class Compare, class Allocator>

iterator<T, Compare, Allocator>
BinarySearchTree<T, Compare, Allocator>::lower_bound(
    const value_type& value) const
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

template<typename T, class Compare, class Allocator>
iterator<T, Compare, Allocator>
BinarySearchTree<T, Compare, Allocator>::upper_bound(
    const value_type& value) const
{
    return ++find(_end->right, value);
}

template<typename T, class Compare, class Allocator>
void BinarySearchTree<T, Compare, Allocator>::clear()
{
    erase(begin(), end());
}

// template<typename T, class Compare, class Allocator>
// BinarySearchTree<T, Compare, Allocator>::iterator
// BinarySearchTree<T, Compare, Allocator>::insert(value_type value)
template<typename T, class Compare, class Allocator>
std::pair<typename BinarySearchTree<T, Compare, Allocator>::iterator, bool>
BinarySearchTree<T, Compare, Allocator>::insert(value_type value)
{
    auto old_size = _size;

    if (_end->right->is_nill)
    {
        _end->left = _end->right =
            make_node(std::move(value), _end, _end, _end);
        _end->parent = _end->right;
        ++_size;
        return std::pair<iterator, bool>(_end->right, old_size != _size);
    }
    return std::pair<iterator, bool>(insert(_end->right, std::move(value)),
                                     old_size != _size);
}

// template<typename T, class Compare, class Allocator>
// std::pair<typename BinarySearchTree<T, Compare, Allocator>::iterator, bool>
// BinarySearchTree<T, Compare, Allocator>::insert(value_type&& value)
// {
//     auto old_size = _size;
//     auto res = insert(value);

//     return std::pair<iterator, bool>{res, old_size != _size};
// }

template<typename T, class Compare, class Allocator>
BinarySearchTree<T, Compare, Allocator>::iterator
BinarySearchTree<T, Compare, Allocator>::insert(iterator pos, value_type value)
{
    if (pos->is_nill) return _end;
    if (!_cmp(pos->data, value) && !_cmp(value, pos->data)) return pos;

    if (_cmp(value, pos->data))
    {
        if (pos->left->is_nill)
        {
            pos->left = make_node(std::move(value), pos.current(), _end, _end);
            ++_size;
            if (pos->left->data < _end->left->data) _end->left = pos->left;
            return pos->left;
        }
        return insert(pos->left, std::move(value));
    }
    else
    {
        if (pos->right->is_nill)
        {
            pos->right = make_node(std::move(value), pos.current(), _end, _end);
            ++_size;
            if (_end->parent == pos) _end->parent = pos->right;
            return pos->right;
        }
        return insert(pos->right, std::move(value));
    }
}

template<typename T, class Compare, class Allocator>
BinarySearchTree<T, Compare, Allocator>::iterator
BinarySearchTree<T, Compare, Allocator>::erase(iterator pos)
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
        delete_node(pos.current());

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
        delete_node(pos.current());

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

        delete_node(pos.current());

        return res;
    }
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::iterator
BinarySearchTree<T, Compare, Allocator>::erase(iterator first, iterator last)
{
    while (first != last) first = erase(first);

    return last;
}

template<typename T, class Compare, class Allocator>

BinarySearchTree<T, Compare, Allocator>::size_type
BinarySearchTree<T, Compare, Allocator>::erase(value_type value)
{
    return erase(find(std::move(value))) != _end;
}

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::swap(
    BinarySearchTree& rhs) noexcept
{
    std::swap(_size, rhs._size);
    std::swap(_end, rhs._end);
}

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::print_infix() const
{
    print_infix(_end->right);
}

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::print_reverse_infix() const
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

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::print_layers() const
{
    if (_end->right == _end) return;

    std::queue<Node*> q;
    q.push(_end->right);

    while (!q.empty())
    {
        auto level_size = q.size();

        for (int i{}; i < level_size; ++i)
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

template<typename T, class Compare, class Allocator>

bool BinarySearchTree<T, Compare, Allocator>::compare(Node* a, Node* b) const
{
    if (a->is_nill != b->is_nill) return false;
    if (!a->is_nill)
        return (a->data == b->data) && compare(a->left, b->left) &&
               compare(a->right, b->right);

    return true;
}

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::print_infix(Node* n) const
{
    if (!n || n == _end) return;
    print_infix(n->left);
    std::print("{} ", n->data);
    print_infix(n->right);
}

template<typename T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::print_post(Node* n, size_t depth)
{
    if (!n->is_nill)
    {
        print_post(n->right, depth + 1);
        std::string s(depth, ' ');
        std::println("{} {}", s, n->data);
        print_post(n->left, depth + 1);
    }
}

template<class T, class Compare = std::less<T>,
         class Allocator = std::allocator<T>>

void swap(BinarySearchTree<T, Compare, Allocator>& a,
          BinarySearchTree<T, Compare, Allocator>& b)
{
    a.swap(b);
}

template<class T, class Compare, class Allocator>

void BinarySearchTree<T, Compare, Allocator>::dispatch(Node* n)
{
    if (n->is_nill) return;

    std::queue<Node*> q;
    q.push(n);

    while (!q.empty())
    {
        auto level_size = q.size();

        for (int i{}; i < level_size; ++i)
        {
            Node* curr{q.front()};
            insert(curr->data);
            q.pop();

            if (!curr->left->is_nill) { q.push(curr->left); }
            if (!curr->right->is_nill) { q.push(curr->right); }
        }
    }
}





#endif
