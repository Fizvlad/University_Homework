#ifndef VK_API_UTILITY_H_INCLUDED
#define VK_API_UTILITY_H_INCLUDED

#include <string> // std::string, std::stringstream
#include <exception> // std::exception

#include "curl/curl.h" // curl
#include "nlohmann/json.hpp" // json


namespace vk_api {
    /// VK_API exception class
    class ApiRequestExpetion : std::exception {
    public:
        explicit ApiRequestExpetion (const char* message) : what_(message) {}
        ApiRequestExpetion (const ApiRequestExpetion &other) : what_(other.what_) {}
        ApiRequestExpetion &operator= (const ApiRequestExpetion &other) {
            what_ = other.what_;
            return *this;
        }
        const char* what () const noexcept {
            return what_;
        }
    private:
        const char* what_;
    };

    ///
    /// \brief Send HTTPS request for given url
    ///
    /// \param url URL of page
    /// \return Parsed json
    ///
    nlohmann::json jsonRequest (std::string url);

    ///
    /// \brief Send HTTPS request for given method
    ///
    /// \param methodName Name of the method
    /// \param parameters Query string
    /// \param accessToken VK access token (might be not necessary)
    /// \param version API version
    /// \return "response" value of recieved json
    ///
    nlohmann::json apiRequest (std::string methodName, std::string parameters, std::string accessToken = "", std::string version = "5.69");

    ///
    /// \brief Executes given JS code on VK server
    ///
    /// Please take a look at https://vk.com/dev/execute before use.
    ///
    /// \param code string of code
    /// \param accessToken VK access token (might be not necessary)
    /// \param version API version
    /// \return "response" value of recieved json
    ///
    nlohmann::json execute (std::string code, std::string accessToken, std::string version = "5.69");
}

#endif // VK_API_UTILITY_H_INCLUDED
