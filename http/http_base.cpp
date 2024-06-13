#include "http_base.h"

#include <boost/algorithm/string.hpp>

#include <json/json.h>
#include <json/reader.h>

#include <spdlog/spdlog.h>

using boost::split;
using boost::is_any_of;
using namespace std;

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

void HTTP_Base::print_headers() const
{
    headers_.walk([](const char* key, const char* value) {
        spdlog::debug("{}: {}", key, value);
        return true;
    });
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

void HTTP_Base::print_body() const
{
    if (body_.empty())
        return;

    auto type = headers_.get("Content-Type");
    if (type != "application/json") {
        spdlog::debug("Body: {}", body_);
        return;
    }

    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(body_, value)) {
        spdlog::error("Failed to parse body to JSON.");
        spdlog::debug("Body: {}", body_);
        return;
    }

    spdlog::debug("Body: {}", value.toStyledString());
}

bool HTTP_Base::handle_first_line(const string& line)
{
    std::vector<std::string> vec;
    split(vec, line, is_any_of(" "));
    if (vec.size() != 3) {
        spdlog::error("Failed to parse HTTP request first line '{}', "
                      "the size should be 3.\n",
                      line);
        return false;
    }

    return handle_first_line_i(vec);
}
