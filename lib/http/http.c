#include "http.h"
#include <string.h>
#include <curl/curl.h>
#include <slcurses.h>

#define OK 0
#define INIT_ALREADY_DONE 1
#define HTTPS_NOT_AVAILABLE 2
#define FAIL

static CURL *g_curl = NULL;
static CURLcode g_last_error = 0;

int isProtocolPresent(const char * const * protocols, char *protocol) {
  for (int i = 0; protocols[i]; ++i)
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

int post(char *url, char *fields) {
  // Do not verify certificate
  curl_easy_setopt(g_curl, CURLOPT_SSL_VERIFYPEER, 0L);
  // Perform a POST
  curl_easy_setopt(g_curl, CURLOPT_POST, 1L);
  // Provide the URL
  curl_easy_setopt(g_curl, CURLOPT_URL, url);
  // Set the POST fields
  curl_easy_setopt(g_curl, CURLOPT_POSTFIELDS, fields);
  // Perform a PUT
  int res = curl_easy_perform(g_curl);
  if (res != CURLE_OK)
    g_last_error = res;
    return HTTP_FAIL;
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
