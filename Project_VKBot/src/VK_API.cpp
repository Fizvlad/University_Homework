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

    return nlohmann::json::parse(safeRequest_(url));
}

/*
    -------------------
    ---- LongPoll
    -------------------
*/
