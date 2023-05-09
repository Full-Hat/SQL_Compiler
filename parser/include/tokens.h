#pragma once 

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <iostream>

#include "Containers/unique_ptr.h"

#define IMPLEMENT_GET_VECTOR

#define STATIC_CONST static constexpr std::vector<std::string_view>
#define CONST constexpr std::vector<std::string_view>

#define CREATE_ARRAY(name, ...) \
    static constexpr auto name = \
    [] { \
        constexpr std::string_view arr[] = {__VA_ARGS__}; \
        std::array<std::string_view, sizeof(arr) / sizeof(arr[0])> result; \
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) { \
            result[i] = arr[i]; \
        } \
        return result; \
    }(); 

enum TOKEN_TYPES
{
    SELECT, 
    DELETE, 
    NONE
};

struct tokens_data_t
{
    virtual constexpr TOKEN_TYPES get_type() const = 0;

    virtual ~tokens_data_t() = default;
};

struct select_data_t : public tokens_data_t
{
    virtual constexpr TOKEN_TYPES get_type() const override
    {
        return TOKEN_TYPES::SELECT;
    }

    // "SELECT [m_col_names] FROM m_target_name"
    std::vector<std::string> m_col_names;
    std::string m_target_name;
};

struct delete_data_t : public tokens_data_t
{
    virtual constexpr TOKEN_TYPES get_type() const override
    {
        return TOKEN_TYPES::DELETE;
    }

    // "DELETE FROM m_target_name WHERE [m_checks.first = m_checks.second, ...]"
    std::string m_target_name;
    std::vector<std::pair<std::string, std::string>> m_checks;
};

struct tokens_t
{
    constexpr tokens_t() = default;
    virtual constexpr ~tokens_t() = default;
    // Support functions 

    virtual constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string_view> &_query) const = 0;
};

struct select_t : public tokens_t
{
    // "SELECT <NAMES> FROM <NAME>"
    CREATE_ARRAY(tokens, "SELECT", ".NAMES", "FROM", ".NAME")

    virtual constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string_view> &_query) const override
    {
        std::optional<unique_ptr<tokens_data_t>> result;
        
        if (_query.size() == 0)
        {
            return result;
        }
        if (_query[0] != "SELECT")
        {
            return result;
        }

        select_data_t* data = new select_data_t;
        auto it = _query.begin() + 1;

        for (; *it != "FROM"; it++)
        {
            if (*it == ",")
            {
                continue;
            }

            data->m_col_names.push_back(std::string(*it));
        }

        it++;
        data->m_target_name = *it;

        return {data};
    }
};

struct delete_t : public tokens_t
{
    // "DELETE FROM <NAME> WHERE <TYPE> = <VALUE>"
    CREATE_ARRAY(tokens, "DELETE FROM", ".NAME", "WHERE", ".NAME", "=", ".NAME")

    virtual constexpr std::optional<unique_ptr<tokens_data_t>> try_parse(const std::vector<std::string_view> &_query) const override
    {
        return std::nullopt;
    }
};

struct tokens_data_objects_t 
{
    using tokens_objs = std::vector<unique_ptr<tokens_t>>;
    tokens_objs m_tokens_data_objs;

    constexpr tokens_data_objects_t()
    {
        m_tokens_data_objs.push_back(make_unique_as<tokens_t, select_t>());
        m_tokens_data_objs.push_back(make_unique_as<tokens_t, delete_t>());
    }

    constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string_view> &_query) const 
    {
        for (const auto &obj : m_tokens_data_objs)
        {
            const auto parsed_obj = obj->try_parse(_query);
            if (parsed_obj.has_value())
            {
                return parsed_obj.value();
            }
        }
        return std::nullopt;
    }
};
