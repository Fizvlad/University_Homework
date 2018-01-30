#include <iostream>
#include <fstream>
#include "VK_API.h"

using namespace std;
using namespace vk_api;
using json = nlohmann::json;

int main()
{
    setlocale(LC_CTYPE, "rus");
    fstream fin("../../private/key.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;
    // Reading messages which were received while bot was offline
    auto r = chat::recieveNewMessages(token);
    for (auto m : r) {
        cout << m.getUserId() << " at " << m.getTimestamp() << " wrote message (id: " << m.getId() << "): " << m.getText() << endl;
        m.markAsRead(token);
    }
    // Creating longpoll
    longpoll::session s;
    s.initialize(token);
    s.listen([&] (json upd) {
                for (auto i : upd) {
                    if ((unsigned short)i[0] == longpoll::EVENTS::MESSAGE::NEW) {
                        cout << i[3] << " at " << i[4] << " wrote message (id: " << i[1] << "): " << i[5].get<string>() << endl;
                        // Mark as read
                        string param = "peer_id=";
                        param += to_string((int)i[3]);
                        param += "&start_message_id=";
                        param += to_string((int)i[1]);
                        apiRequest("messages.markAsRead", param, token);

                        if (i[5] == "!stop")
                            return false;
                    }
                }
                return true;
             });
    return 0;
}
