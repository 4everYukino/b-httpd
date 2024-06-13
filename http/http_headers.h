#ifndef HTTP_HEADERS_H_
#define HTTP_HEADERS_H

#include <algorithm>
#include <string>
#include <unordered_map>

using headers_map = std::unordered_map<std::string, std::string>;

class HTTP_Headers
{
public:
    void reset();

    bool add(const std::string& header);
    bool add(const char* key, const char* value);

    std::string get(const char* key) const;

    template<class T>
    bool walk(T op) const
    {
        return std::all_of(maps_.begin(),
                           maps_.end(),
                           [&](const std::pair<std::string, std::string>& i) {
            return op(i.first.c_str(), i.second.c_str());
        });
    }

private:
    headers_map maps_;
};

#endif
