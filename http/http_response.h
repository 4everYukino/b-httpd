#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include "http_base.h"

class HTTP_Response : public HTTP_Base
{
public:
    HTTP_Response();

    void status(int status);
    int status() const;

    bool handle_first_line_i(const std::vector<std::string>& vec) override;

    void show() const override;

private:
    int status_;
    std::string res_phrase_;
};

#endif
