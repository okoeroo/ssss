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
 * (splitting of an ascii password)
 *
 * Authors: 
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: splitPasswd.c,v 1.3 2010/03/31 13:00:31 hahkala Exp $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glite/security/ssss.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROGNAME "glite-ssss-split-passwd"

static void print_usage_and_die (int exit_code) {
  printf("\n");
  printf("<%s> Version %s by (C) EGEE\n", PROGNAME, PACKAGE_VERSION);
  printf("usage: %s [-q] [-h] <nShares> <nNeeded> <key>\n", PROGNAME);
  printf("Examples: (NOTE: \" marks are needed if shell variables are included in the password string)");
  printf("5 split passwords 2 are needed to unlock\n");
  printf("%s 5 2 \"#%&lkXYt\"\n", PROGNAME);
  printf("7 split paswords, 3 are needed to find original password.\n");
  printf("%s 7 3 \"#%&lkXYt\"\n", PROGNAME);
  exit(exit_code); 
}

int main(int argc, char** argv){
  unsigned int nNeeded;
  unsigned int nShares;
  unsigned int i;
  int flag;
  int verbose = 1;    
  unsigned char *key;
  unsigned char ** keys;

  while ((flag = getopt (argc, argv, "hq")) != -1) {
    switch (flag) {
      case 'h':
        print_usage_and_die(EXIT_SUCCESS);
        break;
      case 'q':
        verbose = 0;
        break;
      default:
        print_usage_and_die(EXIT_FAILURE);
        break;
    }
  }

  if(argc != (optind + 3)){
    print_usage_and_die(EXIT_FAILURE);
  }

  nShares = (unsigned int) atoi(argv[optind + 0]);
  nNeeded = (unsigned int) atoi(argv[optind + 1]);
  key = argv[optind + 2];
  
  if (verbose) printf("\nPassword to split (%d of %d): %s", nNeeded, nShares, key);

  // Split keys 
  keys = glite_security_ssss_split_passwd(key,nShares,nNeeded);
  if(keys==NULL){
    printf("\n\nError in splitting password. Check logfile");
    return 1;
  }

  if (verbose) printf("\n\nSplit passwords:");
  for(i=0;i<nShares;i++){
    if (verbose) printf("\n%i. splitPasswd = ",i+1);
    printf("%s ", keys[i]);
  }
  printf("\n");

  return 0;
}

// vim:set ts=2 sw=2 et:
