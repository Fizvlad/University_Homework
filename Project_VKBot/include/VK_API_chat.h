#ifndef VK_API_MESSAGES_H_INCLUDED
#define VK_API_MESSAGES_H_INCLUDED


#include <string> // std::string
#include <ctime> // time_t
#include <vector> // std::vector

#include "VK_API_longpoll.h"

namespace vk_api {
    /// Id of user or message
    typedef unsigned long vkid_t;

    /// Structure with message data
    class Message {
    public:
        time_t timestamp;
        vkid_t id;
        vkid_t senderId;
        vkid_t receiverId;
        std::string text;
        bool ifRead;

        Message (time_t Timestamp = 0, vkid_t Id = 0, vkid_t SenderId = 0, vkid_t ReceiverId = 0, std::string Text = "", bool IfRead = true);
        Message (nlohmann::json input); ///< Parsing message from json
    };

    /// Working with chats and messages
    class ChatBot {
    public:
        ChatBot (std::string accessToken);

        ChatBot () = delete;
        ChatBot &operator=(const ChatBot&) = delete;
        ChatBot (const ChatBot&) = delete;
        ChatBot &operator=(ChatBot&&) = delete;
        ChatBot (ChatBot&&) = delete;
        ~ChatBot ();

        template <typename MessageHandlerFunc> void start (MessageHandlerFunc handler) {
            std::vector<Message> unread = getUnreadMessages_();
            for (auto message : unread) {
                handler(message);
            }

            longpoll_.listen([&] (nlohmann::json upd) {
                for (auto i : upd) {
                    if ((unsigned short)i[0] == longpoll::EVENTS::MESSAGE::NEW) {
                        Message m(i[4], i[1], i[3], 0, i[5]);
                        if (!handler(m)) {
                            return false;
                        }
                    }
                }
                return true;
             });
        }

        void markAsRead (vkid_t userId);
        void sendMessage (vkid_t receiver, std::string text);
    private:
        longpoll::Session longpoll_;

        std::vector<Message> getUnreadMessages_();
    };
}

#endif // VK_API_MESSAGES_H_INCLUDED
