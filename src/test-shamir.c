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
 * Testprogram for Shamir secret sharing scheme
 *      (splitting and joining for all shares)
 * Usage: ./test-shamir nShares nNeeded key
 *
 * Authors:
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: test-shamir.c,v 1.7 2010/03/31 13:00:31 hahkala Exp $
 */

#include <ssss.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define PROGNAME "test-shamir"

// =============================     MAIN    ================================================
/**  Testprogram for Shamir secret sharing scheme   */
int main(int argc, char** argv){
  unsigned int nNeeded=0;
  unsigned int nShares=0;
  unsigned int i;
  unsigned char *key;
  unsigned char *jKey;
  unsigned char ** keys;

  if(argc < 3){
    printf("Usage: progname nShares nNeeded key\n");
    printf("Examples: \n");
    printf("5 split keys 2 are needed to unlock\n");
    printf("%s 5 2 123456781234678\n",PROGNAME);
    printf("7 split keys, 3 are needed to unlock.\n");
    printf("%s 7 3 12345678\n",PROGNAME);
    exit(EXIT_FAILURE);
  }

  nShares= (unsigned int) atoi(argv[1]);
  nNeeded= (unsigned int) atoi(argv[2]);
  key = argv[3];

  printf("\nKey to split (%d of %d): %s", nNeeded, nShares, key);

  // Split keys
  keys = ssss_split_key(key, nShares, nNeeded);
  if(keys==NULL){
    printf("\n\nError in splitting key. Check logfile");
    return 1;
  }

  printf("\n\nSplit keys:");
  for(i=0;i<nShares;i++){
    printf("\nx = %i splitKey = %s",i+1,keys[i]);
  }

  // Join keys
  jKey = ssss_join_keys(keys, nShares);
  if(jKey==NULL){
    printf("\n\nError in joining key. Check logfile");
    return 2;
  }
  printf("\n\nJoined key : %s\n",jKey);

  i = strcmp(key, jKey);
  if (0 == i) {
    printf("The joined key is the same as the original.\n");
  }
  else {
    printf("ERROR: the original and the joined key are not the same!\n");
  }
  return i;

}

/* vim:set sw=2 ts=2 et si: */
