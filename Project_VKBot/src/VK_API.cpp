#include "VK_API.h"

/*
    -------------------
    ---- Utility
    -------------------
*/
namespace
{
    // Unnamed namespace (ironic.) to hide some utility functions or classes
    size_t writeFunction_ (char *recievedData, size_t size, size_t nmemb, std::string *buffer)
    {
        size_t result = 0;
        if (buffer != nullptr) {
            buffer->append(recievedData, size * nmemb);
            result = size * nmemb;
        }
        return result;
    }

    std::string safeRequest_ (std::string url)
    {
        CURL *curl; // curl object
        curl = curl_easy_init();
        if (!curl)
            throw vk_api::apiRequestExpetion("Failed to create curl object");
        // Created curl

        std::string errorBuffer;
        std::string outputBuffer;

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); // Don't show header
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // Do redirect
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0); // Don't hide body
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // Use SSL
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputBuffer); // Buffer for callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction_); // Callback
        // Ready to go
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK)
            throw vk_api::apiRequestExpetion(errorBuffer.c_str());
        return outputBuffer;
    }
}

/*
    -------------------
    ---- API
    -------------------
*/
nlohmann::json vk_api::apiRequest (std::string methodName, std::string parameters, std::string accessToken, std::string version)
{
    std::string url = "https://api.vk.com/method/";
    url += methodName;
    url += "?";
    if (parameters != "") {
        url += parameters;
        url += "&";
    }
    if (accessToken != "") {
        url += "access_token=";
        url += accessToken;
        url += "&";
    }
    url += "v=";
    url += version;

    nlohmann::json response = nlohmann::json::parse(safeRequest_(url));
    if (response.find("failed") != response.end())
        throw vk_api::apiRequestExpetion("API request error");

    return response["response"];
}

/*
    -------------------
    ---- LongPoll
    -------------------
*/
vk_api::longpoll::session::session (uint8_t mode, unsigned short timeout) : server_(""), key_(""), ts_(0), mode_((unsigned short)mode), timeout_(timeout)
{}
vk_api::longpoll::session::~session ()
{}
void vk_api::longpoll::session::initialize (std::string accessToken)
{
    nlohmann::json response = apiRequest("messages.getLongPollServer", mode_ & vk_api::longpoll::MODE::GET_PTS ? "need_pts=1" : "need_pts=0", accessToken);
    server_ = response["server"];
    key_    = response["key"];
    ts_     = response["ts"];
}
nlohmann::json vk_api::longpoll::session::request_ ()
{
    if (server_ == "")
        vk_api::apiRequestExpetion("Uninitialized session");
    std::string url = "https://";
    url += server_;
    url += "?version=2&act=a_check&key=";
    url += key_;
    url += "&ts=";
    url += std::to_string(ts_);
    url += "&wait=";
    url += std::to_string(timeout_);
    url += "&mode=";
    url += std::to_string((int)mode_);

    nlohmann::json response = nlohmann::json::parse(safeRequest_(url));
    ts_ = response["ts"];
    return response["updates"];
}
