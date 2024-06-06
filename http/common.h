#ifndef COMMON_H_
#define COMMON_H_

#define CRLF "\r\n"
#define CRLF_LEN (sizeof(CRLF) - 1)

enum Handle_Status
{
    HANDLE_EXIT = -1,
    HANDLE_CONTINUE = 0,
    HANDLE_FINISHED = 1,
};

#endif
