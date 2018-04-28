#include "VK_API_chat.h"

namespace {
    std::string url_encode (std::string str) {
        std::string result;
        CURL *curl = curl_easy_init();
        if(curl) {
            char *output = curl_easy_escape(curl, str.c_str(), str.size());
            if(output) {
                result.append(output);
                curl_free(output);
            } else {
                throw vk_api::ApiRequestExpetion("Unable to encode string. escape returned NULL.");
            }
        } else {
            throw vk_api::ApiRequestExpetion("Unable to encode string. Unable to init CURL.");
        }
        curl_easy_cleanup(curl);
        return result;
    }
    std::string url_decode (std::string str) {
        std::string result;
        CURL *curl = curl_easy_init();
        if(curl) {
            int l;
            char *output = curl_easy_unescape(curl, str.c_str(), str.size(), &l);
            if(output) {
                result.append(output, l);
                curl_free(output);
            } else {
                throw vk_api::ApiRequestExpetion("Unable to decode string. unescape returned NULL.");
            }
        } else {
            throw vk_api::ApiRequestExpetion("Unable to decode string. Unable to init CURL.");
        }
        curl_easy_cleanup(curl);
        return result;
    }
}


vk_api::Message::Message (nlohmann::json input) :
timestamp(input["date"]), id(input["id"]), senderId(input["out"] == 1 ? 0 : (vkid_t) input["user_id"]),
receiverId(input["out"] == 0 ? 0 : (vkid_t) input["user_id"]), text(input["body"].get<std::string>()), ifRead(input["read_state"] == 1) {}

vk_api::Message::Message (time_t Timestamp, vkid_t Id, vkid_t SenderId, vkid_t ReceiverId, std::string Text, bool IfRead) :
timestamp(Timestamp), id(Id), senderId(SenderId), receiverId(ReceiverId), text(Text), ifRead(IfRead) {}


vk_api::ChatBot::ChatBot (std::string accessToken) : longpoll_(), accessToken_(accessToken) {
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
    param << "user_id=" << receiver << "&message=" << url_encode(text);
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
