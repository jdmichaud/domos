#ifndef __HTTP_H__
#define __HTTP_H__

/* Public API */

int initHTTP();
int post(char *url, char *fields);
void releaseHTTP();
char *getLastError();

/* Private API */

int isProtocolPresent(const char * const * protocols, char *protocol) {


#endif // __HTTP_H__