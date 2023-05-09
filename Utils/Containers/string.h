#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr std::vector<std::string> split(const std::string_view &_str, const char _del)
{
    std::string buff;
    buff.reserve(10);

    std::vector<std::string> result;

    for (auto i = 0; i < _str.size(); i++)
    {
        if (_str[i] == _del)
        {
            result.push_back(std::move(buff));
            do
            {
                ++i;
            } while (i < _str.size() && _str[i] == _del);
        }
        else
        {
            buff += _str[i];
        }
    }

    return result;
}