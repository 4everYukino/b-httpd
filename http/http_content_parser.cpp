#include "http_content_parser.h"

#include "common.h"

#include <spdlog/spdlog.h>

#include <regex>

using namespace std;

const regex REGEX_SIZE("(\\d+)\r\n");

const regex REGEX_CRLF("\r\n");

void HTTP_Content_Parser::reset()
{
    state_ = CHUNKED_PARSER_STATE_CHUNK_SIZE;
    expected_size_ = 0;
    received_size_ = 0;
}

Chunked_Parser_State HTTP_Content_Parser::parsing_chunked(Message_Block* mb, string& content)
{
    /*

    13\r\n
    Hello, World!
    \r\n

    0\r\n
    \r\n

    */
    switch (state_) {
    case CHUNKED_PARSER_STATE_ERROR:
        spdlog::error("Unexpected state 'CHUNKED_PARSER_STATE_ERROR' "
                      "when parsing chunked content.");
        break;

    case CHUNKED_PARSER_STATE_CHUNK_SIZE:
        if (parsing_chunk_size(mb) != HANDLE_FINISHED)
            break;

        // Fallthrough
    case CHUNKED_PARSER_STATE_DATA:
        if (parsing_chunk_data(mb, content) != HANDLE_FINISHED)
            break;

        // Fallthrough
    case CHUNKED_PARSER_STATE_END_CRLF:
        if (parsing_chunk_crlf(mb) != HANDLE_FINISHED)
            break;

        // Fallthrough
    case CHUNKED_PARSER_STATE_FINISHED:
        // Do nothing.
        break;
    }

   return state_;
}

Handle_Status HTTP_Content_Parser::parsing_chunk_size(Message_Block* mb)
{
    string valid_buffer(mb->rd_ptr(), mb->length());

    smatch matches;
    if (regex_match(valid_buffer, matches, REGEX_SIZE)) {
        expected_size_ = ::strtoul(matches[1].str().c_str(), nullptr, 10);
        mb->rd_ptr(matches[1].length());

        // If chunk size is 0, which means this is the last chunk, parse CRLF directly.
        state_ = expected_size_ == 0 ? CHUNKED_PARSER_STATE_END_CRLF
                                     : CHUNKED_PARSER_STATE_DATA;
        return HANDLE_FINISHED;
    }

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Content_Parser::parsing_chunk_data(Message_Block* mb, string& content)
{
    auto remain = expected_size_ - received_size_;

    auto n = ::min(remain, mb->length());

    content.clear();
    content.assign(mb->rd_ptr(), n);

    mb->rd_ptr(n);
    received_size_ += n;

    if (received_size_ == expected_size_) {
        state_ = CHUNKED_PARSER_STATE_END_CRLF;
        return HANDLE_FINISHED;
    }

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Content_Parser::parsing_chunk_crlf(Message_Block* mb)
{
    string valid_buffer(mb->rd_ptr(), mb->length());

    if (!regex_match(valid_buffer, REGEX_CRLF))
        return HANDLE_CONTINUE;

    mb->rd_ptr(CRLF_LEN);

    // If this is the last chunk, finish parsing.
    if (expected_size_ == 0) {
        state_ = CHUNKED_PARSER_STATE_FINISHED;
        return HANDLE_FINISHED;
    }

    // Else, going to parse next chunk.
    reset();
    return HANDLE_FINISHED;
}
