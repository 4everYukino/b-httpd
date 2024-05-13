#include "http_parser.h"
#include "http_request.h"
#include "http_response.h"

#include <errno.h>
#include <fstream>
#include <string.h>

#include <spdlog/spdlog.h>

#include <json/json.h>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    // Debug spdlog
    spdlog::debug("Hello, i am {}, this message will be displayed by spdlog.", "bruce");

    // Debug HTTP_Parser
    HTTP_Parser parser;

    spdlog::info("Parsing HTTP_Request.txt demo.");
    std::fstream f("../HTTP_request.txt");
    if (!f.is_open()) {
        spdlog::error("Failed to open file, errno={}, {}.",
                      errno, strerror(errno));
        return -1;
    }

    HTTP_Request req;
    if (!parser.parse(f, req)) {
        spdlog::error("Failed to parse HTTP request.");
        return -1;
    }

    f.close();
    req.show();

    spdlog::info("Parsing HTTP_Response.txt demo.");
    f.open("../HTTP_response.txt");
    if (!f.is_open()) {
        spdlog::error("Failed to open file, errno={}, {}.",
                      errno, strerror(errno));
        return -1;
    }

    HTTP_Response res;
    if (!parser.parse(f, res)) {
        spdlog::error("Failed to parse HTTP response.");
        return -1;
    }

    f.close();
    res.show();

    return 0;
}
