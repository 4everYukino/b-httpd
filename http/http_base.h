#ifndef HTTP_BASE_H_
#define HTTP_BASE_H_

#include "http_headers.h"

#include <string>
#include <vector>

class HTTP_Base
{
public:
    HTTP_Base();

    void version(const char* version);
    const char* version() const;

    void headers(const HTTP_Headers& headers);
    HTTP_Headers headers() const;
    HTTP_Headers& headers();

    void body(const char* body);
    std::string body() const;
    std::string& body();

    bool handle_first_line(const std::string& line);
    virtual bool handle_first_line_i(const std::vector<std::string>& vec) = 0;

    virtual void show() const = 0;

protected:
    std::string version_;
    HTTP_Headers headers_;
    std::string body_;
};

#endif
