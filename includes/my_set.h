#ifndef MY_SET_H
#define MY_SET_H

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <print>

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
            if (_current->right)
            {
                _current = _current->right;
                while (_current->left) _current = _current->left;
                return _current;
            }
            else
            {
                auto temp = _current->parent;
                while (temp->data < _current->data)
                {
                    if (temp->left && temp->left->data > _current->data)
                    {
                        _current = temp->left;
                        return _current;
                    }
                    else if (temp->right && temp->right->data > _current->data)
                    {
                        _current = temp->right;
                        return _current;
                    }
                    else temp = temp->parent;
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

    bool is_empty() const { return !_size; }
    std::size_t size() const { return _size; }
    bool is_equal(const My_set& rhs) const;


    iterator begin() { return _begin; }
    iterator end() { return _end; }
    iterator lower_bound(const value_type& value);
    iterator upper_bound(const value_type& value);
    iterator find_min();
    iterator find_max();
    iterator find(Node* cur, value_type value);
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

private:
    std::size_t _size{0};
    Node* _root;
    Node* _begin;
    Node* _end{static_cast<Node*>(operator new(sizeof(Node)))};

    void clear(Node* b);
    const_iterator insert(value_type value, Node* pos);
    void print_infix(Node* n) const;
    void print_post(Node* n, std::size_t depth = 0);


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
    _root->left = _root->right = _end;
    _begin = _root;
    _begin->left = _begin->right = _end;
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::My_set(const My_set& rhs) : My_set()
{
    _size = rhs._size;
    for (const auto& x : rhs) this->insert(x);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::My_set(const std::initializer_list<T>& lst) : My_set()
{
    _size = lst.size();
    for (const auto& x : lst) this->insert(x);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::~My_set()
{
    clear();
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
    if (*cur == value) return cur;
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
    clear(_root);
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::insert(value_type value)
{
    if (_root->is_nill)
    {
        _root = new Node{value, _end, _end, _end};
        return _root;
    }
    return insert(value, _root);
}

// TODO implement end() checks and check if there is a parent
template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::erase(iterator it)
{
    if (it->parent->is_nill)
    {
        delete it.current();
        it = _end;
        return _end;
    }
    else if (it->left->is_nill && it->right->is_nill)
    {
        auto temp = it->parent;
        if (temp->left == it) temp->left = _end;
        else temp->right = _end;
        delete it.current();
        if (temp) return temp; // TODO debug from here
        else return end();
    }
    if (it->left && (!it->right || it->right == _end))
    {
        if (it->parent->left == it.current()) it->parent->left = it->left;
        else it->parent->right = it->left;
    }
    else if (it->right && !it->left)
    {
        if (it->parent->left == it.current()) it->parent->left = it->right;
        else it->parent->right = it->right;
    }
    else if (it->left && it->right)
    {
        if (it->parent->left == it.current())
        {
            it->parent->left = it->left;
            it->left->parent = it->parent;
            it->left->right = it->right;
        }
        if (it->parent->right == it.current())
        {
            it->parent->right = it->left;
            it->left->parent = it->parent;
            it->left->right = it->right;
        }
    }
}

template<class T, class Compare>
    requires std::equality_comparable<T>
My_set<T, Compare>::iterator My_set<T, Compare>::erase(iterator first,
                                                       iterator last)
{
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
bool My_set<T, Compare>::is_equal(const My_set& rhs) const
{
}

template<class T, class Compare>
    requires std::equality_comparable<T>
void My_set<T, Compare>::clear(Node* b)
{
    _size = 0;
    if (!b || b == _end) return;

    if (!b->left && (!b->right || b->right == _end)) erase(b);
    else
    {
        clear(b->left);
        clear(b->right);
    }
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
            return pos->right;
        }
        else if (pos->right->is_nill)
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

    return;
}

template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
void swap(My_set<T, Compare>& a, My_set<T, Compare>& b)
{
    a.swap(b);
}


#endif
