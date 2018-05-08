#include "VK_API_chat.h"

namespace {
    #include "utility/encode_decode.cpp"
}


vk_api::Message::Message (nlohmann::json input) :
    timestamp(input["date"]), id(input["id"]), senderId(input["out"] == 1 ? 0 : (vkid_t) input["user_id"]),
    receiverId(input["out"] == 0 ? 0 : (vkid_t) input["user_id"]), text(input["body"].get<std::string>()) {}


vk_api::Message::Message (time_t Timestamp, vkid_t Id, vkid_t SenderId, vkid_t ReceiverId, std::string Text) :
timestamp(Timestamp), id(Id), senderId(SenderId), receiverId(ReceiverId), text(Text) {}


vk_api::ChatBot::ChatBot (std::string accessToken, vk_api::vkid_t groupId) : longpoll_(), accessToken_(accessToken), groupId_(groupId) {
    longpoll_.initialize(accessToken);
}


vk_api::ChatBot::~ChatBot () {}


void vk_api::ChatBot::markAsRead (vk_api::Message message) {
    std::stringstream param;
    param << "peer_id=" << message.senderId << "&start_message_id=" << message.id;
    apiRequest("messages.markAsRead", param.str(), accessToken_);
}


void vk_api::ChatBot::sendMessage (vkid_t receiver, std::string text) {
    std::stringstream param;
    param << "user_id=" << receiver << "&message=" << str_encode(text);
    vk_api::apiRequest("messages.send", param.str(), accessToken_);
}


std::vector<vk_api::Message> vk_api::ChatBot::getUnreadMessages_ () {
    size_t count = 0;
    std::vector<vk_api::Message> result(0);
    do {
        std::stringstream param;
        param << "count=200&unread=1&offset=" << result.size();
        nlohmann::json response = vk_api::apiRequest("messages.getDialogs", param.str(), accessToken_);
        size_t count = response["count"];
        result.reserve(count);
        for (auto i : response["items"]) {
            Message message(i["message"]);
            result.push_back(message);
        }
    } while (count == 200);
    std::reverse(result.begin(), result.end());
    return result;
}

void vk_api::ChatBot::setOnlineStatus(bool isOnline) {
    try {
        std::stringstream param;
        param << "group_id=" << groupId_;
        vk_api::apiRequest(isOnline ? "groups.enableOnline" : "groups.disableOnline", param.str(), accessToken_);
    } catch (vk_api::ApiRequestExpetion e) {
        // Correct status is already set
    }
}
