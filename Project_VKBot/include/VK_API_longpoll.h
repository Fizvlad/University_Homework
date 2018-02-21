#ifndef VK_API_LONGPOLL_H_INCLUDED
#define VK_API_LONGPOLL_H_INCLUDED

#include <string> // std::string
#include <ctime> // time_t
#include <sstream> // std::stringstream

#include "nlohmann/json.hpp" // json

#include "VK_API_utility.h"


namespace vk_api {
    /// Working with longpoll requests
    namespace longpoll {
        /// Longpoll request mode
        namespace MODE {
            const uint8_t GET_ATTACHMENTS = 0x02; // 1   2
            const uint8_t EXTENDED_EVENTS = 0x08; // 3   8
            const uint8_t GET_PTS         = 0x20; // 5  32
            const uint8_t GET_EXTRA       = 0x40; // 6  64
            const uint8_t GET_RANDOM_ID   = 0x80; // 7 128
        }


        /// Id of events which can be received with longpoll
        namespace EVENTS {
            namespace MESSAGE {
                namespace FLAGS {
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

            namespace USER {
                const unsigned short ONLINE  = 8;
                const unsigned short OFFLINE = 9;

                const unsigned short TYPING = 61;
                const unsigned short TYPING_IN_CHAT = 62;
                const unsigned short CALLED = 70;
            }

            namespace DIALOGUE {
                namespace FLAGS // Only for group messages
                {
                    const unsigned short RESET  = 10;
                    const unsigned short CHANGE = 11;
                    const unsigned short SET    = 12;
                }
                const unsigned short PARAMETERS_CHANGED = 51;
            }

            namespace OTHER {
                const unsigned short COUNTER_CHANGED = 80;
                const unsigned short NOTIFICATIONS_PARAMETERS_CHANGED = 114;
            }
        }


        /// Interface for work with longpoll
        class Session {
        public:
            Session (uint8_t mode = 0, unsigned short timeout = 25);
            Session &operator=(const Session&) = delete;
            Session (const Session&) = delete;
            Session &operator=(Session&&) = delete;
            Session (Session&&) = delete;
            ~Session ();

            void initialize (std::string accessToken); ///< Setting up access token. Necessary for requests

            /// Single longpoll request
            template <typename Func>
            bool listen_once (Func f) {
                nlohmann::json updates = request_();
                return f(updates);
            }

            /// N of logpoll requests. Stop listening if handler return FALSE
            template <typename Func>
            bool listen_n (Func f, unsigned n) {
                for (unsigned i = 0; i < n; i++)
                    if (!listen_once(f))
                        return false;
            }

            /// Listening while predicate returns TRUE. Stop listening if handler return FALSE
            template <typename Func, typename Predicate>
            bool listen_while (Func f, Predicate p) {
                while (p())
                    if (!listen_once(f))
                        return false;
            }

            /// Listen while handler return TRUE
            template <typename Func>
            bool listen (Func f) {
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
}

#endif // VK_API_LONGPOLL_H_INCLUDED
