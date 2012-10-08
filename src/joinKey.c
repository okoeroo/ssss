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
 * (joining for given split keys (NULL for not known keys))
 *
 * Usage:  ./joinKey splitKey1 splitKey2 .... splitKeyN
 *
 * Authors: 
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: joinKey.c,v 1.8 2010/03/31 13:00:31 hahkala Exp $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glite/security/ssss.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROGNAME "glite-ssss-join-key"

static void print_usage_and_die (int exit_code) {
    printf("\n");
    printf("<%s> Version %s by (C) EGEE\n", PROGNAME, PACKAGE_VERSION);
    printf("usage: %s [-q] [-h] [-V] (join-key|NULL)...\n", PROGNAME);
    printf("Examples:\n");
    printf("To recover key: 64aa67e55e5a52ac704b58bb0e1c2695\n");
    printf("Key with size 32 chars, need two keys to recover secret and I may have e.g. keys 3 and 4.\n");
    printf("%s NULL NULL c9952de5f904a1939c223e6bc866dd7e 40986fe02ca1bbe0aabf8af65bdec526\n", PROGNAME);
    printf("Recover a custom key 12345678 with size 8 chars. Need 3 split keys to recover the key and have split keys 1,2 and 5..\n");
    printf("%s NULL 6fbc0334 d2ecd103 NULL NULL ce4a189d\n", PROGNAME);
  exit(exit_code); 
}

int main(int argc, char** argv){
  unsigned int nShares=0;
  int flag;
  unsigned int i;
  int verbose = 1;    
  unsigned char *jKey;
  unsigned char ** keys;

  while ((flag = getopt (argc, argv, "hqV")) != -1) {
    switch (flag) {
      case 'h':
        print_usage_and_die(EXIT_SUCCESS);
        break;
      case 'V':
        printf("<%s> Version %s by (C) EGEE\n", PROGNAME, PACKAGE_VERSION);
        exit(EXIT_SUCCESS);
        break;
      case 'q':
        verbose = 0;
        break;
      default:
        print_usage_and_die(EXIT_FAILURE);
        break;
    }
  }

  if(argc < (optind + 2)){
    print_usage_and_die(EXIT_FAILURE);
  }
 
  nShares = (unsigned int) argc - optind;

  // Allocate splitkeys
  keys = (unsigned char **) malloc(nShares*sizeof(unsigned char *));

  for (i=0; i<nShares; i++) {
    if(strcmp(argv[i + optind], "NULL") == 0) {
      keys[i] = NULL;
    }
    else {
      keys[i] = argv[i + optind];
    }
  }
      
  // Join keys
  jKey = glite_security_ssss_join_keys(keys, nShares);
  if(jKey==NULL){
    printf("\n\nError in joining key. Check logfile");
    return 2;
  }
  if (verbose) printf("\nJoined key : ");
  printf("%s\n",jKey);

  return 0;
}

// vim:set ts=2 sw=2 et:
