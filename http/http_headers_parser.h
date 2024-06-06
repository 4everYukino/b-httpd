#ifndef HTTP_HEADERS_PARSER_H_
#define HTTP_HEADERS_PARSER_H_

#include "common.h"
#include "http_base.h"

#include "../utils/message_block.h"

enum HTTP_Headers_Parser_State
{
    HEADERS_PARSER_STATE_ERROR = -1,
    HEADERS_PARSER_FIRST_LINE,
    HEADERS_PARSER_HEADERS,
    HEADERS_PARSER_STATE_COMPLATE
};

class HTTP_Headers_Parser
{
public:
    void reset();

    HTTP_Headers_Parser_State parse_headers(Message_Block* mb, HTTP_Base& base);

private:
    Handle_Status parsing_first_line(Message_Block* mb, HTTP_Base& base);

    Handle_Status parsing_headers(Message_Block* mb, HTTP_Base& base);

private:
    HTTP_Headers_Parser_State state_ = HEADERS_PARSER_STATE_ERROR;
};

#endif
