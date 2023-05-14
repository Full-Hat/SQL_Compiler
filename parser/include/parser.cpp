#include "parser.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

#include "Containers/unique_ptr.h"
#include "tokens.h"

// SELECT * FROM table
// DELETE FROM table WHERE 1 = 1

constexpr unique_ptr<tokens_data_t> parser::parse_to_objects(const std::string &_string_to_parse)
{
    std::string current_parsed_string;
    std::vector<std::string> words;

    //auto select_tokens = tokens.m_select.tokens;
    //auto delete_tokens = tokens.m_delete.tokens;

    for (const auto ch : _string_to_parse)
    {
        if (ch == ' ')
        {
            if (current_parsed_string.size() == 0)
            {
                continue;
            }

            words.push_back(current_parsed_string);
            current_parsed_string.clear();
            continue;
        }
        const std::vector<char> deleters { ',', '(', ')', '*', '=' };
        if (std::find(deleters.cbegin(), deleters.cend(), ch) != deleters.cend())
        {
            words.push_back(current_parsed_string);
            current_parsed_string.clear();
            words.push_back(std::to_string(ch));
            continue;
        }
        current_parsed_string += ch;
    }

    // Here we get words from input string 
    auto parsed_query = tokens_data_objects.try_parse(words);
    if (parsed_query.has_value())
    {
        throw std::runtime_error("Can't parse query, maybe there is error in string or class has not implemented");
    }

    return parsed_query;
}

// constexpr tree<token_class_type, unique_ptr<token_value>> parser::make_tree(const unique_ptr<tokens_data_t> &_objects)
// {
//     return {};
// }