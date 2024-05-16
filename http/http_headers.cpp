#include "http_headers.h"

#define SEPARATOR ": "
#define SEPARATOR_LEN (sizeof(SEPARATOR) - 1)

using namespace std;

HTTP_Headers::HTTP_Headers()
{

}

bool HTTP_Headers::add(const string& header)
{
    // TODO: Use regex to aviod bug.
    const auto pos = header.find(SEPARATOR);
    if (pos == string::npos)
        return false;

    return maps_.insert(
            make_pair(string(header.begin(), header.begin() + pos),
                      string(header.begin() + pos + SEPARATOR_LEN, header.end()))).second;
}

bool HTTP_Headers::add(const char* key, const char* value)
{
    if (!key || !value)
        return false;

    return maps_.insert(make_pair(key, value)).second;
}

string HTTP_Headers::get(const char* key) const
{
    string value;
    if (!key)
        return value;

    auto it = maps_.find(key);
    if (it != maps_.end())
        value = it->second;

    return value;
}
