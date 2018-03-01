#include "selection/VK_selection.h"


vk_selection::unitType vk_selection::Unit::type() const {
    return type_;
}
vk_selection::vkid_t vk_selection::Unit::id() const {
    return id_;
}
std::string vk_selection::Unit::customId() const {
    return customId_;
}

vk_selection::Unit::Unit (std::string id) : type_(Undefined), id_(0), customId_("") {
    if (!initUser_(id) && !initOther_(id)) {
        throw vk_api::ApiRequestExpetion("Unable to initialize Unit. Probably wrong id");
    }
}
vk_selection::Unit::Unit (vk_selection::vkid_t id, bool isUser) : type_(Undefined), id_(0), customId_("") {
    bool ifInitialized = false;
    if (isUser) {
        ifInitialized = initUser_(std::to_string(id));
    } else {
        ifInitialized = initOther_(std::to_string(id));
    }

    if (!ifInitialized) {
        throw vk_api::ApiRequestExpetion("Unable to initialize Unit. Wrong id or isUser");
    }
}

bool vk_selection::Unit::initUser_(std::string id) {
    std::stringstream param;
    param << "fields=screen_name&user_ids=" << id;
    nlohmann::json userInfo;
    try {
        userInfo = vk_api::apiRequest("users.get", param.str())[0];
    } catch (vk_api::ApiRequestExpetion e) {
        return false;
    }
    type_ = User;
    id_ = userInfo["id"];
    customId_ = userInfo["screen_name"];
    return true;
}
bool vk_selection::Unit::initOther_(std::string id) {
    std::stringstream param;
    param << "group_ids=" << id;
    nlohmann::json groupInfo;
    try {
        groupInfo = vk_api::apiRequest("groups.getById", param.str())[0];
    } catch (vk_api::ApiRequestExpetion e) {
        return false;
    }
    std::string type = groupInfo["type"];
    if (type == "page") {
        type_ = Public;
    } else if (type == "group") {
        type_ = Group;
    } else if (type == "event") {
        type_ = Event;
    } else {
        std::stringstream e;
        e << "Unknown unit type: " << type;
        throw vk_api::ApiRequestExpetion(e.str().c_str());
    }
    id_ = groupInfo["id"];
    customId_ = groupInfo["screen_name"];
    return true;
}

std::ostream &vk_selection::operator<< (std::ostream &os, const vk_selection::Unit &unit) {
    return os << unit.type() << " " << unit.id() << " " << unit.customId();
}
