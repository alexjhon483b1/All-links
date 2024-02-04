#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback function to write response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    printf("%.*s", (int)realsize, (char *)contents);
    return realsize;
}

// Callback function to write headers
size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    printf("Header: %.*s", (int)realsize, (char *)contents);
    return realsize;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return 1;
    }

    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a curl handle
    curl = curl_easy_init();
    if (curl) {
        // Set the URL using the provided URL argument
        curl_easy_setopt(curl, CURLOPT_URL, argv[1]);

        // Set the callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Set the callback function to handle headers
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);

        // Follow HTTP redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Add custom headers with domain and path details
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:100.0) Gecko/20100101 Firefox/100.0");

        // Extract the host and path from the URL and set them as custom headers
        char *url_copy = strdup(argv[1]);  // Make a copy to avoid modifying the original string
        char *host = strtok(url_copy, "://");
        if (host) {
            char *path = strchr(host, '/');
            if (path) {
                *path = '\0';  // Null-terminate the host
                headers = curl_slist_append(headers, host);
                headers = curl_slist_append(headers, path + 1);  // Skip the leading '/'
            }
        }
        free(url_copy);  // Free the copied string

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request and capture the result
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    // Cleanup libcurl
    curl_global_cleanup();

    return 0;
}

