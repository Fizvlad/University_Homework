#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED


#include <string> // std::string
#include <iostream> // std::ostream
#include <sstream> // std::stringstream

#include "VK_API_utility.h" // vk_api::apiRequest

namespace vk_selection {
    /// Id of user or group
    typedef unsigned long vkid_t;

    enum unitType {Undefined, User, Public, Group, Event};

    /// Class to work with single User, Group, Public or Event
    class Unit {
    public:
        Unit () = delete;
        Unit (vkid_t id, bool isUser);
        Unit (std::string id); ///< Will automatically identify type
        Unit &operator=(const Unit&) = default;
        Unit (const Unit&) = default;
        Unit &operator=(Unit&&) = default;
        Unit (Unit&&) = default;
        ~Unit () = default;

        /// \return Unit type
        unitType type() const;
        /// \return Unit id
        vkid_t id() const;
        /// \return Unit custom id (screen name)
        std::string customId() const;

        // TODO methods
    private:
        unitType type_;
        vkid_t id_;
        std::string customId_; ///< Can be empty

        bool initUser_(std::string id); ///< Return FALSE if unable to initialize
        bool initOther_(std::string id); ///< Return FALSE if unable to initialize
    };

    /// Print type, id and custom id
    std::ostream &operator<< (std::ostream &os, const Unit &unit);

    /// Class to work with selection of Units
    class Selection {
    public:
        Selection (std::string str = "");
        Selection &operator=(const Selection&);
        Selection (const Selection&);
        Selection &operator=(Selection&&);
        Selection (Selection&&);
        ~Selection ();

        // TODO methods
    private:
        bool isEverything_; ///< TRUE if this selection includes every single unit (Functionality is reduced in this case)

        // TODO How to store all the data? (Largest VK public got 5 millions subscribers)
    };
}

#endif // VK_SELECTION_H_INCLUDED
