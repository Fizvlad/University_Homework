#ifndef VK_API_H_INCLUDED
#define VK_API_H_INCLUDED

#include <string>
#include <exception>
#include <ctime>
#include <vector>
#include <sstream>

#include "curl/curl.h"
#include "nlohmann/json.hpp"

namespace vk_api
{
    /*
        -------------------
        ---- Custom types
        -------------------
    */
    /// Id user or message
    typedef unsigned long vkid_t;

    /*
        -------------------
        ---- Exceptions
        -------------------
    */
    /// VK_API exception class
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
    ///
    /// \brief Send HTTPS request for given method
    ///
    /// \param methodName Name of the method
    /// \param parameters Query string
    /// \param accessToken VK access token (might be not necessary)
    /// \param version API version
    /// \return Request response in JSON
    ///
    nlohmann::json apiRequest (std::string methodName, std::string parameters = "", std::string accessToken = "", std::string version = "5.69");


    /*
        -------------------
        ---- LongPoll
        -------------------
    */
    /// Working with longpoll requests
    namespace longpoll
    {
        /// Longpoll request mode
        namespace MODE
        {
            const uint8_t GET_ATTACHMENTS = 0x02; // 1   2
            const uint8_t EXTENDED_EVENTS = 0x08; // 3   8
            const uint8_t GET_PTS         = 0x20; // 5  32
            const uint8_t GET_EXTRA       = 0x40; // 6  64
            const uint8_t GET_RANDOM_ID   = 0x80; // 7 128
        }

        /// Id of events which can be received with longpoll
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

        /// Interface for work with longpoll
        class session
        {
        public:
            session (uint8_t mode = 0, unsigned short timeout = 25);
            session &operator=(const session&) = delete;
            session (const session&) = delete;
            session &operator=(session&&) = delete;
            session (session&&) = delete;
            ~session ();

            void initialize (std::string accessToken); ///< Setting up access token. Necessary for requests

            /// Single longpoll request
            template <typename Func>
            bool listen_once (Func f)
            {
                nlohmann::json updates = request_();
                return f(updates);
            }
            /// N of logpoll requests. Stop listening if handler return FALSE
            template <typename Func>
            bool listen_n (Func f, unsigned n)
            {
                for (unsigned i = 0; i < n; i++)
                    if (!listen_once(f))
                        return false;
            }
            /// Listening while predicate returns TRUE. Stop listening if handler return FALSE
            template <typename Func, typename Predicate>
            bool listen_while (Func f, Predicate p)
            {
                while (p())
                    if (!listen_once(f))
                        return false;
            }
            /// Listen while handler return TRUE
            template <typename Func>
            bool listen (Func f)
            {
                while (true)
                    if (!listen_once(f))
                        return false;
            }
        private:
            std::string    server_;
            std::string    key_;
            time_t         ts_;
            unsigned short mode_;
            unsigned short timeout_;

            nlohmann::json request_ ();
        };
    }


    /*
        -------------------
        ---- Working with chat
        -------------------
    */
    namespace chat
    {
        /// Message type. Can be created by
        class message
        {
        public:
            message () = delete;
            message &operator=(const message&) = default;
            message (const message&) = default;
            message &operator=(message&&) = default;
            message (message&&) = default;
            ~message () = default;

            time_t getTimestamp ();
            vkid_t getId ();
            vkid_t getUserId ();
            std::string getText();

            void markAsRead (std::string accessToken);
        private:
            time_t ts_;
            vkid_t id_;
            vkid_t user_;
            std::string text_;

            message (nlohmann::json input);

            friend std::vector<message> recieveNewMessages (std::string accessToken, unsigned amount);
        };

        /// Get vector with new messages
        std::vector<message> recieveNewMessages (std::string accessToken, unsigned amount = 200);
    }
}

#endif // VK_API_H_INCLUDED
