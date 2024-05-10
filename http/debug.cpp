#include <spdlog/spdlog.h>

#include <json/json.h>

int main()
{
    spdlog::info("Hello, i am {}, debuging spdlog.", "bruce");

    Json::Value Object;
    Object["Name"] = "Bruce";
    Object["Age"] = 24;
    Object["Others"]["Wechat"] = "13533658239";
    Object["Others"]["Home"] = "Guangzhou";
    spdlog::info("Debug jsoncpp, {}", Object.toStyledString());

    return 0;
}
