#include "VK_API_longpoll.h"

vk_api::longpoll::Session::Session (uint8_t mode, unsigned short timeout) : server_(""), key_(""), ts_(0), mode_((unsigned short)mode), timeout_(timeout) {}

vk_api::longpoll::Session::~Session () {}

void vk_api::longpoll::Session::initialize (std::string accessToken) {
    nlohmann::json response = apiRequest("messages.getLongPollServer", mode_ & vk_api::longpoll::MODE::GET_PTS ? "need_pts=1" : "need_pts=0", accessToken);
    server_ = response["server"];
    key_    = response["key"];
    ts_     = response["ts"];
}

nlohmann::json vk_api::longpoll::Session::request_ () {
    if (server_ == "") {
        vk_api::ApiRequestExpetion("Uninitialized session");
    }

    std::stringstream url;
    url << "https://" << server_ << "?version=2&act=a_check&key=" << key_ << "&ts=" << std::to_string(ts_) << "&wait=" << std::to_string(timeout_) << "&mode=" << std::to_string((int)mode_);

    nlohmann::json response = vk_api::jsonRequest(url.str());
    if (response.find("failed") != response.end()) {
        throw vk_api::ApiRequestExpetion("API request error. Response failed");
    }
    ts_ = response["ts"];
    return response["updates"];
}
