#include "selection/VK_selection.h"

const char SELECTION_EXTENSION[4] = "vks";

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

vk_selection::Selection vk_selection::Unit::friends() {
    // TODO implementation
}
vk_selection::Selection vk_selection::Unit::subscribers() {
    // TODO implementation
}
vk_selection::Selection vk_selection::Unit::members() {
    // TODO implementation
}


vk_selection::Selection::Selection(std::string name) : isInverted_(false), size_(0), name_("") {
    std::stringstream fileName;
    fileName << name << "." << SELECTION_EXTENSION;
    std::FILE *file = std::fopen(fileName.str().c_str(), "w+");
    std::fprintf(file, "%c%c%u%c", isInverted() ? '1' : '0', '\n', size(), '\n');
    std::fclose(file);
}

vk_selection::Selection::~Selection()
{
    // TODO implementation
}


vk_selection::Selection& vk_selection::Selection::operator&&(const Selection& other) const {
    // TODO implementation
}
vk_selection::Selection &vk_selection::Selection::operator||(const Selection& other) const {
    // TODO implementation
}
vk_selection::Selection &vk_selection::Selection::operator!() {
    // TODO implementation
}

bool vk_selection::Selection::isInverted() const {
    return isInverted_;
}
unsigned long vk_selection::Selection::size() const {
    return size_;
}
