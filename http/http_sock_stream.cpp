#include "http_sock_stream.h"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <spdlog/spdlog.h>

#define MAX_ADDR_LEN 128

HTTP_Sock_Stream::HTTP_Sock_Stream()
{
}

bool HTTP_Sock_Stream::init(int fd)
{
    if (fd <= 0) {
        spdlog::error("Failed to init stream, incorrect fd '{}'.", fd);
        return false;
    }

    fd_ = fd;

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (!getpeername(fd_, (struct sockaddr *)&addr, &len)) {
        spdlog::error("Failed to getpeername, errno={}, {}.",
                      errno, strerror(errno));
        return false;
    }

    port_ = ntohs(addr.sin_port);

    char buffer[len];
    if (!inet_ntop(AF_INET, &addr.sin_addr, buffer, len)) {
        spdlog::error("Failed to inet_ntop, errno={}, {}.",
                      errno, strerror(errno));
        return false;
    }

    host_.assign(buffer);
    return true;
}

std::string HTTP_Sock_Stream::addr() const
{
    std::string res;

    char buf[MAX_ADDR_LEN];
    if (snprintf(buf,
                 MAX_ADDR_LEN,
                 "%s:%d", host_.c_str(), port_) < 0) {
        spdlog::error("Unexcepted error, cannot print addr.");
        return res;
    }

    res.assign(buf);
    return res;
}

size_t HTTP_Sock_Stream::read(char* buffer, size_t len)
{
    return ::read(fd_, buffer, len);
}

size_t HTTP_Sock_Stream::write(const std::vector<iovec>& vec)
{
    return ::writev(fd_, vec.data(), vec.size());
}
