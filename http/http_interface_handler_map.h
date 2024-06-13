#ifndef HTTP_INTERFACE_HANDER_MAP_H_
#define HTTP_INTERFACE_HANDER_MAP_H_

#include "http_interface_handler_base.h"

#include <map>
#include <memory>
#include <string>

class HTTP_Interface_Handler_Map
{
public:
    static HTTP_Interface_Handler_Map& instance();

    // The pointer 'factory' will be taken over by smart pointer.
    bool register_handler(const std::string& uri,
                          HTTP_Interface_Handler_Base_Factory* factory);

    HTTP_Interface_Handler_Base* find(const std::string& uri);

private:
    HTTP_Interface_Handler_Map();

    ~HTTP_Interface_Handler_Map();

    HTTP_Interface_Handler_Map(const HTTP_Interface_Handler_Map& other) = delete;

    const HTTP_Interface_Handler_Map& operator=(const HTTP_Interface_Handler_Map& other) = delete;

private:
    std::map<std::string, std::unique_ptr<HTTP_Interface_Handler_Base_Factory>> factories_;
};

#endif
