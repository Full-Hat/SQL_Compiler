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
    void push_back(const Key &_key, const T &_value)
    {
        p_t::push_back({ _key, _value });
    }
    T operator[] (const Key &_key)
    {
        const auto it = std::find_if(p_t::begin(), p_t::end(), [&](const auto el) { return el.first == _key; });
        return *it;
    }
};

#endif // VECTOR_H
