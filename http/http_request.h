#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "http_base.h"

class HTTP_Request : public HTTP_Base
{
public:
    HTTP_Request();

    void method(const char* method);
    const char* method() const;

    void uri(const char* uri);
    const char* uri() const;

    void query_string(const char* query_string);
    const char* query_string() const;

    bool handle_first_line_i(const std::vector<std::string>& vec) override;

    void show() const override;

private:
    bool parse_uri(const std::string& line);

private:
    std::string method_;
    std::string uri_;
    std::string query_string_;
};

#endif
