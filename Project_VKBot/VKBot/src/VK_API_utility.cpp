#include "VK_API_utility.h"


namespace {
    size_t writeFunction_ (char *recievedData, size_t size, size_t nmemb, std::string *buffer) {
        size_t result = 0;
        if (buffer != nullptr) {
            buffer->append(recievedData, size * nmemb);
            result = size * nmemb;
        }
        return result;
    }

    std::string safeRequest_ (std::string url) {
        CURL *curl; // curl object
        curl = curl_easy_init();
        if (!curl) {
            throw vk_api::ApiRequestExpetion("Failed to create curl object");
        }
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
        if (result != CURLE_OK) {
            throw vk_api::ApiRequestExpetion(errorBuffer.c_str());
        }
        curl_easy_cleanup(curl);
        return outputBuffer;
    }

    #include "utility/encode_decode.cpp"
}


nlohmann::json vk_api::jsonRequest (std::string url) {
    std::string responseString = safeRequest_(url);
    nlohmann::json response;
    try {
        response = nlohmann::json::parse(responseString);
    } catch (...) {
        response = {};
        // TODO Correct handle of parse error
    }
    return response;
}

nlohmann::json vk_api::apiRequest_raw (std::string methodName, std::string parameters, std::string accessToken, std::string version) {
    std::stringstream url;
    url << "https://api.vk.com/method/" << methodName << "?";
    if (parameters != "") {
        url << parameters << "&";
    }
    if (accessToken != "") {
        url << "access_token=" << accessToken << "&";
    }
    url << "v=" << version;
    return jsonRequest(url.str());
}
nlohmann::json vk_api::apiRequest (std::string methodName, std::string parameters, std::string accessToken, std::string version) {
    nlohmann::json response = apiRequest_raw(methodName, parameters, accessToken, version);
    if (response.find("response") == response.end()) {
        throw vk_api::ApiRequestExpetion("API request error. Bad response");
    }
    return response["response"];
}
nlohmann::json vk_api::execute (std::string code, std::string accessToken, std::string version) {
    std::stringstream param;
    param << "code=" << str_encode(code);
    return vk_api::apiRequest_raw("execute", param.str(), accessToken, version);
}
