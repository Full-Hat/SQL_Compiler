#include <string>
#include <vector>

#include "tree.h"

class sql_rools
{

};

class parser
{
    class token_value : public std::string
    {
        
    };
    template<typename T = token_value>
    class token
    {
    public:
        T m_value;
        constexpr operator T() const
        {
            return m_value;
        }

        constexpr token<T>& operator= (const std::string &_value)
        {
            m_value = _value;
            return *this;
        }
        constexpr token() : m_value()
        {

        }
        constexpr token(const T &_value) : m_value(_value) 
        {
            
        }
    };
    enum class token_type
    {
        
    };
protected:
    constexpr std::vector<token<>> parse_to_tokens(const std::string &_string_to_parse);
    constexpr tree<token_type, token<>> make_tree(const std::vector<token<>> &_tokens);
public:
    std::string parse(const sql_rools &_rools);
};