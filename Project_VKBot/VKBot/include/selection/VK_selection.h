#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED


#include <string> // std::string, std::stringstream
#include <iostream> // std::ostream
#include <cstdio> // std::open, std::read, ...

#include "VK_API_utility.h" // vk_api::apiRequest

namespace vk_selection {

    ///
    /// \brief Class to work with selection of Units
    ///
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


        ///
        /// \brief TRUE if selection is inverted
        ///
        /// \return Return TRUE if selection contains everything except of stated units
        bool isInverted () const;

        ///
        /// \brief Size of selection
        ///
        /// \return Return amount of units in selection
        size_t size () const;


        ///
        /// \brief Save to file "${name}.txt"
        ///
        void saveAs (std::string name);


    private:
        bool isInverted_;
        size_t size_;
        std::string name_;

        Selection (std::string name);
        friend class Unit;

        template <typename F> void inFile (const char* mode, F func) {
            std::FILE *file = std::fopen(name_.c_str(), mode);
            func(file);
            std::fclose(file);
        }

        void updateInfo_ ();
    };


    ///
    /// \brief Id of user or group
    ///
    typedef unsigned long vkid_t;


    ///
    /// \brief Type of Unit
    ///
    enum unitType : char {Undefined = '0', User, Public, Group, Event};
    const std::string unitTypeNames[] = {"Undefined", "User", "Public", "Group", "Event"};

    ///
    /// \brief Class to work with single User, Group, Public or Event
    ///
    class Unit {
    public:
        ///
        /// \brief Create using id
        ///
        Unit (vkid_t id, bool isUser);

        ///
        /// \brief Crete using custom id
        ///
        /// Automatically identifies Unit type
        Unit (std::string id);


        Unit () = delete;
        Unit &operator=(const Unit&) = default;
        Unit (const Unit&) = default;
        Unit &operator=(Unit&&) = default;
        Unit (Unit&&) = default;
        ~Unit () = default;


        ///
        /// \brief Unit type
        ///
        /// \return Unit type
        unitType type() const;

        ///
        /// \brief Unit id
        ///
        /// \return Unit id
        vkid_t id() const;

        ///
        /// \brief Unit custom id
        ///
        /// \return Unit custom id (screen name)
        std::string customId() const;


        ///
        /// \brief Selection of user friends
        ///
        /// \return Selection (10000 max) with friends of user or empty selection
        Selection friends ();

        ///
        /// \brief Selection of user subscribers
        ///
        /// Requires user accessToken
        /// \return Selection with subscribers of user or empty selection
        Selection subscribers (std::string accessToken);

        ///
        /// \brief Selection of group/public/event members
        ///
        /// \return Selection with members or empty selection
        Selection members (std::string accessToken);


    private:
        unitType type_;
        vkid_t id_;
        std::string customId_;

        bool initUser_ (std::string id); ///< Return FALSE if unable to initialize
        bool initOther_ (std::string id); ///< Return FALSE if unable to initialize
    };

    ///
    /// \brief Print type, id and custom id
    ///
    std::ostream &operator<< (std::ostream &os, const Unit &unit);

}

#endif // VK_SELECTION_H_INCLUDED
