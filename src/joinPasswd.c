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
 * (joining for given split password parts (NULL for not known keys))
 *
 * Authors:
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: joinPasswd.c,v 1.3 2010/03/31 13:00:31 hahkala Exp $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ssss.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROGNAME "ssss-join-passwd"

static void print_usage_and_die (int exit_code) {
    printf("\n");
    printf("<%s> Version %s by (C) EGEE\n", PROGNAME, PACKAGE_VERSION);
    printf("usage: %s [-q] [-h] (join-key|NULL)...\n", PROGNAME);
    printf("Examples:\n");
    printf("To recover password: #%&lkXYt\n");
    printf("Need two parts of the split password but have 5.\n");
    printf("%s 8190b6ea2758da88d522 03274abe284449c750df 84afde832930b8f7cc8d 064672572a1c2836484a 87ce062b2b089766c3f8\n", PROGNAME);
    printf("Need two parts to recover the password and have split part 2 and 4\n");
    printf("%s NULL 03274abe284449c750df NULL 064672572a1c2836484a\n", PROGNAME);
  exit(exit_code);
}

int main(int argc, char** argv){
  unsigned int nShares=0;
  int flag;
  unsigned int i;
  int verbose = 1;
  unsigned char *jKey;
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
  jKey = ssss_join_passwd(keys, nShares);
  if(jKey==NULL){
    printf("\n\nError in joining pasword. Check logfile");
    return 2;
  }
  if (verbose) printf("\nJoined password : ");
  printf("%s\n",jKey);

  return 0;
}

// vim:set ts=2 sw=2 et:
