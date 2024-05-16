#ifndef HTTP_SERVICE_HANDLER_H_
#define HTTP_SERVICE_HANDLER_H_

#include "../utils/task.h"

#include "../http/http_sock_stream.h"

class HTTP_Service_Handler final : Task
{
public:
    HTTP_Service_Handler(int id,
                         int fd);

    bool run() override;

    void after_failed() override;

    void after_complated() override;

private:
    int fd_ = -1;

    HTTP_Sock_Stream stream_;

    // TODO:
    // MessageBlock
};

#endif
