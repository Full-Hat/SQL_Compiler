#pragma once 

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <iostream>

#include "Containers/unique_ptr.h"
#include "Containers/tree.h"

#define IMPLEMENT_GET_VECTOR

#define STATIC_CONST static constexpr std::vector<std::string_view>

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

enum class token_class_type
{
    OPERATION,
    ARGUMENT,
    VARIABLE,
    ARGUMENT_ROOT,
    REFERENCE,
};

enum token_sub_type
{
    SELECT, 
    DELETE, 
    ANY,
    NONE
};

class token_value
{
public:
    virtual constexpr token_class_type get_token_class_type() const = 0;
    virtual constexpr token_sub_type get_token_sub_type() const = 0;
};

struct token_value_select : public token_value
{
    virtual constexpr token_class_type get_token_class_type() const { return token_class_type::OPERATION; };
    virtual constexpr token_sub_type get_token_sub_type() const { return token_sub_type::SELECT; };
};

struct token_value_select_arg_root : public token_value_select
{
    virtual constexpr token_class_type get_token_class_type() const override { return token_class_type::ARGUMENT_ROOT; };
    virtual constexpr token_sub_type get_token_sub_type() const override { return token_sub_type::SELECT; };
};

struct token_value_string : public token_value, public std::string
{
    virtual constexpr token_class_type get_token_class_type() const override { return token_class_type::ARGUMENT; };
    virtual constexpr token_sub_type get_token_sub_type() const override { return token_sub_type::ANY; };
};

struct token_value_variable : public token_value
{
    std::string m_variable_name;
    unique_ptr<std::string> m_real_value;
    virtual constexpr token_class_type get_token_class_type() const override { return token_class_type::VARIABLE; };
    virtual constexpr token_sub_type get_token_sub_type() const override { return token_sub_type::ANY; };
};

struct tokens_data_t
{
    virtual constexpr token_sub_type get_type() const = 0;
    using tree_type = tree_scalar<token_class_type, unique_ptr<token_value>>;
    virtual constexpr tree_type parse_query_to_tree() const = 0;

    virtual ~tokens_data_t() = default;
};

struct select_data_t : public tokens_data_t
{
    virtual constexpr token_sub_type get_type() const override
    {
        return token_sub_type::SELECT;
    }

    using tree_type = tree_scalar<token_class_type, unique_ptr<token_value>>;
    //using tree_node_type = tree_type::node_scalar<unique_ptr<token_value>>;
    virtual constexpr tree_type parse_query_to_tree() const override
    {
        tree_type root;

        auto select_op = make_unique_as<token_value, token_value_select>();
        root.get_values_struct().set_value(std::move(select_op));

        auto arg_root = make_unique_as<token_value, token_value_select_arg_root>();
        auto arg_root_tree = tree_type(std::move(arg_root));
        for (const auto arg : m_col_names)
        {
            unique_ptr<token_value> arg_ptr;
            if (arg[0] == ':')
            {
                arg_ptr = make_unique_as<token_value, token_value_string>();
                arg_root_tree.add_child(token_class_type::ARGUMENT, tree_type(std::move(arg_ptr)));
            }
            else
            {
                arg_ptr = make_unique_as<token_value, token_value_variable>();
                arg_root_tree.add_child(token_class_type::REFERENCE, tree_type(std::move(arg_ptr)));
            }
        }
        root.add_child(token_class_type::ARGUMENT_ROOT, std::move(arg_root_tree));

        auto target_table = make_unique_as<token_value, token_value_string>();
        root.add_child(token_class_type::ARGUMENT, tree_type(std::move(target_table)));
        return root;
    }

    // "SELECT [m_col_names] FROM m_target_name"
    std::vector<std::string> m_col_names;
    std::string m_target_name;
};

struct delete_data_t : public tokens_data_t
{
    virtual constexpr token_sub_type get_type() const override
    {
        return token_sub_type::DELETE;
    }

    // virtual constexpr tree<token_class_type, unique_ptr<token_value>> parse_query_to_tree() const override
    // {
    //     return {};
    // }

    // "DELETE FROM m_target_name WHERE [m_checks.first = m_checks.second, ...]"
    std::string m_target_name;
    std::vector<std::pair<std::string, std::string>> m_checks;
};

struct tokens_t
{
    constexpr tokens_t() = default;
    virtual constexpr ~tokens_t() = default;
    // Support functions 

    virtual constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string> &_query) const = 0;
};

struct select_t : public tokens_t
{
    // "SELECT <NAMES> FROM <NAME>"
    CREATE_ARRAY(tokens, "SELECT", ".NAMES", "FROM", ".NAME")

    virtual constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string> &_query) const override
    {
        unique_ptr<tokens_data_t> result;
        
        if (_query.size() == 0)
        {
            return std::nullopt;
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

    virtual constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string> &_query) const override
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

    constexpr unique_ptr<tokens_data_t> try_parse(const std::vector<std::string> &_query) const 
    {
        for (const auto &obj : m_tokens_data_objs)
        {
            auto parsed_obj = obj->try_parse(_query);
            if (parsed_obj.has_value())
            {
                return parsed_obj;
            }
        }
        return std::nullopt;
    }
};

static const tokens_data_objects_t tokens_data_objects;
