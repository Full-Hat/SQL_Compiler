#pragma once

#include <string>
#include <vector>

#include "Containers/tree.h"
#include "Containers/unique_ptr.h"
#include "tokens.h"

class parser
{
public:
protected:
    constexpr unique_ptr<tokens_data_t> parse_to_objects(const std::string &_string_to_parse);
    //constexpr tree<token_class_type, unique_ptr<token_value>> make_tree(const unique_ptr<tokens_data_t> &_objects);
public:
    std::string parse();
};