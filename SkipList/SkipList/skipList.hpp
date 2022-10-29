#ifndef __LIST__
#define __LIST__

#include <functional>
#include <array>
#include <algorithm>

#define LVL_COUNT 10

template <typename Key,
    typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
class skip_list {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

private:
    key_compare _comparator{};
    allocator_type _allocator{};

    using value_p = std::unique_ptr<value_type, std::function<void(value_type*)>> ;
    struct node {
        value_p data;
        std::array<std::shared_ptr<node>, LVL_COUNT> childs;
        std::array<std::weak_ptr<node>, LVL_COUNT> parents;

        node() : data(nullptr) { /*std::cout << "Node ctr" << '\n';*/ }
        node(value_p&& _data) : data(std::move(_data)) { /*std::cout << "Node rref ctr" << '\n';*/ }
    };

    using node_p = std::shared_ptr<node>;
    node_p make_node(key_type&& key, mapped_type&& val) {
        auto mem = _allocator.allocate(1);
        //std::cout << "Node copy" << '\n';
        auto pairPtr = value_p(new(mem) value_type(key, val), [this](value_type* _ptr) {
            _ptr->~value_type();
            //std::cout << "Node dtr" << '\n';
            _allocator.deallocate(_ptr, 1);
            });
        return std::make_shared<node>(std::move(pairPtr));
    }

    template <bool ConstIter, bool IsForward>
    class skip_list_iterator {
        std::weak_ptr<node> dat;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::conditional_t<ConstIter, const value_type, value_type>;
        using pointer = std::conditional_t<ConstIter, const_pointer, pointer>;
        using reference = std::conditional_t<ConstIter, const_reference, reference>;
        using iterator_category = std::forward_iterator_tag;

        skip_list_iterator() = default;
        skip_list_iterator(std::shared_ptr<node>& _node) : dat(_node) {}
        skip_list_iterator(std::weak_ptr<node>& _node) : dat(_node) {}

        skip_list_iterator& operator++() {
            node_p _node = dat.lock();
            if (!_node)
                throw std::out_of_range("List is out of range");
            
            if(IsForward)
                dat = _node->childs[0];
            else
                dat = _node->parents[0];

            return *this;
        }

    
        skip_list_iterator operator++(int) {
            node_p _node = dat.lock();
            if (!_node)
                throw std::out_of_range("List is out of range");
            
            auto tmp = *this;            
            if (IsForward)
                dat = _node->childs[0];
            else
                dat = _node->parents[0];

            return tmp;
        }      

        reference operator*() {
            node_p _node = dat.lock();
            if (!_node)
                throw std::out_of_range("List is out of range");
            return *(_node->data);
        }

        pointer operator->() {
            node_p _node = dat.lock();
            if (!_node)
                throw std::out_of_range("List is out of range");
            return _node->data.get();
        }

        bool operator==(const skip_list_iterator& rhs) const {
            node_p l_node = dat.lock();
            node_p r_node = rhs.dat.lock();

            if (l_node && r_node)
                return l_node->data->first == r_node->data->first
                    && l_node->data->second == r_node->data->second;
            else if (!l_node && !r_node)
                return true;
            else
                return false;

        }
        bool operator!=(const skip_list_iterator& rhs) const { return !(*this == rhs); }
    };


    size_type size_val = 0;
    size_t level_val = 0;
    // —читаем головной и хвостовой элементы пустыми
    node_p head = make_node(key_type(), mapped_type());
    node_p tail = make_node(key_type(), mapped_type());

public:
    using iterator = skip_list_iterator<false, true>;
    using const_iterator = skip_list_iterator<true, true>;
    using reverse_iterator = skip_list_iterator<false, false>;
    using const_reverse_iterator = skip_list_iterator<true, false>;


    skip_list() = default;
    
    explicit skip_list(const key_compare& comp, const allocator_type& a = Alloc()) :
        _comparator(comp), _allocator(a) {}
    
