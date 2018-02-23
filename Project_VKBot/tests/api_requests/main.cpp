#include <iostream>

#include "VK_API.h"

using namespace std;
using json = nlohmann::json;

int main()
{
    try {
        json buffer = vk_api::apiRequest("groups.getMembers", "group_id=157926798"); // Token not needed
        cout << buffer;
    }
    catch (exception e) {
        cout << e.what();
    }
    return 0;
}
