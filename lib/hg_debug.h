/*
 * http://c.learncodethehardway.org/book/ex20.html
 */


#ifndef _HG_DEBUG_H
#define _HG_DEBUG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>


// #define DEBUG_ON 1

// compile with DEBUG_ON defined -> then debug messages will print.
// all other kinds of messages still remain (e.g. errors and warnings)
#ifdef DEBUG_ON
#define hg_debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define hg_debug(M, ...)
#endif

// get a safe readable version of errno.
#define hg_clean_errno() (errno == 0 ? "None" : strerror(errno))

// macros for logging messages meant for the end use
#define hg_log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, hg_clean_errno(), ##__VA_ARGS__)

#define hg_log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, hg_clean_errno(), ##__VA_ARGS__)

#define hg_log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)


#define hg_enduser_warning(M, ...) fprintf(stderr, "[WARNING] " M "\n", ##__VA_ARGS__)



// check will make sure the condition A is true, and if not logs the error M (with variable arguments for log_err), then jumps to the function's error: for cleanup.
#define hg_check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// sentinel is placed in any part of a function that shouldn't run, and if it does prints an error message then jumps to the error: label. You put this in if-statements and switch-statements to catch conditions that shouldn't happen, like the default:.
#define hg_sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// check_mem that makes sure a pointer is valid, and if it isn't reports it as an error with "Out of memory."
#define hg_check_mem(A) check((A), "Out of memory.")

// An alternative macro check_debug that still checks and handles an error, but if the error is common then you don't want to bother reporting it. In this one it will use debug instead of log_err to report the message, so when you define NDEBUG the check still happens, the error jump goes off, but the message isn't printed.
#define hg_check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }


#endif /* _HG_DEBUG_H */
