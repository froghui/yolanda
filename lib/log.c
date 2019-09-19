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

void yolanda_log(int severity, const char *msg) {
    const char *severity_str;
    switch (severity) {
        case LOG_DEBUG_TYPE:
            severity_str = "debug";
            break;
        case LOG_MSG_TYPE:
            severity_str = "msg";
            break;
        case LOG_WARN_TYPE:
            severity_str = "warn";
            break;
        case LOG_ERR_TYPE:
            severity_str = "err";
            break;
        default:
            severity_str = "???";
            break;
    }
    (void) fprintf(stdout, "[%s] %s\n", severity_str, msg);

}

void yolanda_logx(int severity, const char *errstr, const char *fmt, va_list ap)
{
    char buf[1024];
    size_t len;

    if (fmt != NULL)
        vsnprintf(buf, sizeof(buf), fmt, ap);
    else
        buf[0] = '\0';

    if (errstr) {
        len = strlen(buf);
        if (len < sizeof(buf) - 3) {
            snprintf(buf + len, sizeof(buf) - len, ": %s", errstr);
        }
    }

    yolanda_log(severity, buf);
}

void yolanda_msgx(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    yolanda_logx(LOG_MSG_TYPE, NULL, fmt, ap);
    va_end(ap);
}

void yolanda_debugx(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    yolanda_logx(LOG_DEBUG_TYPE, NULL, fmt, ap);
    va_end(ap);
}