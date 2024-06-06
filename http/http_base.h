#ifndef HTTP_BASE_H_
#define HTTP_BASE_H_

#include "http_headers.h"

#include <sys/uio.h>

#include <string>
#include <vector>

class HTTP_Base
{
public:
    HTTP_Base();

    virtual void reset() = 0;

    void version(const char* version);
    const char* version() const;

    void headers(const HTTP_Headers& headers);
    HTTP_Headers headers() const;
    HTTP_Headers& headers();
    void print_headers() const;

    void body(const char* body);
    std::string body() const;
    std::string& body();
    void print_body() const;

    bool handle_first_line(const std::string& line);
    virtual bool handle_first_line_i(const std::vector<std::string>& vec) = 0;

    virtual bool to_iovec(std::vector<iovec>& vec) const = 0;

    virtual void show() const = 0;

protected:
    std::string version_;
    HTTP_Headers headers_;
    std::string body_;
};

#endif
