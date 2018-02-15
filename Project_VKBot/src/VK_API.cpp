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
            throw vk_api::ApiRequestExpetion("Failed to create curl object");
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
            throw vk_api::ApiRequestExpetion(errorBuffer.c_str());
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
    std::stringstream url;
    url << "https://api.vk.com/method/" << methodName << "?";
    if (parameters != "")
        url << parameters << "&";
    if (accessToken != "") {
        url << "access_token=" << accessToken << "&";
    }
    url << "v=" << version;

    std::string responseString = safeRequest_(url.str());
    nlohmann::json response;
    try {
        response = nlohmann::json::parse(responseString);
    } catch (...) {
        std::cout << responseString << std::endl;
    }
    if (response.find("response") == response.end())
        throw vk_api::ApiRequestExpetion("API request error");
    return response["response"];
}


/*
    -------------------
    ---- LongPoll
    -------------------
*/
vk_api::longpoll::Session::Session (uint8_t mode, unsigned short timeout) : server_(""), key_(""), ts_(0), mode_((unsigned short)mode), timeout_(timeout)
{}

vk_api::longpoll::Session::~Session ()
{}

void vk_api::longpoll::Session::initialize (std::string accessToken)
{
    nlohmann::json response = apiRequest("messages.getLongPollServer", mode_ & vk_api::longpoll::MODE::GET_PTS ? "need_pts=1" : "need_pts=0", accessToken);
    server_ = response["server"];
    key_    = response["key"];
    ts_     = response["ts"];
}

nlohmann::json vk_api::longpoll::Session::request_ ()
{
    if (server_ == "")
        vk_api::ApiRequestExpetion("Uninitialized session");

    std::stringstream url;
    url << "https://" << server_ << "?version=2&act=a_check&key=" << key_ << "&ts=" << std::to_string(ts_) << "&wait=" << std::to_string(timeout_) << "&mode=" << std::to_string((int)mode_);

    nlohmann::json response = nlohmann::json::parse(safeRequest_(url.str()));
    if (response.find("failed") != response.end())
        throw vk_api::ApiRequestExpetion("API request error");
    ts_ = response["ts"];
    return response["updates"];
}


/*
    -------------------
    ---- Working with chat
    -------------------
*/
