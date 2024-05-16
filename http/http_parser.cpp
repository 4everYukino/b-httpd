#include "http_parser.h"

#include <sstream>

#include <boost/algorithm/string.hpp>

#include <spdlog/spdlog.h>

using boost::ends_with;
using namespace std;

#define SET_ERROR_AND_RETURN() \
    do { \
        state_ = HTTP_PARSER_STATE_ERROR; \
        return false; \
    } while (0)

HTTP_Parser::HTTP_Parser()
{

}

bool HTTP_Parser::parse(const char* message, HTTP_Base& base)
{
    if (!message) {
        spdlog::error("The 'message' should not be nullptr.\n");
        return false;
    }

    istringstream stream(message);
    return parse(stream, base);
}

bool HTTP_Parser::parse(istream& stream, HTTP_Base& base)
{
    if (stream.bad()) {
        spdlog::error("Failed to parse HTTP messages from istream, the status is bad.\n");
        return false;
    }

    // Reset 'state_'
    state_ = HTTP_PARSER_STATE_FIRST_LINE;

    string line;
    while (getline(stream, line)) {
        switch (state_) {
            case HTTP_PARSER_STATE_FIRST_LINE:
                if (!ends_with_CRLF(line) || !base.handle_first_line(line))
                    SET_ERROR_AND_RETURN();

                state_ = HTTP_PARSER_STATE_HEADERS;
                break;

            case HTTP_PARSER_STATE_HEADERS:
                if (!ends_with_CRLF(line))
                    SET_ERROR_AND_RETURN();

                if (line.empty()) {
                    // Determine whether it is necessary to parse the request body.
                    auto content_length = base.headers().get("Content-Length");
                    if (content_length.empty()) {
                        state_ = HTTP_PARSER_STATE_COMPLATE;
                        return true;
                    }

                    content_length_ = atoll(content_length.c_str());
                    state_ = HTTP_PARSER_STATE_CONTENT;
                    break;
                }

                if (!base.headers().add(line)) {
                    spdlog::error("Failed to add header '{}'.\n", line);
                    SET_ERROR_AND_RETURN();
                }

                break;

            case HTTP_PARSER_STATE_CONTENT:

                // TODO
                break;

            case HTTP_PARSER_STATE_ERROR:
                spdlog::error("Unexpected state when parsing HTTP messages.");
                return false;

            default:
                break;
        }
    }

    return true;
}

bool HTTP_Parser::complated() const
{
    return state_ == HTTP_PARSER_STATE_COMPLATE;
}

bool HTTP_Parser::ends_with_CRLF(string& line)
{
    if (!ends_with(line, "\r")) {
        spdlog::error("HTTP messages line '{}' doesn't ends with CRLF.", line);
        return false;
    }

    line.pop_back();
    return true;
}
