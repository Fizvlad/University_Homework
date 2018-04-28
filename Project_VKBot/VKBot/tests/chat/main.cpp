#include <iostream>
#include <fstream>
#include "VK_API.h"

using namespace std;
using namespace vk_api;
using json = nlohmann::json;

int main()
{
    setlocale(LC_CTYPE, "rus");
    fstream fin("private/key.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;
    ChatBot bot(token);
    bot.start([&] (Message m, bool isOld) {
        cout << (isOld ? "(Old message) " : "") << m.senderId << " send message to " << m.receiverId << " with id " << m.id << " at " << m.timestamp << ". Content: " << endl << m.text << endl;
        if (m.text == "!help") {
            bot.markAsRead(m);
            bot.sendMessage(m.senderId, "Список доступных комманд:\n !read - прочесть сообщения \n !answer - ответить в беседу \n !stop - остановить работу бота");
        } else if (m.text == "!read") {
            bot.markAsRead(m);
        } else if (m.text == "!answer") {
            bot.markAsRead(m);
            bot.sendMessage(m.senderId, "Честно говоря, мне надоел весь этот чёртов фарс");
        } else if (m.text == "!stop") {
            bot.markAsRead(m);
            return false;
        }
        return true;
    });
    return 0;
}
