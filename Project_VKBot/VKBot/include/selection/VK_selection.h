#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED


#include <string> // std::string, std::stringstream
#include <iostream> // std::ostream
#include <cstdio> // std::open, std::read, ...

#include "VK_API_utility.h" // vk_api::apiRequest

namespace vk_selection {

    /// Class to work with selection of Units.
    /// Each Selection have its own file in "${name_}.vks" which contains ids.
    class Selection {
    public:

        Selection() = delete;
        Selection&operator=(const Selection&) = delete;
        Selection(const Selection&) = delete;
        Selection&operator=(Selection&&) = default;
        Selection(Selection&&) = default;

        ~Selection();


        Selection operator&&(const Selection &other) const;
        Selection operator||(const Selection &other) const;
        Selection operator!();


        /// TRUE if selection is inverted.
        ///
        /// \return Return TRUE if selection contains everything except of stated units.
        bool isInverted() const;


        /// Size of selection.
        ///
        /// \return Return amount of units in selection.
        size_t size() const;


        /// Save to file "${name}.txt".
        void saveAs(std::string name);


    private:

        bool isInverted_;
        size_t size_;
        std::string name_;


        Selection(const Selection& other, std::string name);
        Selection(std::string name);


        friend class Unit;


        template <typename F>
        void inFile_(const char* mode, F func) {
            std::FILE *file = std::fopen(name_.c_str(), mode);
            func(file);
            std::fclose(file);
        }

        template <typename F>
        static void inTwoFiles_(const vk_selection::Selection &selection1, const char* mode1, const vk_selection::Selection &selection2, const char* mode2, F func) {
            std::FILE *file1 = std::fopen(selection1.name_.c_str(), mode1);
            std::FILE *file2 = std::fopen(selection2.name_.c_str(), mode2);
            func(file1, file2);
            std::fclose(file1);
            std::fclose(file2);
        }

        void updateInfo_();
    };


    /// Id of user or group.
    typedef unsigned long vkid_t;


    /// Type of Unit.
    enum unitType : char {Undefined = '0', User, Public, Group, Event};

    /// Unit types.
    const std::string unitTypeNames[] = {"Undefined", "User", "Public", "Group", "Event"};


    /// Class to work with single User, Group, Public or Event.
    class Unit {
    public:

        /// Create using id.
        Unit (vkid_t id, bool isUser);

        /// Create using custom id.
        /// Automatically identifies Unit type.
        Unit (std::string id);


        Unit () = delete;

        Unit &operator=(const Unit&) = default;
        Unit (const Unit&) = default;
        Unit &operator=(Unit&&) = default;
        Unit (Unit&&) = default;
        ~Unit () = default;


        /// Unit type.
        ///
        /// \return Unit type.
        unitType type() const;


        /// Unit id.
        ///
        /// \return Unit id.
        vkid_t id() const;


        /// Unit custom id.
        ///
        /// \return Unit custom id (screen name).
        std::string customId() const;


        /// Selection of user friends.
        ///
        /// \return Selection (10000 max) with friends of user or empty selection.
        Selection friends ();


        /// Selection of user subscribers.
        /// Notice: Requires user accessToken.
        ///
        /// \return Selection with subscribers of user or empty selection.
        Selection subscribers (std::string accessToken);


        /// Selection of group/public/event members.
        ///
        /// \return Selection with members or empty selection.
        Selection members (std::string accessToken);


    private:

        unitType type_;
        vkid_t id_;
        std::string customId_;

        /// Return FALSE if unable to initialize.
        bool initUser_ (std::string id);

        ///Return FALSE if unable to initialize.
        bool initOther_ (std::string id);
    };


    /// Print type, id and custom id.
    std::ostream &operator<< (std::ostream &os, const Unit &unit);


    bool operator<(const Unit &l, const Unit &r);

}

#endif // VK_SELECTION_H_INCLUDED
