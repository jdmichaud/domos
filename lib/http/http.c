#include "http.h"
#include <string.h>
#include <curl/curl.h>

#define OK 0
#define INIT_ALREADY_DONE 1
#define HTTPS_NOT_AVAILABLE 2
#define FAIL

#define MAX_BODY_SIZE 1024

static CURL *g_curl = NULL;
static CURLcode g_last_error = 0;

// To avoid a stupid warning
size_t strnlen(const char *s, size_t maxlen);

int isProtocolPresent(const char * const * protocols, char *protocol) {
  int i = 0;
  for (; protocols[i]; ++i)
    if (strncmp("https", protocols[i], strlen(protocol)) == 0)
      return 1;
  return 0;
}

int initHTTP(int https_required) {
  if (g_curl != NULL)
    return INIT_ALREADY_DONE;

  curl_global_init(CURL_GLOBAL_SSL);
  curl_version_info_data *curl_info = curl_version_info(CURLVERSION_NOW);
  if (https_required && !isProtocolPresent(curl_info->protocols, "https"))
    return HTTPS_NOT_AVAILABLE;

  g_curl = curl_easy_init();
  return OK;
}

size_t response_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  return size * nmemb;
}

int post(const char *url, const char *fields) {
  // Do not verify certificate
  curl_easy_setopt(g_curl, CURLOPT_SSL_VERIFYPEER, 0L);
  // Perform a POST
  curl_easy_setopt(g_curl, CURLOPT_POST, 1L);
  // Provide the URL
  curl_easy_setopt(g_curl, CURLOPT_URL, url);
  // Set the POST fields
  curl_easy_setopt(g_curl, CURLOPT_POSTFIELDSIZE, strnlen(fields, MAX_BODY_SIZE));
  curl_easy_setopt(g_curl, CURLOPT_POSTFIELDS, fields);
  // Set it to JSON
  struct curl_slist *headers=NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(g_curl, CURLOPT_HTTPHEADER, headers);
  // Redirect response to callback
  curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, response_callback);
  // Perform a POST
  int res = curl_easy_perform(g_curl);
  curl_slist_free_all(headers);
  if (res != CURLE_OK) {
    g_last_error = res;
    return HTTP_FAIL;
  }
  return HTTP_OK;
}

int releaseHTTP() {
  curl_easy_cleanup(g_curl);
  // Power down libcurl
  curl_global_cleanup();
  return HTTP_OK;
}

const char * getLastError() {
  return curl_easy_strerror(g_last_error);
}
