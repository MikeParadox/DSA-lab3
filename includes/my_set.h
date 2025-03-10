#ifndef MY_SET_H
#define MY_SET_H

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <print>

using std::println;

template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
class My_set
{
    struct Node;

public:
    class iterator;

    using value_type = T;
    using const_iterator = iterator;
    using reference = value_type&;
    using pointer = value_type*;

    class iterator
    {
    public:
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
                if (_current->parent->is_nill) return _current->parent;

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
                return _current;
            }
        }
        auto operator<=>(const iterator& rhs) const = default;

    private:
        Node* _current;
    };

    My_set();
    My_set(const My_set& rhs);
    My_set(const std::initializer_list<T>& lst);
    ~My_set();

    My_set& operator=(const My_set& rhs);

    bool empty() const { return !_size; }
    std::size_t size() const { return _size; }
    bool equal(const My_set& rhs) const;


    iterator begin() { return _begin; }
    iterator end() { return _end; }
    iterator lower_bound(const value_type& value);
    iterator upper_bound(const value_type& value);
    iterator find(value_type value) { return find(_root, value); }
    iterator find_min();
    iterator find_max();
    iterator find_prev(iterator it);
    iterator find_next(iterator it);

    iterator insert(value_type value);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear();
    void swap(My_set& rhs) noexcept;

    void print_infix() const;
    void print_reverse_infix() const;
    void print_layers() const;

    // print method to visualise a tree
    void print_post() { print_post(_root); }
    // method to check if all the nodes have links alright
    void check_all_links() const;


private:
    std::size_t _size{0};
    Node* _root;
    Node* _begin;
    Node* _end{static_cast<Node*>(operator new(sizeof(Node)))};

    const_iterator insert(value_type value, Node* pos);
    void print_infix(Node* n) const;
    void print_post(Node* n, std::size_t depth = 0);
    iterator find(Node* cur, value_type value);
    iterator erase_leaf(iterator pos);

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
using iterator = typename My_set<T, Compare>::iterator;

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::My_set()
{
    _end->is_nill = true;
    _end->parent = _root;
    _end->left = _end->right = _end;
    _root = _end;
    _root->left = _root->right = _root->parent = _end;
    _begin = _root;
    _begin->left = _begin->right = _end;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::My_set(const My_set& rhs) : My_set()
{
    for (const auto& x : rhs) insert(x);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::My_set(const std::initializer_list<T>& lst) : My_set()
{
    for (const auto& x : lst) insert(x);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::~My_set()
{
    clear(); // TODO
    delete _end;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>& My_set<T, Compare>::operator=(const My_set& rhs)
{
    auto temp{rhs};
    swap(temp);

    return *this;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::find(Node* cur,
                                                      value_type value)
{
    if (cur->data == value) return cur;
    if (!cur || cur == end()) return end();

    if (value < cur->data) return find(cur->left, value);
    else return find(cur->right, value);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::find_min()
{
    return begin();
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::find_max()
{
    return _end->parent;
}

// TODO implement
template<class T, class Compare>
    requires std::equality_comparable<T>
iterator<T, Compare> My_set<T, Compare>::lower_bound(const value_type& value)
{
    return _begin;
}


template<class T, class Compare>
    requires std::equality_comparable<T>
iterator<T, Compare> My_set<T, Compare>::upper_bound(const value_type& value)
{
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::find_prev(iterator it)
{
    return --it;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::find_next(iterator it)
{
    return ++it;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::clear()
{
    erase(begin(), end());
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::insert(value_type value)
{
    if (_root->is_nill)
    {
        _root = new Node{value, _end, _end, _end};
        _begin = _root;
        _end->parent = _root;
        ++_size;
        check_all_links(); // TODO delete after debugging
        return _root;
    }
    return insert(value, _root);
}


template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::insert(value_type value,
                                                        Node* pos)
{
    if (pos->data == value) return pos;

    if (pos->data > value)
    {
        if (pos->left->is_nill)
        {
            pos->left = new Node{value, pos, _end, _end};
            ++_size;
            if (value < _begin->data) _begin = pos->left;
            check_all_links(); // TODO delete after debugging
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
            check_all_links(); // TODO delete after debugging
            return pos->right;
        }
        return insert(value, pos->right);
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::erase(iterator pos)
{
    if (pos->is_nill) return _end;
    --_size;

    if (pos->left->is_nill && pos->right->is_nill)
    {
        auto temp = pos->parent;
        if (temp->is_nill) _begin = _root = _end;
        else
        {
            if (temp->left == pos) temp->left = _end;
            else temp->right = _end;
            if (pos == begin()) _begin = temp;
        }
        delete pos.current();
        return temp;
    }
    else if (!pos->left->is_nill && !pos->right->is_nill)
    {
        auto temp = pos->left;
        while (!temp->right->is_nill) temp = temp->right;
        if (temp->parent != pos) temp->parent->right = temp->left;

        if (!temp->left->is_nill) { temp->left->parent = temp->parent; }

        if (pos->parent->is_nill) _root = temp;
        else
        {
            if (pos->parent->left == pos) pos->parent->left = temp;
            else pos->parent->right = temp;
        }
        temp->parent = pos->parent;
        if (temp != pos->right)
        {
            temp->right = pos->right;
            pos->right->parent = temp;
        }
        if (temp != pos->left)
        {
            temp->left = pos->left;
            pos->left->parent = temp;
        }
        if (temp->left->right == temp) temp->left->right = _end;

        delete pos.current();
        return temp;
    }
    else
    {
        auto temp = pos->left->is_nill ? pos->right : pos->left;
        if (pos->parent->left == pos) pos->parent->left = temp;
        else pos->parent->right = temp;
        temp->parent = pos->parent;
        if (pos == begin()) _begin = temp;
        if (pos->parent->is_nill) _root = temp;
        delete pos.current();
        return temp;
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::erase(iterator first,
                                                       iterator last)
{
    while (first != last) first = erase(first);

    return first;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::swap(My_set& rhs) noexcept
{
    std::swap(_size, rhs._size);
    std::swap(_root, rhs._root);
    std::swap(_begin, rhs._begin);
    std::swap(_end, rhs._end);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::print_infix() const
{
    print_infix(_root);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::print_reverse_infix() const
{
} // using stack without recursion

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::print_layers() const
{
}

// TODO ask if it should just return bool or find out which one is
// greater in some way
template<class T, class Compare>
    requires std::equality_comparable<T>
bool My_set<T, Compare>::equal(const My_set& rhs) const
{
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::print_infix(Node* n) const
{
    if (!n || n == _end) return;
    print_infix(n->left);
    std::print("{} ", n->data);
    print_infix(n->right);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::print_post(Node* n, size_t depth)
{
    if (!n->is_nill)
    {
        print_post(n->right, depth + 1);
        std::string s(depth, ' ');
        std::println("{} {}", s, n->data);
        print_post(n->left, depth + 1);
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::check_all_links() const
{

    if (!_root->is_nill && !_root->parent->is_nill)
        println("root parent isn't nill");
    if (!_end->is_nill) println("end isn't nill");
    if (empty())
    {
        if (!_root->is_nill) println("root isn't nill in empty set");
        if (!_begin->is_nill) println("begin isn't nill in empty set");
    }
    else if (_size == 1)
    {
        if (_root->is_nill) println("root is nill in set with one elem");
        if (_begin->is_nill || _begin != _root)
            println("invalid begin in set with one elem");
    }
    else {}
}

template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
void swap(My_set<T, Compare>& a, My_set<T, Compare>& b)
{
    a.swap(b);
}










#endif
