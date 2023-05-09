#pragma once

#include <memory>
#include <optional>
#include <type_traits>
template<typename T>
class unique_ptr 
{
protected:
    T* m_value = nullptr;
public:
    constexpr unique_ptr() = default;
    constexpr unique_ptr(T* _value) : m_value(_value) 
    {
        _value = nullptr;
    }
    constexpr unique_ptr(unique_ptr &_value)
    {
        m_value = _value.m_value;
        _value.m_value = nullptr;
    }
    constexpr unique_ptr(unique_ptr &&_value)
    {
        m_value = _value.m_value;
        _value.m_value = nullptr;
    }
    constexpr unique_ptr(const std::nullopt_t) 
    {

    }
    constexpr ~unique_ptr()
    {
        if (std::is_array<T>::value)
        {
            delete[] m_value;
        }
        else 
        {
            delete m_value;
        }
    }

    // Operators
    T operator* () const 
    {
        return *m_value;
    }

    unique_ptr<T>& operator= (unique_ptr&& _value)
    {
        m_value = _value.m_value;
        return *this;
    }

    unique_ptr<T>& operator= (const unique_ptr& _value) = delete;
    /*{
        m_value = _value.m_value;
        _value.m_value = nullptr;
        return *this;
    }*/

    unique_ptr<T>& operator= (T*& _value)
    {
        m_value = _value;
        _value = nullptr;
        return *this;
    }

    T* operator-> ()
    {
        return m_value;
    }

    const T* operator-> () const
    {
        return m_value;
    }

    // Methods 
    template<typename M>
    M& as()
    {
        return *dynamic_cast<M>(m_value);
    }

    constexpr bool has_value() const 
    {
        return m_value != nullptr;
    }
};

template <typename T>
constexpr unique_ptr<T> make_unique()
{
    return unique_ptr<T>(new T);
}

template <typename B, typename I>
constexpr unique_ptr<B> make_unique_as()
{
    return unique_ptr<B>(new I);
}
