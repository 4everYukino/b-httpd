#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include "http_base.h"

#include <istream>

enum HTTP_Parser_State
{
    HTTP_PARSER_STATE_ERROR = -1,
    HTTP_PARSER_STATE_FIRST_LINE,
    HTTP_PARSER_STATE_HEADERS,
    HTTP_PARSER_STATE_CONTENT,
    HTTP_PARSER_STATE_COMPLATE,
};

class HTTP_Parser
{
public:
    HTTP_Parser();

    bool parse(const char* message, HTTP_Base& base);
    bool parse(std::istream& is, HTTP_Base& base);

    bool complated() const;

private:
    bool ends_with_CRLF(std::string& line);

private:
    HTTP_Parser_State state_ = HTTP_PARSER_STATE_FIRST_LINE;

    size_t content_length_ = 0;
};

#endif
