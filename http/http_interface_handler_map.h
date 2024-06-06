#ifndef HTTP_INTERFACE_HANDER_MAP_H_
#define HTTP_INTERFACE_HANDER_MAP_H_

#include "http_interface_handler_base.h"

#include <map>
#include <string>

class HTTP_Interface_Handler_Map
{
public:
    static HTTP_Interface_Handler_Map& instance();

    HTTP_Interface_Handler_Base* find(const std::string& uri);

private:
    HTTP_Interface_Handler_Map();

    ~HTTP_Interface_Handler_Map();

    HTTP_Interface_Handler_Map(const HTTP_Interface_Handler_Map& other) = delete;

    const HTTP_Interface_Handler_Map& operator=(const HTTP_Interface_Handler_Map& other) = delete;

private:
    std::map<std::string, > map_;
};

#endif
