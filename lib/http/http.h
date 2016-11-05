#ifndef __HTTP_H__
#define __HTTP_H__

#define HTTP_OK 0
#define HTTP_FAIL 1

/* Public API */

int initHTTP(int https_required);
int post(char *url, char *fields);
int releaseHTTP();
const char * getLastError();

/* Private API */

int isProtocolPresent(const char * const * protocols, char *protocol);


#endif // __HTTP_H__