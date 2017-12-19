#ifndef VK_API_H_INCLUDED
#define VK_API_H_INCLUDED

#include <string>
#include <exception>

#include "curl/curl.h"
#include "nlohmann/json.hpp"

namespace vk_api
{
    /*
        -------------------
        ---- Exceptions
        -------------------
    */
    class apiRequestExpetion : std::exception
    {
    public:
        explicit apiRequestExpetion (const char* message) : what_(message) {}
        apiRequestExpetion (const apiRequestExpetion &other) : what_(other.what_) {}
        apiRequestExpetion &operator= (const apiRequestExpetion &other)
        {
            what_ = other.what_;
            return *this;
        }
        const char* what () { return what_; }
    private:
        const char* what_;
    };

    /*
        -------------------
        ---- API
        -------------------
    */
    nlohmann::json apiRequest (std::string methodName, std::string parameters = "", std::string accessToken = "", std::string version = "5.69");

    /*
        -------------------
        ---- LongPoll
        -------------------
    */
    namespace longpoll
    {
        namespace MODE
        {
            const uint8_t GET_ATTACHMENTS = 0x02; // 1   2
            const uint8_t EXTENDED_EVENTS = 0x08; // 3   8
            const uint8_t GET_PTS         = 0x20; // 5  32
            const uint8_t GET_EXTRA       = 0x40; // 6  64
            const uint8_t GET_RANDOM_ID   = 0x80; // 7 128
        }

        namespace EVENTS
        {
            namespace MESSAGE
            {
                namespace FLAGS
                {
                    const unsigned short CHANGE = 1;
                    const unsigned short SET    = 2;
                    const unsigned short RESET  = 3;
                }
                const unsigned short NEW    = 4;
                const unsigned short EDITED = 5;

                const unsigned short INPUT_READ    = 6; // Flag of incoming  letter changed to 'read'
                const unsigned short OUTPUT_READ   = 7; // User read our message

                const unsigned short DELETED   = 13;
                const unsigned short RECOVERED = 14;
            }

            namespace USER
            {
                const unsigned short ONLINE  = 8;
                const unsigned short OFFLINE = 9;

                const unsigned short TYPING = 61;
                const unsigned short TYPING_IN_CHAT = 62;
                const unsigned short CALLED = 70;
            }

            namespace DIALOGUE
            {
                namespace FLAGS // Only for group messages
                {
                    const unsigned short RESET  = 10;
                    const unsigned short CHANGE = 11;
                    const unsigned short SET    = 12;
                }
                const unsigned short PARAMETERS_CHANGED = 51;
            }

            namespace OTHER
            {
                const unsigned short COUNTER_CHANGED = 80;
                const unsigned short NOTIFICATIONS_PARAMETERS_CHANGED = 114;
            }
        }

        class session
        {
        public:
            session (uint8_t mode = 0, unsigned short timeout = 25);
            session &operator=(const session&) = delete;
            session (const session&) = delete;
            session &operator=(session&&) = delete;
            session (session&&) = delete;
            ~session ();

            void initialize (std::string accessToken);

            template <typename Func>
            void listen_once (Func f)
            {
                nlohmann::json updates = request_();
                f(updates);
            }
            template <typename Func>
            void listen_n (Func f, unsigned n)
            {
                for (unsigned i = 0; i < n; i++)
                    listen_once(f);
            }
            template <typename Func, typename Predicate>
            void listen_while (Func f, Predicate p)
            {
                while (p())
                    listen_once(f);
            }
            template <typename Func>
            void listen (Func f)
            {
                while (true)
                    listen_once(f);
            }
        private:
            std::string    server_;
            std::string    key_;
            unsigned long  ts_;
            unsigned short mode_;
            unsigned short timeout_;

            nlohmann::json request_ ();
        };
    }
}

#endif // VK_API_H_INCLUDED
