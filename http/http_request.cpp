#include "http_request.h"

#include <boost/algorithm/string/case_conv.hpp>

#include <spdlog/spdlog.h>

HTTP_Request::HTTP_Request()
{

}

void HTTP_Request::method(const char* method)
{
    if (method)
        method_ = method;
}

const char* HTTP_Request::method() const
{
    return method_.c_str();
}

void HTTP_Request::uri(const char* uri)
{
    if (uri)
        uri_ = uri;
}

const char* HTTP_Request::uri() const
{
    return uri_.c_str();
}

void HTTP_Request::query_string(const char* query_string)
{
    if (query_string)
        query_string_ = query_string;
}

const char* HTTP_Request::query_string() const
{
    return query_string_.c_str();
}

bool HTTP_Request::handle_first_line_i(
        const std::vector<std::string>& vec)
{
    // GET /data HTTP/1.1
    method_ = boost::to_upper_copy(vec[0]);
    if (!parse_uri(vec[1]))
        return false;

    version_ = vec[2];

    return true;
}

void HTTP_Request::show() const
{
    std::string uri(uri_);
    if (!query_string_.empty()) {
        uri.append("?")
           .append(query_string_);
    }

    spdlog::debug("{} {} {}",
                  method_, uri, version_);

    headers_.walk([](const char* key, const char* value) {
        spdlog::debug("{}: {}", key, value);
        return true;
    });

    if (!body_.empty())
        spdlog::debug("Body: {}", body_);
}

bool HTTP_Request::parse_uri(const std::string& line)
{
    auto pos = line.find('?');
    if (pos == std::string::npos) {
        uri_ = line;
        return true;
    }

    uri_.assign(line.begin(), line.begin() + pos);
    query_string_.assign(line.begin() + pos + 1, line.end());
    return true;
}
