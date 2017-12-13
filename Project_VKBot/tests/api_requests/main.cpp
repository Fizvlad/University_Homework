#include <iostream>
#include "../../include/VK_API.h"

using namespace std;

int main()
{
    try {
        string buffer = vk_api::apiRequest("groups.getMembers", "group_id=157926798"); // Token not needed
        cout << buffer;
    }
    catch (exception e) {
        cout << e.what();
    }
    return 0;
}
