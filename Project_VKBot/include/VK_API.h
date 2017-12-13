#ifndef VK_API_H_INCLUDED
#define VK_API_H_INCLUDED

#include <string>
#include <exception>

#include <curl/curl.h>


namespace vk_api
{
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

    std:: string apiRequest (std::string methodName, std::string parameters, std::string accessToken = "", std::string version = "5.69");
}

#endif // VK_API_H_INCLUDED
