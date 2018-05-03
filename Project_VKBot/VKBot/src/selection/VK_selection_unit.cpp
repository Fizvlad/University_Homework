#include "selection/VK_selection.h"

namespace {
    std::string js_userSubscribers (vk_selection::vkid_t id, size_t offset = 0) {
        std::stringstream code;
        code << "var id=" << id << ";var count=1000;var offset=" << offset << ";var requestsLimit=25;var result=[];var first=API.users.getFollowers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.users.getFollowers({\"user_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
        return code.str();
    }

    std::string js_groupMembers (vk_selection::vkid_t id, size_t offset = 0) {
        std::stringstream code;
        code << "var id=" << id << ";var count=1000;var offset=" << offset << ";var requestsLimit=25;var result=[];var first=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
        return code.str();
    }
}

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
    if (userInfo["screen_name"] == nullptr) {
        // Private profile?
        std::stringstream sn;
        sn << "id" << id_;
        customId_ = sn.str();
    } else {
        customId_ = userInfo["screen_name"];
    }
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

vk_selection::Selection vk_selection::Unit::friends() {
    if (type_ != vk_selection::User) {
        throw vk_api::ApiRequestExpetion("Can not request friends for this Unit (not user).");
    }
    std::stringstream param;
    param << "user_id=" << id_ << "&count=10000";
    nlohmann::json response = vk_api::apiRequest("friends.get", param.str())["items"];

    std::stringstream name;
    name << "user_" << id_ << "_friends";
    Selection result(name.str());

    result.inFile_("ab", [response](FILE *file){
        char pre = (char) vk_selection::User; // Forced to first create char pre because there is no way to save enum element into file
        for (vk_selection::vkid_t id : response) {
            fwrite(&pre, sizeof(pre), 1, file);
            fwrite(&id, sizeof(vk_selection::vkid_t), 1, file);
        }
    });

    result.size_ += response.size();
    result.updateInfo_();
    return result;
}
vk_selection::Selection vk_selection::Unit::subscribers(std::string accessToken) {
    // TODO Finish function (only first 25k loaded). ATTENTION: this method does not work with group token
    if (type_ != vk_selection::User) {
        throw vk_api::ApiRequestExpetion("Can not request friends for this Unit (not user).");
    }

    size_t total = 1;
    size_t current = 0;
    nlohmann::json responses = {};
    while (current < total) {
        // NOTICE Result of following command is array of arrays (due to API.execute restrictions)
        nlohmann::json response = vk_api::execute(js_userSubscribers(id_, current), accessToken);
        total = (size_t) response["total"];
        current += (size_t) response["current"];
        responses.push_back(response["items"]);
    }
    std::stringstream name;
    name << "user_" << id_ << "_subscribers";
    Selection result(name.str());

    result.inFile_("ab", [responses](FILE *file){
        char pre = (char) vk_selection::User; // Forced to first create char pre because there is no way to save enum element into file
        for (auto re : responses) {
            for (auto sub : re) {
                for (vk_selection::vkid_t id : sub) {
                    fwrite(&pre, sizeof(pre), 1, file);
                    fwrite(&id, sizeof(vk_selection::vkid_t), 1, file);
                }
            }
        }
    });
    result.size_ = total;
    result.updateInfo_();
    return result;
}
vk_selection::Selection vk_selection::Unit::members(std::string accessToken) {
    if (type_ == vk_selection::User) {
        throw vk_api::ApiRequestExpetion("Can not request members for this Unit (User).");
    }

    size_t total = 1;
    size_t current = 0;
    nlohmann::json responses = {};
    while (current < total) {
        // NOTICE Result of following command is array of arrays (due to API.execute restrictions)
        nlohmann::json response = vk_api::execute(js_groupMembers(id_, current), accessToken);
        total = (size_t) response["total"];
        current += (size_t) response["current"];
        responses.push_back(response["items"]);
    }
    std::stringstream name;
    name << "group_" << id_ << "_members";
    Selection result(name.str());

    result.inFile_("ab", [responses](FILE *file){
        char pre = (char) vk_selection::User; // Forced to first create char pre because there is no way to save enum element into file
        for (auto re : responses) {
            for (auto sub : re) {
                for (vk_selection::vkid_t id : sub) {
                    fwrite(&pre, sizeof(pre), 1, file);
                    fwrite(&id, sizeof(vk_selection::vkid_t), 1, file);
                }
            }
        }
    });
    result.size_ = total;
    result.updateInfo_();
    return result;
}
