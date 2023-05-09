// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of FullHat library, created to add to std::vector additional operators
//
//===----------------------------------------------------------------------===//

#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Key, typename T>
class vector : protected std::vector<std::pair<Key, T>>
{
protected:
    using parent_type_name = std::vector<std::pair<Key, T>>;
    using p_t = parent_type_name;
    #define delegate_parent(what) this->::parent_type_name::what
public:
    constexpr void push_back(const Key &_key, const T &_value)
    {
        p_t::push_back({ _key, _value });
    }
    constexpr T operator[] (const Key &_key)
    {
        const auto it = std::find_if(p_t::begin(), p_t::end(), [&](const auto el) { return el.first == _key; });
        return *it;
    }

    constexpr typename vector<Key, T>::iterator find(const Key &_key)
    {
        return std::find(this->begin(), this->end(), _key);
    }

    constexpr typename vector<Key, T>::const_iterator find(const Key &_key) const
    {
        return std::find(this->begin(), this->end(), _key);
    }

    constexpr void remove(const Key &_key)
    {
        const auto it = find(_key);
        if (it == this->end())
            throw std::invalid_argument("Can't find key in vector");

        this->erase();
    }
};

#endif // VECTOR_H
