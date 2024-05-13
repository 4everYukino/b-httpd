#include "http_base.h"

#include <boost/algorithm/string.hpp>

#include <spdlog/spdlog.h>

using namespace std;

HTTP_Base::HTTP_Base()
{

}

void HTTP_Base::version(const char* version)
{
    if (version)
        version_ = version;
}

const char* HTTP_Base::version() const
{
    return version_.c_str();
}

void HTTP_Base::headers(const HTTP_Headers& headers)
{
    headers_ = headers;
}

HTTP_Headers HTTP_Base::headers() const
{
    return headers_;
}

HTTP_Headers& HTTP_Base::headers()
{
    return headers_;
}

void HTTP_Base::body(const char* body)
{
    if (body)
        body_ = body;
}

string HTTP_Base::body() const
{
    return body_;
}

string& HTTP_Base::body()
{
    return body_;
}

bool HTTP_Base::handle_first_line(const string& line)
{
    std::vector<std::string> vec;
    boost::split(vec, line, boost::is_any_of(" "));
    if (vec.size() != 3) {
        spdlog::error("Failed to parse HTTP request first line '{}', "
                      "the size should be 3.\n",
                      line);
        return false;
    }

    return handle_first_line_i(vec);
}
