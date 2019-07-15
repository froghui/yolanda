//
// Created by shengym on 2019-07-07.
//

#include    "common.h"
#include    <stdarg.h>        /* ANSI C header file */
#include    <syslog.h>        /* for syslog() */


# define MAXLINE 4096


/* error - print a diagnostic and optionally exit */
void error(int status, int err, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
}


static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap) {
    int errno_save, n;
    char buf[MAXLINE + 1];

    errno_save = errno;        /* value caller might want printed */

    vsnprintf(buf, MAXLINE, fmt, ap);    /* safe */

    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");


    fflush(stdout);        /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(stderr);

    return;
}

//void
//err_quit(const char *fmt, ...) {
//    va_list ap;
//
//    va_start(ap, fmt);
//    err_doit(0, LOG_ERR, fmt, ap);
//    va_end(ap);
//    exit(1);
//}

