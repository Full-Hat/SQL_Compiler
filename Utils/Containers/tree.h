#pragma once

#include <any>
#include <sys/types.h>
#include <type_traits>
#include <utility>
#include <vector>
#include <optional>

#include "vector.h"

template <typename Container>
concept ContainerWithPushBack = requires(Container c) {
    { c.begin() } -> std::same_as<typename Container::iterator>;
    { c.end() } -> std::same_as<typename Container::iterator>;
    { c.push_back(std::declval<typename Container::value_type>()) };
};

template <typename T, typename U>
concept ScalarAssignable = requires(T t, U u) {
    { t = u } -> std::same_as<T&>;
};

template <typename T>
concept TreeScalar = requires {
    requires ScalarAssignable<T&, T>;
    requires std::is_same_v<typename T::node_type, typename T::template node_scalar<T>>;
};

template<typename U>
class node
{
public:
    using data_type = U;
protected:
    data_type m_data_set;

public:
    constexpr node() = default;
    constexpr node(data_type &&_value) : m_data_set(_value) {}

    constexpr data_type& data() { return m_data_set; }
    constexpr data_type data() const { return m_data_set; }

    // Non vector implementation

};

template<typename U>
class node_scalar : public node<U>
{
public:
    using node<U>::node;

    constexpr void set_value(U &&_value)
    {
        this->m_data_set = std::forward<U>(_value);
    }
};

template<typename U>
class node_container : public node<U>
{
public:
    using node<U>::node;
    // Vector implementation
    using any_vector = std::vector<std::any>;

    constexpr typename any_vector::iterator begin() { return this->m_data_set.begin(); }

    constexpr typename any_vector::iterator end(){ return this->m_data_set.end(); }

    constexpr void has_values() const
    {
        return this->m_data_set.empty();
    }

    constexpr void push_back(std::vector<std::any> &&_value)
    {
        this->m_data_set.push_back(std::forward<U>(_value));
    }
};

template <typename Key, typename T, typename C, typename ChildType, typename = void>
class tree
{
public:
    using data_type = T;
    using children_set_type = vector<Key, ChildType>;

    template<typename U = T>
    class node;

    template<typename U = T>
    class node_container;

    template<typename U = T>
    class node_scalar;

    using node_type = C;

protected:
    vector<Key, ChildType> m_children;
    node_type m_data;

public:
    constexpr tree() = default;
    constexpr tree(data_type &&_value) : m_data((data_type&&)_value)
    {
    }
    //constexpr virtual ~tree() {}
    constexpr node_type get_values_struct() { return m_data; }
    constexpr children_set_type get_all_childs() { return m_children; }
    constexpr std::optional<ChildType> find_child(const Key &_key)
    {
        const auto it = std::find(m_children.begin(), m_children.end(), [&](auto _pair) { return _pair.first == _key; });
        if (it == m_children.end())
        {
            return std::nullopt;
        }

        return {it->second};
    }
    constexpr void add_child(const Key &_key, ChildType &&_child)
    {
        m_children.push_back(_key, std::forward<ChildType>(_child));
    }
};

template <typename Key, typename T, typename C = node_scalar<T>>
class tree_scalar : public tree<Key, T, C, tree_scalar<Key, T>, std::enable_if<TreeScalar<T>>>
{
public:
    using tree<Key, T, C, tree_scalar<Key, T>, std::enable_if<TreeScalar<T>>>::tree;
    
    //constexpr virtual ~tree_scalar() {}
    
    constexpr void add_child(const Key &_key, tree_scalar<Key, T, C> &&_child)
    {
        this->m_children.push_back(_key, std::forward<tree_scalar<Key, T, C>>(_child));
    }
};
template <typename Key, typename T, typename C = node_container<T>>
class tree_container : public tree<Key, T, C, tree_container<Key, T>, std::enable_if<ContainerWithPushBack<T>>>
{
public:
    using tree<Key, T, C, std::enable_if<ContainerWithPushBack<T>>>::tree;

    using tree<Key, T, C, std::enable_if<ContainerWithPushBack<T>>>::add_child;

    //constexpr virtual ~tree_container() {}
};


// template <typename Key, typename T>
// class tree {
// public:
//     using size_type = typename std::vector<std::pair<Key, T>>::size_type;
//     using tree_data_type = vector<Key, T>;

//     constexpr tree() : data_{}, children_{} {}

//     template <typename U = T>
//     constexpr tree(const U& data) : data_{data}, children_{} {}

//     template <typename U = T>
//     constexpr void add_child(U&& data) {
//         children_.push_back(std::forward<U>(data));
//     }

//     template <typename U = T>
//     constexpr void add_child(const tree<Key, U>& t) {
//         children_.push_back(t);
//     }

//     template <typename U = T>
//     constexpr void add_child(const Key &_key, tree<Key, U>&& t) {
//         children_.push_back(_key, std::move(t));
//     }

//     constexpr void remove_child(std::size_t i) {
//         children_.erase(children_.begin() + i);
//     }

//     template <typename U = T>
//     constexpr void add_value(const Key &_key, const U &_data)
//     {
//         data_.push_back(_key, _data);
//     }

//     template <typename U = T>
//     constexpr void add_value(const Key &_key, U &&_data)
//     {
//         data_.push_back(_key, (U&&)_data);
//     }

//     constexpr void clear() {
//         data_ = {};
//         children_.clear();
//     }

//     // Data getters
//     constexpr std::conditional_t<std::is_const_v<T>, const T&, T&> value(const Key &_key) noexcept {
//         return data_[_key];
//     }

//     constexpr std::conditional_t<std::is_const_v<T>, const T&, T&> value(const Key &_key) const noexcept {
//         return data_[_key];
//     }

//     constexpr const tree& left() const noexcept {
//         return children_[0];
//     }

//     constexpr tree& left() noexcept {
//         return children_[0];
//     }

//     constexpr const tree& right() const noexcept {
//         return children_[1];
//     }

//     constexpr tree& right() noexcept {
//         return children_[1];
//     }

//     constexpr bool empty() const noexcept {
//         return children_.empty();
//     }

//     constexpr size_t size() const noexcept {
//         return children_.size();
//     }

//     constexpr const tree& operator[](Key index) const noexcept {
//         return children_[index];
//     }

//     constexpr tree& operator[](Key index) noexcept {
//         return children_[index];
//     }
    
// private:
//     tree_data_type data_;
//     vector<Key, tree> children_;
//};
