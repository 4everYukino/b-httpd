#ifndef HTTP_INTERFACE_HANDLER_BASE_H_
#define HTTP_INTERFACE_HANDLER_BASE_H_

#include "http_request.h"
#include "http_response.h"

#include <string>

class HTTP_Interface_Handler_Base
{
public:
    virtual bool init() = 0;

    // Do not handle body here, content shoule be handle on handle_content().
    virtual bool handle_request(const HTTP_Request& request) = 0;

    virtual bool handle_content(const std::string& content) = 0;

    // All content received, generate response.
    virtual bool handle_over(HTTP_Response& response) = 0;

    virtual std::string name() const = 0;

};

#endif
