#ifndef HTTP_SERVICE_HANDLER_H_
#define HTTP_SERVICE_HANDLER_H_

#include "../utils/task.h"

#include "../utils/message_block.h"

#include "../http/common.h"
#include "../http/http_sock_stream.h"

#include "../http/http_interface_handler_base.h"

#include "../http/http_headers_parser.h"
#include "../http/http_content_parser.h"

#include "../http/http_request.h"
#include "../http/http_response.h"

#include <memory>

enum HTTP_Service_State
{
    STATE_CONNECTING,
    STATE_CONNECTED,

    STATE_HEADER_RECEIVING,
    STATE_HEADER_RECEIVED,

    STATE_CONTENT_RECEIVING,
    STATE_CONTENT_RECEIVED,

    STATE_CLOSE
};

class HTTP_Service_Handler final : Task
{
public:
    HTTP_Service_Handler(int id,
                         int fd);

    bool run() override;

    void after_failed() override;

    void after_complated() override;

private:
    bool run_i();

    Handle_Status handle_reading();

    Handle_Status handle_data();

    Handle_Status handle_header_receiving();

    Handle_Status handle_header_received();

    Handle_Status handle_content_receiving();

    Handle_Status handle_content_received();

private:
    int fd_ = -1;

    int state_ = STATE_CONNECTING;

    HTTP_Sock_Stream stream_;

    Message_Block mb_;

    HTTP_Headers_Parser headers_parser_;
    HTTP_Content_Parser content_parser_;

    HTTP_Request request_;
    unsigned long content_length_ = 0;
    unsigned long content_received_ = 0;

    std::unique_ptr<HTTP_Interface_Handler_Base> interface_handler_;

    HTTP_Response response_;
};

#endif
