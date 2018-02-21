#ifndef VK_API_MESSAGES_H_INCLUDED
#define VK_API_MESSAGES_H_INCLUDED


#include <string> // std::string
#include <ctime> // time_t
#include <vector> // std::vector

#include "VK_API_longpoll.h"

namespace vk_api {
    /// Id of user or message
    typedef unsigned long vkid_t;


    /// Working with chats and messages
    namespace messages
    {
        /// Real message class
        class Message {
        public:
            Message () = delete; ///< Constructor will only be accessible from some functions
            Message &operator=(const Message&) = default;
            Message (const Message&) = default;
            Message &operator=(Message&&) = default;
            Message (Message&&) = default;
            ~Message () = default;

            time_t getTimestamp ();
            vkid_t getId ();
            vkid_t getSenderId ();
            std::string getText();
            bool getReadStatus();
        private:
            const time_t ts_;
            const vkid_t id_; ///< Message id
            const vkid_t sender_; ///< Sender id
            const vkid_t receiver_; ///< Receiver id
            const std::string text_;
            const bool ifRead_; ///< Read state

            Message (nlohmann::json input); ///< Parsing message from json
        };

        std::vector<Message> getUnreadMessages(size_t amount);

        /// Dialogue class
        class Dialogue {
        public:
            Dialogue () = delete; ///< Constructor will only be accessible from some functions
            Dialogue &operator=(const Dialogue&) = default;
            Dialogue (const Dialogue&) = default;
            Dialogue &operator=(Dialogue&&) = default;
            Dialogue (Dialogue&&) = default;
            ~Dialogue () = default;

            vkid_t getId ();
            bool getReadStatus();
        private:
            vkid_t id_; ///< Id of user
        };
    }
}

#endif // VK_API_MESSAGES_H_INCLUDED
