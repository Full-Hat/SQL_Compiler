#include <algorithm>
#include <utility>
#include <vector>

template <typename Key, typename T>
class vector 
{
protected:
    std::vector<std::pair<Key, T>> m_data;

public:
    void push_back(const Key &_key, const T &_value)
    {
        m_data.push_back({ _key, _value });
    }
    T operator[] (const Key &_key)
    {
        const auto it = std::find_if(m_data->begin(), m_data->end(), [&](const auto el) { return el.first == _key; });
        return *it;
    }
};

template <typename Key, typename T>
class tree {
public:
    using size_type = typename std::vector<std::pair<Key, T>>::size_type;
    using tree_data_type = vector<Key, T>;

    constexpr tree() : data_{}, children_{} {}

    template <typename U = T>
    constexpr tree(const U& data) : data_{data}, children_{} {}

    template <typename U = T>
    constexpr void add_child(U&& data) {
        children_.emplace_back(std::forward<U>(data));
    }

    template <typename U = T>
    constexpr void add_child(const tree<Key, U>& t) {
        children_.push_back(t);
    }

    template <typename U = T>
    constexpr void add_child(tree<Key, U>&& t) {
        children_.push_back(std::move(t));
    }

    constexpr void remove_child(std::size_t i) {
        children_.erase(children_.begin() + i);
    }

    template <typename U = T>
    constexpr void add_value(const Key &_key, const U &_data)
    {
        data_.push_back(_key, _data);
    }

    constexpr void clear() {
        data_ = {};
        children_.clear();
    }

    // Data getters
    constexpr std::conditional_t<std::is_const_v<T>, const T&, T&> value(const Key &_key) noexcept {
        return data_[_key];
    }

    constexpr std::conditional_t<std::is_const_v<T>, const T&, T&> value(const Key &_key) const noexcept {
        return data_[_key];
    }

    constexpr const tree& left() const noexcept {
        return children_[0];
    }

    constexpr tree& left() noexcept {
        return children_[0];
    }

    constexpr const tree& right() const noexcept {
        return children_[1];
    }

    constexpr tree& right() noexcept {
        return children_[1];
    }

    constexpr bool empty() const noexcept {
        return children_.empty();
    }

    constexpr size_t size() const noexcept {
        return children_.size();
    }

    constexpr const tree& operator[](Key index) const noexcept {
        return children_[index];
    }

    constexpr tree& operator[](Key index) noexcept {
        return children_[index];
    }
    
private:
    tree_data_type data_;
    vector<Key, tree> children_;
};
