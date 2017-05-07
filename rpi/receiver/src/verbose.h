#ifndef __VERBOSE_H__
#define __VERBOSE_H__

extern bool g_verbose;
#define VERBOSE(s) if (g_verbose) { s; }

#endif // __VERBOSE_H__