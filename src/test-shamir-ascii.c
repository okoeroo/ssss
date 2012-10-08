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
 *
 * Authors: 
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: test-shamir-ascii.c,v 1.3 2010/03/31 13:00:31 hahkala Exp $
 */

#include <glite/security/ssss.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define PROGNAME "test-shamir-ascii"

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
    printf("Examples: (NOTE: \" marks are needed if shell variables are included in the password string)\n");
    printf("5 split passwords 2 are needed to unlock\n");
    printf("%s 5 2 \"#%&lkXYt\"\n",PROGNAME);
    printf("7 split keys, 3 are needed to unlock.\n");
    printf("%s 7 3 \"#%&lkXYt\"\n",PROGNAME);
    exit(EXIT_FAILURE);
  }

  nShares= (unsigned int) atoi(argv[1]);
  nNeeded= (unsigned int) atoi(argv[2]);
  key = argv[3];

  printf("\nPassword to split (%d of %d): %s", nNeeded, nShares, key);
  
  // Split keys
  keys = glite_security_ssss_split_passwd(key, nShares, nNeeded);
  if(keys==NULL){
    printf("\n\nError in splitting password. Check logfile");
    return 1;
  }

  printf("\n\nSplit passwords:");
  for(i=0;i<nShares;i++){
    printf("\nx = %i splitKey = %s",i+1,keys[i]);
  }
  
  // Join passwords
  jKey = glite_security_ssss_join_passwd(keys, nShares);
  if(jKey==NULL){
    printf("\n\nError in joining password. Check logfile");
    return 2;
  }

  printf("\n\nJoined password : %s\n",jKey);

  i = strcmp(key, jKey);
  if (0 == i) {
    printf("The joined password is the same as the original.\n");
  }
  else {
    printf("ERROR: the original and the joined passwords are not the same!\n");
  }
  return i;
  
}

/* vim:set sw=2 ts=2 et si: */
