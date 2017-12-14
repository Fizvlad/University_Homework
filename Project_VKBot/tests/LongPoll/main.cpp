#include <iostream>
#include <string>

#include "nlohmann/json.hpp" // gcc 4.2+ required

#include "VK_API.h"

using json = nlohmann::json;

int main()
{
    std::cout << "Set your access token: " << std::endl;
    std::string token;
    std::cin >> token;
    auto response = json::parse(vk_api::apiRequest("messages.getLongPollServer", "need_pts=0", token))["response"];
    std::cout << "JSON:" << response << std::endl;
    std::string server = response["server"].get<std::string>();
    std::string key = response["key"].get<std::string>();
    std::string ts = std::to_string(response["ts"].get<long>());
    std::cout << server << " " << key << " " << ts << std::endl;

    return 0;
}
