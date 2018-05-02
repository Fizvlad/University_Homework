#ifndef VK_API_MESSAGES_H_INCLUDED
#define VK_API_MESSAGES_H_INCLUDED


#include <string> // std::string, std::stringstream
#include <ctime> // time_t
#include <vector> // std::vector

#include "curl/curl.h" // url encode/decode

#include "VK_API_longpoll.h"

namespace vk_api {

    ///
    /// \brief Id of user or message
    ///
    typedef unsigned long vkid_t;


    ///
    /// \brief Structure with message data
    ///
    struct Message {
        time_t timestamp;
        vkid_t id;
        vkid_t senderId;
        vkid_t receiverId;
        std::string text;
        bool ifRead;

        Message (time_t Timestamp = 0, vkid_t Id = 0, vkid_t SenderId = 0, vkid_t ReceiverId = 0, std::string Text = "", bool IfRead = true);

        ///
        /// \brief Parsing message from json
        ///
        Message (nlohmann::json input);
    };


    ///
    /// \brief Working with chats and messages
    ///
    class ChatBot {
    public:
        ///
        /// \brief Saving accessToken and setting up longpoll session
        ///
        ChatBot (std::string accessToken);


        ChatBot () = delete;
        ChatBot &operator=(const ChatBot&) = delete;
        ChatBot (const ChatBot&) = delete;
        ChatBot &operator=(ChatBot&&) = delete;
        ChatBot (ChatBot&&) = delete;
        ~ChatBot ();


        ///
        /// \brief Handle last unread message in every dialogue and starts longpoll
        ///
        /// \param handler bool (function*) (Message message, bool isOld) to handle messages
        template <typename MessageHandlerFunc> void start (MessageHandlerFunc handler) {
            std::vector<Message> unread = getUnreadMessages_();
            for (auto message : unread) {
                if (!handler(message, true)) {
                    return;
                }
            }

            longpoll_.listen([&] (nlohmann::json upd) {
                for (auto i : upd) {
                    if ((unsigned)i[0] == longpoll::EVENTS::MESSAGE::NEW && !((unsigned)i[2] & longpoll::MESSAGE_FLAGS::OUTBOX)) {
                        Message message(i[4], i[1], i[3], 0, i[5]);
                        if (!handler(message, false)) {
                            return false;
                        }
                    }
                }
                return true;
             });
        }

        ///
        /// \brief Mark message as read
        ///
        /// \param message Message structure
        void markAsRead (Message message);

        ///
        /// \brief Send message to user
        ///
        /// \param receiver Id of user
        /// \param text Text of message
        void sendMessage (vkid_t receiver, std::string text);


    private:
        longpoll::Session longpoll_;
        std::string accessToken_;

        std::vector<Message> getUnreadMessages_();
    };

}

#endif // VK_API_MESSAGES_H_INCLUDED
