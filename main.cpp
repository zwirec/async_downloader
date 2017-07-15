#include <curl/curl.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <iostream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *stream) {
    size_t realsize = size * nmemb;
    std::string data((const char*) contents, realsize);
    *((std::stringstream*) stream) << data << std::endl;
    return realsize;
}


static void *pull_one_url(void *url) {
    CURL *curl;

    std::stringstream out;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &out);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << long(out.tellp()) << " bytes received from " << (char *) url << std::endl;
        //std::cout << out.str() << std::endl;
    }
    curl_easy_cleanup(curl);
    return NULL;
}


int main(int argc, char **argv) {
    size_t num_urls = (size_t) (argc - 1);

    std::vector<std::string> urls;

    for (int i = 0; i < argc - 1; i++) {
        urls.push_back(std::move(argv[i + 1]));
    }

    std::vector<std::thread> pool_thread;

    curl_global_init(CURL_GLOBAL_ALL);

    for (int i = 0; i < num_urls; i++) {
        pool_thread.emplace_back(pull_one_url, (void*)urls[i].c_str());
    }

    for (auto &thr:pool_thread) {
        thr.join();
    }

    return 0;
}