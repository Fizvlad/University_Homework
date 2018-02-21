#ifndef VK_API_UTILITY_H_INCLUDED
#define VK_API_UTILITY_H_INCLUDED

#include <string> // std::string
#include <exception> // std::exception
#include <sstream> // std::stringstream

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
        const char* what () {
            return what_;
        }
    private:
        const char* what_;
    };

    ///
    /// \brief Send HTTPS request for given url
    ///
    /// \param url URL of page
    /// \return Request response in JSON
    ///
    nlohmann::json apiRequest (std::string url);

    ///
    /// \brief Send HTTPS request for given method
    ///
    /// \param methodName Name of the method
    /// \param parameters Query string
    /// \param accessToken VK access token (might be not necessary)
    /// \param version API version
    /// \return Request response in JSON
    ///
    nlohmann::json apiRequest (std::string methodName, std::string parameters, std::string accessToken = "", std::string version = "5.69");
}

#endif // VK_API_UTILITY_H_INCLUDED
