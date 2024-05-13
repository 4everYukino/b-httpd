#include "http_parser.h"

#include <sstream>

#include <boost/algorithm/string.hpp>

#include <spdlog/spdlog.h>

using namespace std;

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
                if (!base.handle_first_line(line))
                    return false;

                state_ = HTTP_PARSER_STATE_HEADERS;
                break;

            case HTTP_PARSER_STATE_HEADERS:
                // CRLF.
                if (line.size() == 1) {
                    state_ = HTTP_PARSER_STATE_BODY;
                    break;
                }

                if (!base.headers().add(line)) {
                    spdlog::error("Failed to add header '{}'.\n", line);
                    return false;
                }

                break;

            case HTTP_PARSER_STATE_BODY:
                base.body().append(line)
                           .push_back('\n');
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
