#ifndef __HTTP_H__
#define __HTTP_H__

#define HTTP_OK 0
#define HTTP_FAIL 1

/* Public API */
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int initHTTP(int https_required);
int post(const char *url, const char *fields);
int releaseHTTP();
const char * getLastError();

/* Private API */

int isProtocolPresent(const char * const * protocols, char *protocol);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HTTP_H__