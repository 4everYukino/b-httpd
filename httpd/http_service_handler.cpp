#include "http_service_handler.h"

#include <spdlog/spdlog.h>

HTTP_Service_Handler::HTTP_Service_Handler(int id, int fd) : Task(id), fd_(fd)
{
}

bool HTTP_Service_Handler::run()
{
    if (!stream_.init(fd_)) {
        spdlog::error("Failed to init socket stream by fd '{}'.\n", fd_);
        return false;
    }

    spdlog::debug("HTTP_Service_Handler(ID: {}) accept client '{}'.",
                  id_,
                  stream_.addr());

    // TODO

    return true;
}
