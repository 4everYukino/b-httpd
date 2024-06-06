#ifndef HTTP_CONTENT_PARSER_H_
#define HTTP_CONTENT_PARSER_H_

#include "../utils/message_block.h"

#include <string>

enum Chunked_Parser_State
{
    CHUNKED_PARSER_STATE_ERROR,
    CHUNKED_PARSER_STATE_CHUNK_SIZE,
    CHUNKED_PARSER_STATE_DATA,
    CHUNKED_PARSER_STATE_END_CRLF,
    CHUNKED_PARSER_STATE_FINISHED
};

class HTTP_Content_Parser
{
public:
    void reset();

    Chunked_Parser_State parsing_chunked(Message_Block* mb, std::string& content);

private:
    Handle_Status parsing_chunk_size(Message_Block* mb);

    Handle_Status parsing_chunk_data(Message_Block* mb, std::string& content);

    Handle_Status parsing_chunk_crlf(Message_Block* mb);

private:
    Chunked_Parser_State state_ = CHUNKED_PARSER_STATE_ERROR;

    size_t expected_size_ = 0;
    size_t received_size_ = 0;

    bool last_ = false;
};

#endif
