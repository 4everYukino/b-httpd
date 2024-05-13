#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include "http_base.h"

#include <istream>

enum HTTP_Parser_State
{
    HTTP_PARSER_STATE_ERROR = -1,
    HTTP_PARSER_STATE_FIRST_LINE,
    HTTP_PARSER_STATE_HEADERS,
    HTTP_PARSER_STATE_BODY,
};

class HTTP_Parser
{
public:
    HTTP_Parser();

    bool parse(const char* message, HTTP_Base& base);
    bool parse(std::istream& is, HTTP_Base& base);

private:
    HTTP_Parser_State state_ = HTTP_PARSER_STATE_FIRST_LINE;
};

#endif
