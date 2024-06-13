#ifndef HTTP_SOCK_STREAM_H_
#define HTTP_SOCK_STREAM_H_

#include <string>
#include <sys/uio.h>
#include <vector>

class HTTP_Sock_Stream
{
public:
    HTTP_Sock_Stream();

    bool init(int fd);

    std::string addr() const;

    size_t read(char* buffer, size_t len);

    size_t write(char* buffer, size_t len);

    bool write(const std::vector<iovec>& vec);

private:
    int fd_ = -1;

    std::string host_;
    unsigned short port_;
};

#endif
