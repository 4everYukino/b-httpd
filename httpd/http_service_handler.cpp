#include "http_service_handler.h"

#include "../http/http_interface_handler_map.h"

#include <spdlog/spdlog.h>

using namespace std;

#define LEAST_MB_LENGTH CRLF_LEN

HTTP_Service_Handler::HTTP_Service_Handler(int id, int fd) : Task(id), fd_(fd)
{
}

bool HTTP_Service_Handler::run()
{
    if (!stream_.init(fd_)) {
        spdlog::error("Failed to init socket stream by fd '{}'.\n", fd_);
        return false;
    }

    state_ = STATE_CONNECTED;
    headers_parser_.reset();
    spdlog::debug("HTTP_Service_Handler(ID: {}) accept client '{}'.",
                  id_,
                  stream_.addr());

    bool rc = run_i();

    spdlog::debug("HTTP_Service_Handler(ID: {}) finished client '{}'.",
                  id_,
                  stream_.addr());

    return rc && state_ != STATE_ERROR;
}

bool HTTP_Service_Handler::run_i()
{
    if (handle_reading() == HANDLE_EXIT) {
        spdlog::error("Failed to read data from client '{}'.",
                      stream_.addr());
        // 500 Server Internal ERROR
        state_ = STATE_ERROR;
        return false;
    }

    while (handle_data() != HANDLE_EXIT && handle_reading() != HANDLE_EXIT) {
        // Do nothing.
    }

    return true;
}

