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
 * Testprogram for Shamir secret sharing scheme: key generation
 * Usage:  ./genKey keyLength
 *
 * Authors:
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *      Akos Frohner <Akos.Frohner@cern.ch>
 *
 * $Id: genKey.c,v 1.3 2010/03/31 13:00:31 hahkala Exp $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ssssI.h"

#define PROGNAME "ssss-generate-key"

/** Generate a random hex key in chunks of 4 chars*/
unsigned char * generateKey(int len){
  unsigned char *keyf;
  unsigned char bit[5];
  int i,ii,nBytes,iByte;

  lengthTest(len);
  nBytes=len/4;
  short string[nBytes];

  keyf=malloc(sizeof(char)*len);

  if (! RAND_bytes((unsigned char *)&string[0],sizeof(short)*nBytes)) {
    printf("Error during generating the key!\n");
    exit(EXIT_FAILURE);
  }

  for(iByte=0;iByte<nBytes;iByte++){
    for(i=0;i<4;i++) bit[i]='0';
    bit[4]='\0';
    sprintf(bit,"%4x",abs(string[iByte]));
    for(i=0;i<4;i++){
      ii=(iByte*4)+i;
      *(keyf+ii)=bit[i];
      if(bit[i]==' ') {
        *(keyf+(iByte*4)+i)='0';
      }
    }
  }
  *(keyf+(len))='\0';

  return keyf;
}

int main(int argc, char** argv){
  int keyLength;

  if (argc < 2) {
    printf("\n");
    printf("<%s> Version %s by (C) EGEE\n", PROGNAME, PACKAGE_VERSION);
    printf("usage: %s <key-length>\n", PROGNAME);
    exit(EXIT_FAILURE);
  }

  keyLength = atoi(argv[1]);

  printf("%s\n", generateKey(keyLength));

  return 0;
}

// vim:set ts=2 sw=2 et:
