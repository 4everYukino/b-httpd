#include "http_response.h"

#include <json/json.h>
#include <spdlog/spdlog.h>

HTTP_Response::HTTP_Response()
{

}

void HTTP_Response::status(int status)
{
    status_ = status;
}

int HTTP_Response::status() const
{
    return status_;
}

bool HTTP_Response::handle_first_line_i(
        const std::vector<std::string>& vec)
{
    // HTTP/1.1 200 OK
    version_ = vec[0];
    status_ = atoi(vec[1].c_str());
    res_phrase_ = vec[2];

    return true;
}

void HTTP_Response::show() const
{
    spdlog::debug("{} {} {}",
                  version_, status_, res_phrase_);

    print_headers();

    print_body();
}