Handle_Status HTTP_Service_Handler::handle_reading()
{
    if (!mb_.init()) {
        spdlog::error("Failed to init message block.");
        // 500 Server Internal ERROR
        return HANDLE_EXIT;
    }

    if (!mb_.space()) {
        spdlog::debug("Not enough buffer space to receiving data from '{}', "
                      "{} is still unprocessed in the buffer.",
                      stream_.addr(),
                      mb_.length());

        mb_.crunch();
        if (!mb_.space()) {
            spdlog::error("No buffer space even after crunching.");
            // 500 Server Internal ERROR
            return HANDLE_EXIT;
        }
    }

    size_t n = stream_.read(mb_.wr_ptr(),  mb_.space());

    if (n == -1) {
        // TODO:
        // 可能要判断是不是假错（被信号打断）
        // return HANDLE_CONTINUE;

        spdlog::error("Failed to receive data from '{}', errno={}, {}.",
                      stream_.addr(),
                      errno,
                      strerror(errno));
        return HANDLE_EXIT;
    }

    if (n == 0) {
        spdlog::debug("Connection is closed by '{}'.", stream_.addr());
        state_ = STATE_CLOSE;
        return HANDLE_EXIT;
    }

    spdlog::debug("Received {} data from client '{}'.",
                  n,
                  stream_.addr());

    mb_.wr_ptr(n);

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Service_Handler::handle_data()
{
    Handle_Status rc = HANDLE_EXIT;
    switch (state_) {
    case STATE_CONNECTING:
        // This situation should not occured.
        spdlog::error("Unexpected state, not connected yet.");
        return HANDLE_EXIT;
    case STATE_CONNECTED:
        state_ = STATE_HEADER_RECEIVING;

        // Fallthrough
    case STATE_HEADER_RECEIVING:
        if ((rc = handle_header_receiving()) != HANDLE_FINISHED)
            return rc;

        // Fallthrough
    case STATE_HEADER_RECEIVED:
        if ((rc = handle_header_received()) != HANDLE_FINISHED)
            return rc;

        // Fallthrough
    case STATE_CONTENT_RECEIVING:
        if ((rc = handle_content_receiving()) != HANDLE_FINISHED)
            return rc;

        // Fallthrough
    case STATE_CONTENT_RECEIVED:
        if ((rc = handle_content_received()) != HANDLE_FINISHED)
            return rc;
    }

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Service_Handler::handle_header_receiving()
{
    switch (headers_parser_.parse_headers(&mb_, request_)) {
    case HEADERS_PARSER_STATE_ERROR:
        spdlog::error("Failed to parse HTTP headers from client '{}'.",
                      stream_.addr());
        state_ = STATE_ERROR;
        return HANDLE_EXIT;

    case HEADERS_PARSER_STATE_COMPLATE:
        state_ = STATE_HEADER_RECEIVED;
        return HANDLE_FINISHED;

    // 没解析完，break 出去 return HANDLE_CONTINUE 以继续接收数据并处理。
    case HEADERS_PARSER_FIRST_LINE:
    case HEADERS_PARSER_HEADERS:
        break;
    }

    return HANDLE_CONTINUE;
}

Handle_Status HTTP_Service_Handler::handle_header_received()
{
    interface_handler_.reset(HTTP_Interface_Handler_Map::instance().find(request_.uri()));
    if (!interface_handler_) {
        spdlog::error("Failed to find handler to handle '{}'.",
                      request_.uri());
        // 404 NOT FOUND
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    if (interface_handler_->handle_request(request_)) {
        spdlog::error("Failed to handle '{}' by '{}'.",
                      request_.uri(),
                      interface_handler_->name());
        // 500 Server Internal ERROR
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    content_length_ = request_.content_length();
    if (!content_length_) {
        state_ = STATE_CONTENT_RECEIVED;
        return HANDLE_FINISHED;
    }

    state_ = STATE_CONTENT_RECEIVING;
    return HANDLE_FINISHED;
}

Handle_Status HTTP_Service_Handler::handle_content_receiving()
{
    string content;
    if (request_.chunked()) {
        content_parser_.reset();
        do {
            if (mb_.length() < LEAST_MB_LENGTH)
                return HANDLE_CONTINUE;

            content.clear();
            switch (content_parser_.parsing_chunked(&mb_, content)) {
            case CHUNKED_PARSER_STATE_CHUNK_SIZE:
                break;

            case CHUNKED_PARSER_STATE_DATA:
                // 不要堆积所有 content 再一起处理，有多少就先处理多少。
                if (!interface_handler_->handle_content(content))
                    return HANDLE_EXIT;

                break;
            case CHUNKED_PARSER_STATE_END_CRLF:
                break;

            case CHUNKED_PARSER_STATE_FINISHED:
                state_ = STATE_CONTENT_RECEIVED;
                break;

            case CHUNKED_PARSER_STATE_ERROR:
                spdlog::error("Failed to parse content from client '{}'.", stream_.addr());
                state_ = STATE_ERROR;
                return HANDLE_EXIT;
            }
        } while (state_ != STATE_CONTENT_RECEIVED);
    } else {
        auto remain = content_length_ - content_received_;
        if (!remain) {
            return HANDLE_FINISHED;
        }

        if (!mb_.length())
            return HANDLE_CONTINUE;

        auto n = min(remain, mb_.length());

        content.assign(mb_.rd_ptr(), n);
        mb_.rd_ptr(n);

        content_received_ += n;
        if (content_received_ > content_length_) {
            spdlog::error("Unexpected logical error occured, "
                          "{} of content was received, which is larger then Content-Length {}.",
                          content_received_,
                          content_length_);
            // 500 Server Internal ERROR
            state_ = STATE_ERROR;
            return HANDLE_EXIT;
        }

        if (!interface_handler_->handle_content(content))
            return HANDLE_EXIT;

        return content_received_ == content_length_ ? HANDLE_FINISHED
                                                    : HANDLE_CONTINUE;
    }

    return HANDLE_FINISHED;
}

Handle_Status HTTP_Service_Handler::handle_content_received()
{
    response_.reset();
    if (!interface_handler_->handle_over(response_)) {
        spdlog::error("Failed to get response to response client '{}'.",
                      stream_.addr());
        // 500 Server Internal ERROR
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    vector<iovec> vec;
    if (!response_.to_iovec(vec)) {
        spdlog::error("Failed to seperate response to iovec.");
        // 500 Server Internal ERROR
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    if (!stream_.write(vec)) {
        spdlog::error("Failed to send response to client '{}', errno={}, {}.",
                      stream_.addr(),
                      errno,
                      strerror(errno));
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    if (!interface_handler_->send_content(&stream_)) {
        spdlog::error("Failed to send content to client '{}'.",
                      stream_.addr());
        state_ = STATE_ERROR;
        return HANDLE_EXIT;
    }

    // 如果 keep alive，则重新开始接收 headers。
    if (request_.keep_alive()) {
        request_.reset();
        state_ = STATE_HEADER_RECEIVING;
        return HANDLE_CONTINUE;
    }

    state_ = STATE_CLOSE;
    return HANDLE_EXIT;
}
