#include "http_headers_parser.h"

#include <regex>

#include <spdlog/spdlog.h>

using namespace std;

// GET /data HTTP/1.1
const regex REGEX_FIRST_LINE("(\\S+) (\\S+) HTTP/\\d.\\d\n");

// Host: 127.0.0.1
const regex REGEX_HEADER("(\\S+): (\\S+)\n");

// CRLF
const regex REGEX_CRLF("\r\n");

void HTTP_Headers_Parser::reset()
{
    state_ = HEADERS_PARSER_FIRST_LINE;
}

HTTP_Headers_Parser_State HTTP_Headers_Parser::parse_headers(Message_Block *mb,
                                                             HTTP_Base &base)
{
    if (!mb) {
        spdlog::error("Failed to parse HTTP messages, 'mb' is nullptr.");
        state_ = HEADERS_PARSER_STATE_ERROR;
    }

    switch (state_) {
    case HEADERS_PARSER_STATE_ERROR:
        spdlog::error("Unexpected state 'HEADERS_PARSER_STATE_ERROR' when parsing HTTP headers.");
        break;

    case HEADERS_PARSER_FIRST_LINE:
        if (parsing_first_line(mb, base) != HANDLE_FINISHED)
            break;

        // Fallthrough
    case HEADERS_PARSER_HEADERS:
        if (parsing_headers(mb, base) != HANDLE_FINISHED)
            break;

        // Fallthrough
    case HEADERS_PARSER_STATE_COMPLATE:
        // Do nothing.
        break;
    }

    return state_;
}

Handle_Status HTTP_Headers_Parser::parsing_first_line(Message_Block *mb,
                                                      HTTP_Base &base)
{
    string valid_buffer(mb->rd_ptr(), mb->length());

    smatch matches;
    if (regex_match(valid_buffer, matches, REGEX_FIRST_LINE) &&
            base.handle_first_line(matches[0])) {
        mb->rd_ptr(matches.length());
        state_ = HEADERS_PARSER_HEADERS;
        return HANDLE_FINISHED;
    }

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Headers_Parser::parsing_headers(Message_Block *mb,
                                                   HTTP_Base &base)
{
    string valid_buffer(mb->rd_ptr(), mb->length());

    smatch matches;

    // Check CRLF first
    if (regex_match(valid_buffer, matches, REGEX_CRLF)) {
        mb->rd_ptr(matches.length());
        state_ = HEADERS_PARSER_STATE_COMPLATE;
        return HANDLE_FINISHED;
    }

    if (regex_match(valid_buffer, matches, REGEX_HEADER)) {
        if (!base.headers().add(matches[0])) {
            spdlog::error("Failed to add parsed header '{}', maybe the format is wrong.",
                          matches[0].str());
            state_ = HEADERS_PARSER_STATE_ERROR;
            return HANDLE_EXIT;
        }

        mb->rd_ptr(matches.length());
    }

    return HANDLE_CONTINUE;
}
