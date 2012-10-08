/*
 * Copyright (c) Members of the EGEE Collaboration. 2006-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright
 * holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 *     Akos Frohner <Akos.Frohner@cern.ch>
 */

#include <stdio.h>                  /* for NULL */
#include <stdlib.h>                 /* for free() */
#include <string.h>                 /* for strcmpy() and strlen() */
#include <stdarg.h>                 /* for va_list */
#include <time.h>                   /* for time/localtime/strftime */

#include "ssssI.h"

#define PRIVATE static
#define PUBLIC /* empty */

static SSSS_I_log4c_LogLevel SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_UNKNOWN;
static FILE *SSSS_I_log4c_current_logfile = NULL;

static char *SSSS_I_log4c_loglevel_names[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

#ifndef __GNUC__
#define __attribute__(x)
#endif
void SSSS_I_log4c_init(void) __attribute__((__constructor__));
void SSSS_I_log4c_done(void) __attribute__((__destructor__));

PUBLIC void SSSS_I_log4c_init(void) {
    char *envar;

    SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_NONE;

    envar = getenv(SSSSEnv_LOG_LEVEL);
    if (envar != NULL) {
        if(strncmp(envar, SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_DEBUG], 
                   strlen(SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_DEBUG])) == 0) {
            SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_DEBUG;
        }
        else if(strncmp(envar, SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_INFO], 
                        strlen(SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_INFO])) == 0) {
            SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_INFO;
        }
        else if(strncmp(envar, SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_WARN], 
                        strlen(SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_WARN])) == 0) {
            SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_WARN;
        }
        else if(strncmp(envar, SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_ERROR], 
                        strlen(SSSS_I_log4c_loglevel_names[SSSS_I_LOG4C_ERROR])) == 0) {
            SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_ERROR;
        }
        else {
            SSSS_I_log4c_current_loglevel = SSSS_I_LOG4C_NONE;
        }
    }

    if (SSSS_I_log4c_current_loglevel < SSSS_I_LOG4C_NONE) {
        envar = getenv(SSSSEnv_LOG_FILE);
        if (envar != NULL) {
            SSSS_I_log4c_current_logfile = fopen(envar, "a+");
            // returns NULL on error, in which case we will
            // log to the stderr anyway
        }
    }
}

PUBLIC void SSSS_I_log4c_done(void) {
    if (SSSS_I_log4c_current_logfile != NULL) {
        fclose(SSSS_I_log4c_current_logfile);
        SSSS_I_log4c_current_logfile = NULL;
    }
}

/* The goal: 2006-06-27 10:40:01,655 */
#define SSSS_I_LOG4C_TIME_FORMAT "%F %T"
#define SSSS_I_LOG4C_TIME_LENGTH 25
static char SSSS_I_log4c_time[SSSS_I_LOG4C_TIME_LENGTH];

PUBLIC int SSSS_I_log4c_check_loglevel(SSSS_I_log4c_LogLevel loglevel) 
{
    if (loglevel > SSSS_I_LOG4C_ERROR) return 0;
    if (SSSS_I_log4c_current_loglevel > loglevel) return 0;
    return 1;
}

PUBLIC void SSSS_I_log4c_printf(SSSS_I_log4c_LogLevel loglevel, 
    const char *file, const char *function, const int line,
    const char *format, ...) 
{
    if (loglevel > SSSS_I_LOG4C_ERROR) return;
    if (SSSS_I_log4c_current_loglevel > loglevel) return;

    va_list ap;
    FILE *logfile = SSSS_I_log4c_current_logfile;
    time_t logt;
    struct tm *logtm;
    const char *basename;

    if (NULL == logfile) logfile = stderr;
    
    logt = time(NULL);
    logtm = localtime(&logt);
    if (logtm == NULL) {
        SSSS_I_log4c_time[0] = '\0';
    }
    else if(strftime(SSSS_I_log4c_time, sizeof(SSSS_I_log4c_time), SSSS_I_LOG4C_TIME_FORMAT, logtm) == 0) {
        SSSS_I_log4c_time[0] = '\0';
    }
    
    basename = rindex(file, '/');
    if (NULL != basename) {
        basename++; // skip to after the slash
    }
    else {
        basename = file;
    }

	va_start(ap, format);
    fprintf(logfile, "%s %s - ", SSSS_I_log4c_time, SSSS_I_log4c_loglevel_names[loglevel]);
    vfprintf(logfile, format, ap);
    fprintf(logfile, " - %s#%s:%d\n", basename, function, line);
    fflush(logfile);
	va_end(ap);
}

/* End of file. */
