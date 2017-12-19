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
