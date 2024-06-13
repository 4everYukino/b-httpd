#include "http_interface_handler_map.h"

#include <spdlog/spdlog.h>

HTTP_Interface_Handler_Map& HTTP_Interface_Handler_Map::instance()
{
    static HTTP_Interface_Handler_Map map;
    return map;
}

bool HTTP_Interface_Handler_Map::register_handler(
        const std::string& uri,
        HTTP_Interface_Handler_Base_Factory* factory)
{
    if (factories_.find(uri) != factories_.end()) {
        spdlog::error("Failed to register '{}' to factories, it has been registered.", uri);
        return false;
    }

    factories_[uri].reset(factory);
    return !factories_[uri];
}

HTTP_Interface_Handler_Base* HTTP_Interface_Handler_Map::find(const std::string& uri)
{
    auto it = factories_.find(uri);
    if (it == factories_.end())
        return nullptr;

    if (!it->second) {
        spdlog::error("The pointer of factory '{}' is nullptr.", uri);
        return nullptr;
    }

    return it->second->create();
}

HTTP_Interface_Handler_Map::HTTP_Interface_Handler_Map()
{
    spdlog::debug("HTTP_Interface_Handler_Map is constructed.");
}

HTTP_Interface_Handler_Map::~HTTP_Interface_Handler_Map()
{
    spdlog::debug("HTTP_Interface_Handler_Map is destructed.");
}
