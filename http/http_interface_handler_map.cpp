#include "http_interface_handler_map.h"

#include <spdlog/spdlog.h>

HTTP_Interface_Handler_Map& HTTP_Interface_Handler_Map::instance()
{
    static HTTP_Interface_Handler_Map map;
    return map;
}

HTTP_Interface_Handler_Map::HTTP_Interface_Handler_Map()
{
    spdlog::debug("HTTP_Interface_Handler_Map is constructed.");
}

HTTP_Interface_Handler_Map::~HTTP_Interface_Handler_Map()
{
    spdlog::debug("HTTP_Interface_Handler_Map is destructed.");
}