    skip_list(const skip_list& another) : 
        _comparator(another._comparator), _allocator(another._allocator) 
    {
        for (auto it = another.begin(); it != another.end(); ++it)
            insert(*it);
    }

    skip_list(skip_list&& another) noexcept { *this = std::move(another); } // Need for correct swap method

    ~skip_list() { clear(); }

    skip_list& operator=(const skip_list& another) {

        if (*this != another) {
            clear();
            _comparator = another._comparator;
            _allocator = another._allocator;
            for (auto it = another.begin(); it != another.end(); ++it)
                insert(*it);
        }
        return *this;
    }

    skip_list& operator=(skip_list&& another) noexcept {    // Need for correct swap method
        std::swap(head, another.head);
        std::swap(size_val, another.size_val);
        std::swap(level_val, another.level_val);
        std::swap(_comparator, another._comparator);
        std::swap(_allocator, another._allocator);
        return *this;
    }


    iterator begin() { return iterator(head->childs[0]); }
    const_iterator begin() const {return const_iterator(head->childs[0]); }
    iterator end() { return iterator(); }
    const_iterator end() const{ return const_iterator(); }

    reverse_iterator rbegin() { return reverse_iterator(tail->parents[0]); }
    reverse_iterator rend() { return reverse_iterator(); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(tail->parents[0]); }
    const_reverse_iterator rend() const { return const_reverse_iterator(); }

    bool empty() const { return size_val == 0; }
    size_type size() const { return size_val; }

    mapped_type& operator[](const key_type& key) {
        iterator it = find(key);
        if (it == end())
            return insert(std::make_pair(key, mapped_type())).first->second;
        return it->second;
    };

    mapped_type& at(const key_type& key) {
        iterator it = find(key);
        if (it == end())
            throw std::out_of_range("List is out of range.");
        return it->second;
    };

    const mapped_type& at(const key_type& key) const {
        const_iterator it = find(key);
        if (it == end())
            throw std::out_of_range("List is out of range.");
        return it->second;
    };

    std::pair<iterator, bool> insert(const value_type& val);

    void erase(iterator position);
    size_type erase(const key_type& key); 
    void erase(iterator first, iterator last);

    void swap(skip_list& another) {
        auto copy = std::move(*this);
        *this = std::move(another);
        another = std::move(copy);
    }

    void clear() {
        size_val = 0;
        level_val = 0;
        
        auto cur = head->childs[0];
        for (size_t i = 0; i < LVL_COUNT; i++) {
            head->childs[i].reset();
            tail->parents[i].reset();
        }

        while (cur) {
            auto tmp = cur->childs[0];
            for (size_t i = 0; i < LVL_COUNT && cur->childs[i]; ++i) {
                cur->childs[i]->parents[i].reset();
                cur->childs[i].reset();
            }
            //delete cur;
            cur = tmp;
        }
    }

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

};

template <typename K, typename V, typename C, typename A>   //
inline bool operator==(const skip_list<K, V, C, A>& x, const skip_list<K, V, C, A>& y) {
    if (x.size() != y.size()) 
        return false;

    for (auto itX = x.begin(), itY = y.begin(); itX != x.end() && itY != y.end(); ++itX, ++itY)
        if (itX != itY)
            return false;
    return true;
}   

