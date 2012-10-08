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
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *      Akos Frohner <Akos.Frohner@cern.ch>
 */
#ifndef SSSS_I_H
#define SSSS_I_H

/* Internal header file for SSSS */

#include "glite/security/ssss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>

/** Setting highest possible prime less than 2^16. */
static const long prime = 65521;

/**Function for getting a random hex string*/
unsigned char * generateKey(int len);
/**Function for calculating the modular inverse*/
int inverseModulo(long n, long *in);
/**Test for correct length provided */
int lengthTest(int keyLength);
/**Test for valid hex characters */
int hextest(char x);
/**Conversion of ascii string to valid hex string */
unsigned char *ascii2hex(unsigned char *asciiKey);
/**Conversion of hex string to valid ascii string */
unsigned char *hex2ascii(unsigned char *hexKey);

/* Name of the environment variable holding the log level. */
#define SSSSEnv_LOG_LEVEL      "GLITE_SSSS_LOG_LEVEL"
/* Name of the environment variable holding the log filename. */
#define SSSSEnv_LOG_FILE       "GLITE_SSSS_LOG_FILE"

/** 
 * The SSSS_I_log4c_init() function initializes the common logging 
 * facility for service discovery plugins.
 *
 * SSSS_LOG_LEVEL environmental variable sets the log level,
 * if left empty, then no logging is performed. Acceptable values
 * are: DEBUG, INFO, WARN, ERROR
 *
 * SSSS_LOG_FILE may specify a file, where the logs are written.
 * If not set, then they are written to stdout.
 */
extern void SSSS_I_log4c_init();
/**
 * The SSSS_I_log4c_done() function destroys the context of the
 * common logging facility, closing open file handles, etc.
 */
extern void SSSS_I_log4c_done();

/*
 * Log levels for the SSSS_I_log4c_printf() function, however
 * it is recommended to use the provided macros!
 */
typedef enum {
    SSSS_I_LOG4C_DEBUG,
    SSSS_I_LOG4C_INFO,
    SSSS_I_LOG4C_WARN,
    SSSS_I_LOG4C_ERROR,
    SSSS_I_LOG4C_NONE,
    SSSS_I_LOG4C_UNKNOWN
} SSSS_I_log4c_LogLevel;

/**
 * Check if the current loglevel is greater than the specified.
 */
extern int SSSS_I_log4c_check_loglevel(SSSS_I_log4c_LogLevel loglevel);

extern void SSSS_I_log4c_printf(SSSS_I_log4c_LogLevel loglevel, 
    const char *file, const char *function, const int line,
    const char *format, ...);

#define SSSS_I_log4c_DEBUG(fmt, ...) SSSS_I_log4c_printf(SSSS_I_LOG4C_DEBUG, __FILE__, __func__, __LINE__, fmt,##__VA_ARGS__)
#define SSSS_I_log4c_INFO(fmt, ...) SSSS_I_log4c_printf(SSSS_I_LOG4C_INFO, __FILE__, __func__, __LINE__, fmt,##__VA_ARGS__)
#define SSSS_I_log4c_WARN(fmt, ...) SSSS_I_log4c_printf(SSSS_I_LOG4C_WARN, __FILE__, __func__, __LINE__, fmt,##__VA_ARGS__)
#define SSSS_I_log4c_ERROR(fmt, ...) SSSS_I_log4c_printf(SSSS_I_LOG4C_ERROR, __FILE__, __func__, __LINE__, fmt,##__VA_ARGS__)

#endif // SSSS_I_H
