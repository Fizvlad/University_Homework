std::string str_encode (std::string str) {
    std::string result;
    CURL *curl = curl_easy_init();
    if(curl) {
        char *output = curl_easy_escape(curl, str.c_str(), str.size());
        if(output) {
            result.append(output);
            curl_free(output);
        } else {
            throw vk_api::ApiRequestExpetion("Unable to encode string. escape returned NULL.");
        }
    } else {
        throw vk_api::ApiRequestExpetion("Unable to encode string. Unable to init CURL.");
    }
    curl_easy_cleanup(curl);
    return result;
}
std::string str_decode (std::string str) {
    std::string result;
    CURL *curl = curl_easy_init();
    if(curl) {
        int l;
        char *output = curl_easy_unescape(curl, str.c_str(), str.size(), &l);
        if(output) {
            result.append(output, l);
            curl_free(output);
        } else {
            throw vk_api::ApiRequestExpetion("Unable to decode string. unescape returned NULL.");
        }
    } else {
        throw vk_api::ApiRequestExpetion("Unable to decode string. Unable to init CURL.");
    }
    curl_easy_cleanup(curl);
    return result;
}
