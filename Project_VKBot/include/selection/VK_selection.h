#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED


#include <string>

namespace vk_selection {
    enum unitType {Undefined, User, Public, Group, Event};

    /// Class to work with single User, Group, Public or Event
    class Unit {
    public:
        Unit () = delete;
        Unit (std::string id); ///< Will automatically identify type
        Unit (unitType type, std::string id, std::string customId = "");
        Unit &operator=(const Unit&);
        Unit (const Unit&);
        Unit &operator=(Unit&&);
        Unit (Unit&&);
        ~Unit ();

        // TODO methods
    private:
        unitType type_;
        std::string id_;
        std::string customId_; ///< Can be empty

        void identifyType_();
    };

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
        bool isEveryone_; ///< TRUE if this selection includes every single unit (Functionality is reduced in this case)

        // TODO How to store all the data? (Largest VK public got 5 millions subscribers)
    };
}

#endif // VK_SELECTION_H_INCLUDED
