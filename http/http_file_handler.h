#ifndef HTTP_FILE_HANDLER_H_
#define HTTP_FILE_HANDLER_H_

#include "http_interface_handler_base.h"

class HTTP_File_Handler final : public HTTP_Interface_Handler_Base
{
public:
    bool init() override;

    // Do not handle body here, content shoule be handle on handle_content().
    bool handle_request(const HTTP_Request& request) override;

    bool handle_content(const std::string& content) override;

    // All content received, generate response.
    bool handle_over(HTTP_Response& response) override;

    // Send content to client.
    bool send_content(HTTP_Sock_Stream* stream) override;

    std::string name() const override;

private:
    bool send_file(HTTP_Sock_Stream* stream);

private:
    std::string file_;
};

#endif
