#ifndef VK_API_LONGPOLL_H_INCLUDED
#define VK_API_LONGPOLL_H_INCLUDED

#include <string> // std::string, std::stringstream
#include <ctime> // time_t

#include "nlohmann/json.hpp" // json

#include "VK_API_utility.h"


namespace vk_api {
    /// Working with longpoll requests
    namespace longpoll {
        /// Longpoll request mode
        namespace MODE {
            const unsigned GET_ATTACHMENTS = 2; // 1   2
            const unsigned EXTENDED_EVENTS = 8; // 3   8
            const unsigned GET_PTS         = 32; // 5  32
            const unsigned GET_EXTRA       = 64; // 6  64
            const unsigned GET_RANDOM_ID   = 128; // 7 128
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


        /// Message flags
        namespace MESSAGE_FLAGS {
            const unsigned UNREAD = 1;
            const unsigned OUTBOX = 2;
            const unsigned REPLIED = 4;
            const unsigned IMPORTANT = 8;
            const unsigned CHAT = 16; // DEPRICATED
            const unsigned FRIENDS = 32;
            const unsigned SPAM = 64;
            const unsigned DELETED = 128;
            const unsigned FIXED = 256; // DEPRICATED
            const unsigned MEDIA = 512; // DEPRICATED
            const unsigned HIDDEN = 65536;
            const unsigned DELETED_FOR_ALL = 131072;
        }

        /// Interface for work with longpoll
        class Session {
        public:
            Session (unsigned mode = 0, unsigned short timeout = 25);
            Session &operator=(const Session&) = delete;
            Session (const Session&) = delete;
            Session &operator=(Session&&) = delete;
            Session (Session&&) = delete;
            ~Session ();

            void initialize (std::string accessToken); ///< Setting up access token. Necessary for requests

            /// Single longpoll request
            /// \param f Events handler
            /// \return Result of  f(updates)
            template <typename Func>
            bool listen_once (Func f) {
                nlohmann::json updates = request_();
                return f(updates);
            }

            /// N of logpoll requests. Stop listening if handler return FALSE
            /// \param f Events handler
            /// \return FALSE if listening was stopped because handler returned FALSE, TRUE otherwise
            template <typename Func>
            bool listen_n (Func f, unsigned n) {
                for (unsigned i = 0; i < n; i++) {
                    if (!listen_once(f)) {
                        return false;
                    }
                }
                return true;
            }

            /// Listening while predicate returns TRUE. Stop listening if handler return FALSE
            /// \param f Events handler
            /// \return FALSE if listening was stopped because handler returned FALSE, TRUE otherwise
            template <typename Func, typename Predicate>
            bool listen_while (Func f, Predicate p) {
                while (p()) {
                    if (!listen_once(f)) {
                        return false;
                    }
                }
                return true;
            }

            /// Listen while handler return TRUE
            /// \param f Events handler
            /// \return FALSE if listening was stopped because handler returned FALSE, TRUE otherwise
            template <typename Func>
            bool listen (Func f) {
                while (true) {
                    if (!listen_once(f)) {
                        return false;
                    }
                }
                return true;
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
