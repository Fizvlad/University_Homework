#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED


#include <string> // std::string, std::stringstream
#include <iostream> // std::ostream
#include <cstdio> // std::open, std::read, ...

#include "VK_API_utility.h" // vk_api::apiRequest

namespace vk_selection {
    /// Class to work with selection of Units
    /// Each Selection have its own file in "${name_}.vks" which contains ids
    class Selection {
    public:
        Selection () = delete;
        Selection &operator=(const Selection&) = delete;
        Selection (const Selection&) = delete;
        Selection &operator=(Selection&&) = default;
        Selection (Selection&&) = default;
        ~Selection ();

        Selection &operator&& (const Selection &other) const;
        Selection &operator|| (const Selection &other) const;
        Selection &operator! ();

        /// \return Return TRUE if selection contains everything except of stated units
        bool isInverted () const;
        /// \return Return amount of units in selection
        unsigned long size () const;

        /// Save to file "${name}.txt"
        void saveAs (std::string name);
    private:
        bool isInverted_; ///< TRUE if this selection includes every single unit except of contained (Functionality is reduced in this case)
        unsigned long size_;
        std::string name_; ///< Name of file with stored info

        Selection (std::string name);
        friend class Unit;

        template <typename F> void inFile (const char* mode, F func) {
            std::FILE *file = std::fopen(name_.c_str(), mode);
            func(file);
            std::fclose(file);
        }

        void updateInfo_ ();
    };


    /// Id of user or group
    typedef unsigned long vkid_t;

    enum unitType : char {Undefined = '0', User, Public, Group, Event};
    const std::string unitTypeNames[] = {"Undefined", "User", "Public", "Group", "Event"};

    /// Class to work with single User, Group, Public or Event
    /// Weights up to 40b
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

        /// \return Selection (10000 max) with friends of user or empty selection
        Selection friends ();
        /// \return Selection with subscribers of user or public or empty selection
        Selection subscribers ();
        /// \return Selection with members of group or empty selection
        Selection members ();
    private:
        unitType type_;
        vkid_t id_;
        std::string customId_; ///< Can be empty

        bool initUser_ (std::string id); ///< Return FALSE if unable to initialize
        bool initOther_ (std::string id); ///< Return FALSE if unable to initialize
    };

    /// Print type, id and custom id
    std::ostream &operator<< (std::ostream &os, const Unit &unit);
}

#endif // VK_SELECTION_H_INCLUDED
