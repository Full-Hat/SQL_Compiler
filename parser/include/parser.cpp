#include "parser.h"
#include <algorithm>
#include <cstring>
#include <string>

#include "tokens.h"

// SELECT * FROM table
// DELETE FROM table WHERE 1 = 1

constexpr std::vector<parser::token<>> parser::parse_to_tokens(const std::string &_string_to_parse)
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


    return {};
}

constexpr tree<parser::token_type, parser::token<>> parser::make_tree(const std::vector<token<>> &_tokens)
{
    return {};
}