#include "VK_API_chat.h"

vk_api::Message::Message (nlohmann::json input) {
    timestamp = input["date"];
    id = input["id"];
    text = input["body"];
    ifRead = input["read_state"] == 1;
    if (input["out"] == 1) {
        senderId = 0;
        receiverId = input["user_id"];
    } else {
        senderId = input["user_id"];
        receiverId = 0;
    }
}

vk_api::Message::Message (time_t Timestamp, vkid_t Id, vkid_t SenderId, vkid_t ReceiverId, std::string Text, bool IfRead) :
timestamp(Timestamp), id(Id), senderId(SenderId), receiverId(ReceiverId), text(Text), ifRead(IfRead) {}


vk_api::ChatBot::ChatBot (std::string accessToken) : longpoll_() {
    longpoll_.initialize(accessToken);
}

vk_api::ChatBot::~ChatBot () {}

void markAsRead (vkid_t userId) {
    // TODO Mark as read
}

void sendMessage (vkid_t receiver, std::string text) {
    // TODO Send message
}

std::vector<vk_api::Message> vk_api::ChatBot::getUnreadMessages_ () {
    std::vector<vk_api::Message> result;
    // TODO get unread
    return result;
}
