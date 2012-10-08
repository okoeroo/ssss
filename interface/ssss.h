/** 
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
 *
 * Definition for Shamir secret sharing (SSS) scheme.
 *
 * Based on "How to Share a Secret", by Adi Shamir, Communications of
 * the ACM, November, 1979, Volume 22, Number 11, page 612.
 * See also http://en.wikipedia.org/wiki/Secret_sharing
 *
 * SSS provides a perfect \f$(t,n)\f$-threshold secret sharing scheme.
 * i.e. it is a method for \f$n\f$ parties to carry shares \f$s_i\f$ of
 * a secret s such that any \f$t\f$ of them are needed to recover
 * the secret, but so that no \f$t-1\f$ of them can do so. The threshold
 * is perfect if knowledge of \f$t-1\f$ or fewer shares provides no information
 * regarding \f$s\f$.
 * Shamir \f$(t,n)\f$-threshold scheme is based on classical Lagrange polynomial
 * interpolation of degree \f$t-1\f$ with modular arithmetic instead of real arithmetic.
 * The set of integers modulo a prime number 'prime' forms a field in which
 * interpolation is possible.
 * We choose to break the key in random substrings of 4 characters handled separately 
 * (hex value of 0 - \f$2^{16}\f$). 
 * It is also assumed that the hex key is based on positive numbers.
 * The prime number has to be bigger than the largest number than can
 * be represented (i.e. 0xff) and the number of shares n.
 * In order to handle all byte values (0x00-0xff) and so that we can multiply
 * two such numbers, the shared secrets will fit in a 16-bits integer
 * (unsigned short). The prime has to be less than \f$2^{16}\f$ in order to fit them
 * all. The largest such prime is 65521.
 */

/**
 * \mainpage Shamir Secret Sharing Scheme (SSSS)
 *  Documentation of SSSS used for splitting and joining of a key
 */

#ifndef GLITE_SECURITY_SSSS_H
#define GLITE_SECURITY_SSSS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function for spliting of a key.
 *
 * @param key   The key in hexadecimal string format.
 * @param nShares   The desired number of shares.
 * @param nNeeded   The number of keys required to recover.
 *
 * @return The array of random split keys in hexadecimal string format, if
 * there was no error. If there was an error, NULL is returned.
 *
 * The caller is responsible for freeing the allocated strings in
 * case of success.
 */
unsigned char ** glite_security_ssss_split_key(unsigned char * key, 
    unsigned int nShares, unsigned int nNeeded);

/**
 * Function for joining of split keys.
 *
 * @param keys  Array of key pieces in hexadecimal string format.
 * @param nShares   The number of shares.
 *
 * @return The joined key in hexadecimal string format, if there 
 * was no error. If there was an error, NULL is returned.
 *
 * The caller is responsible for freeing the allocated string in
 * case of success. The order of the submitted keys (**keys) does matter. 
 * If a key is missing, e.g. due to a server being down, this key 
 * should be set to NULL for the corresponding index in **keys. 
 */
unsigned char * glite_security_ssss_join_keys(unsigned char **keys, 
    unsigned int nShares);

/**
 * Function for spliting of a asci key for passwords.
 *
 * @param key   The key in hexadecimal string format.
 * @param nShares   The desired number of shares.
 * @param nNeeded   The number of keys required to recover.
 *
 * @return The array of split random password parts in hexadecimal 
 * string format, if there was no error. If there was an error, NULL 
 * is returned.
 *
 * The caller is responsible for freeing the allocated strings in
 * case of success.
 */
unsigned char ** glite_security_ssss_split_passwd(unsigned char * key, 
    unsigned int nShares, unsigned int nNeeded);

/**
 * Function for joining of split ascii keys for passwords.
 *
 * @param keys  Array of key pieces in hexadecimal string format.
 * @param nShares   The number of shares.
 *
 * @return The joined key in hexadecimal string format, if there 
 * was no error. If there was an error, NULL is returned.
 *
 * The caller is responsible for freeing the allocated string in
 * case of success. The order of the submitted password parts (**keys) 
 * does matter. If a key is missing, e.g. due to a server being down, this part 
 * should be set to NULL for the corresponding index in **keys. 
 */
unsigned char * glite_security_ssss_join_passwd(unsigned char **keys, 
    unsigned int nShares);

#ifdef __cplusplus
}
#endif


#endif /* GLITE_SECURITY_SSSS_H */