template <typename K, typename V, typename C, typename A>   //
inline bool operator!=(const skip_list<K, V, C, A>& x, const skip_list<K, V, C, A>& y) {
    return !(x == y);
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
typename skip_list<key_type, Value, Compare, Alloc>::iterator skip_list<key_type, Value, Compare, Alloc>::find(const key_type& key) {
    auto cur = head;
    for (int i = (int)level_val; i >= 0; --i)
        while (cur->childs[i] && _comparator(cur->childs[i]->data->first, key))
            cur = cur->childs[i];

    return ( cur->childs[0] && (cur->childs[0]->data->first == key) ) ? iterator(cur->childs[0]) : end();
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
typename skip_list<key_type, Value, Compare, Alloc>::const_iterator skip_list<key_type, Value, Compare, Alloc>::find(const key_type& key) const {
    auto cur = head;
    for (int i = (int)level_val; i >= 0; --i) {
        while (cur->childs[i] && _comparator(cur->childs[i]->data->first, key)) {
            cur = cur->childs[i];
        }
    }
    
    return (cur->childs[0] && (cur->childs[0]->data->first == key)) ? const_iterator(cur->childs[0]) : end();
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
std::pair<typename skip_list<key_type, Value, Compare, Alloc>::iterator, bool> 
skip_list<key_type, Value, Compare, Alloc>::insert(const value_type& val) {
    
    std::array<node_p, LVL_COUNT> update;
    update.fill(head);

    std::array<node_p, LVL_COUNT> rupdate;
    rupdate.fill(tail);

    auto cur = head;
    for (int i = (int)level_val;  i >= 0; --i) {
        while (cur->childs[i] && _comparator(cur->childs[i]->data->first, val.first))
            cur = cur->childs[i];

        rupdate[i] = cur->childs[i] ? cur->childs[i] : tail;
        update[i] = cur;
    }
    cur = cur->childs[0];

    if (cur && cur->data->first == val.first) {
        return std::make_pair(iterator(cur), false);
    }

    size_t _lvl = rand() % (LVL_COUNT);
    if (_lvl > level_val)
        level_val = _lvl;

    //if (_lvl > level_val) {
    //    if (level_val == MAX_LVL - 1)
    //        _lvl = MAX_LVL;
    //    else
    //        _lvl = ++level_val;
    //}

    node_p toInsert = make_node(std::move(key_type(val.first)), std::move(mapped_type(val.second)));

    for (size_t i = 0; i <= _lvl; ++i) {
        toInsert->parents[i] = rupdate[i]->parents[i];
        rupdate[i]->parents[i] = toInsert;
        
        toInsert->childs[i] = update[i]->childs[i];
        update[i]->childs[i] = toInsert;
    }
    
    ++size_val;
    return std::make_pair(iterator(toInsert), true);
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
typename skip_list<key_type, Value, Compare, Alloc>::size_type 
skip_list<key_type, Value, Compare, Alloc>::erase(const key_type& key) {
    std::array<node_p, LVL_COUNT> update;
    update.fill(head);

    std::array<node_p, LVL_COUNT> rupdate;
    rupdate.fill(tail);

    auto cur = head;
    for (int i = (int)level_val; i >= 0; --i) {
        while (cur->childs[i] && _comparator(cur->childs[i]->data->first, key))
            cur = cur->childs[i];

        rupdate[i] = cur->childs[i] ? cur->childs[i] : tail;
        update[i] = cur;
    }
    cur = cur->childs[0];

    if (cur && cur->data->first == key) {
        for (size_t i = 0; i <= level_val; i++) {
            if (update[i]->childs[i] != cur)
                break;
            update[i]->childs[i] = cur->childs[i];
            rupdate[i]->parents[i] = cur->parents[i];
        }
        while (level_val > 0 && !(head->childs[level_val])) {
            --level_val;
        }
        --size_val;
        return 1;
    }
    
    return 0;
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
void skip_list<key_type, Value, Compare, Alloc>::erase(skip_list<key_type, Value, Compare, Alloc>::iterator position) {
    erase(position->first);
}

template <typename key_type, typename Value, typename Compare, typename Alloc>
void skip_list<key_type, Value, Compare, Alloc>::erase(skip_list<key_type, Value, Compare, Alloc>::iterator first, 
                                                  skip_list<key_type, Value, Compare, Alloc>::iterator last) {
    for (auto it = first; it != last;) {
        auto tmp = it++;
        erase(tmp);
    }
}

#endif // !__LIST__