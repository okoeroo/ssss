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
 * Functions and tests for the SSSS
 *
 * Authors: 
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: shamir-funcs.c,v 1.4 2010/03/31 13:00:31 hahkala Exp $
 */

#include "ssssI.h"

// Functions and test for the Shamir Secret Sharing Scheme

/** Routine for calculation of modular inverse */
int inverseModulo(long n, long *in){
  // Assume that we want to compute n(-1)(mod prime) (with gcd(prime,n)=1.
  // Run the Extended Euclidean algorithm to get a and b such that a*prime+b*n=1.
  // Rearranging this result, we see that a*prime=1-b*n, or b*n=1(mod prime).
  // This solves the problem of finding the modular inverse of n, as this shows
  // that n(-1)=b(mod prime).
  
  // More information about Basics of Computational Number Theory
  // can be found (at the date of Nov. 11, 2005)
  // http://www.math.oumbc.edu/~campbell/NumbThy/Class/BasicNumbThy.html
  
  int i;
  long a[3] = { 1L, 0L, prime };
  long b[3] = { 0L, 1L, n };
  
  while (a[2] != 0) {
    if (a[2] < b[2])
      for (i=0; i<3; ++i) {
        long tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
      }
    long q = a[2] / b[2];
    for (i=0; i<3; ++i)
      a[i] -= q*b[i];
  }

  // TEST FOR CORRECT RESULT
  // a*prime+b*n=1 where a is in b[0] and b is in b[1]
  if ((b[0]*prime+b[1]*n)%prime != 1) {
    SSSS_I_log4c_ERROR("Not able to modulo innverse %li",n);
    SSSS_I_log4c_ERROR("Values: %li %li %li %li",b[0],b[1],n,prime);
    return 0;
  }
  long inverse = b[1];
  
  while (inverse < 0)
    inverse += prime;
  
  *in = inverse;
  return 1;
}

// ======= Tests performed by the scheme =============0
/** Testing the length of the string*/
int lengthTest(int keyLength){
  if (keyLength < 4) {
    SSSS_I_log4c_ERROR("keyLength (%i) < 4",keyLength);
    return 0;
  }
  if (keyLength%4!=0) {
    SSSS_I_log4c_ERROR("keyLength (%i) must be dividable by 4",keyLength);
    return 0;
  }

  return 1;
}

/** Testing for valid hex chars*/
int hextest(char x){
  int asci;

  asci=(int) x;
  //SSSS_I_log4c_DEBUG("%i",(int) x);
  if (asci < 48 || asci > 102 ) {
    SSSS_I_log4c_ERROR("Invalid hex value :  %c ",asci);
    return 0;
  } else{
    if (asci > 57 &&  asci < 97 ) {
      if(asci < 65 || asci > 70) {
        SSSS_I_log4c_ERROR("Invalid hex value :  %c ",asci);
        return 0;
      }
    }
  }

  return 1;
}

/** Converting an ascii string to a valid hex string */
unsigned char *ascii2hex(unsigned char *keyf){
  unsigned char *hexkey;
  char hexbit[3];
  char bit[5];
  int ascii,len,i,j,hexlen;

  len=strlen(keyf);
  // Find length for hexstring
  hexlen=len*2;
  // If not dividable by 4, add 2 extra chars to make sure of a hextstring with chunks of 4 chars
  if(hexlen%4!=0) hexlen=hexlen+2;
 
  // Allocate memory for hexkey
  hexkey = malloc((hexlen+4)*sizeof(unsigned char *));

  // Inset length of string
  for(i=0;i<4;i++) bit[i]='0';
  bit[4]='\0';
  sprintf(bit,"%4x",len);
  for(i=0;i<4;i++)*(hexkey+i)=bit[i];

  // Loop chars
  for (i=0;i<(hexlen/2);i++){
    if(i<len){
      ascii = (int) keyf[i];      
    }else{
      ascii = 0;
    }

    for(j=0;j<2;j++) hexbit[j]='0';
    hexbit[2]='\0';
    sprintf(hexbit,"%2x",ascii);
    
    // Insert string from char no 4
    for(j=0;j<2;j++)*(hexkey+4+(i*2)+j)=hexbit[j];
  }
  *(hexkey+hexlen+4)='\0';
  // Substitut ' ' with '0'
  for (i=0;i<(hexlen+4);i++){
    if(*(hexkey+i)==' ') *(hexkey+i)='0';
  }

  return hexkey;
}

/** Converting a hex string to a valid ascii string */
unsigned char *hex2ascii(unsigned char *keyf){
  unsigned char *asciikey;
  char hexbit[3];
  char bit[5];
  int ascii,len,i,j;
  char asciichar;
  
  // Find the length of the ascii string
  for(i=0;i<4;i++) bit[i]=*(keyf+i);
  bit[4]='\0';
  len = (int) strtol(bit,NULL,16);

  // Allocate memory for asciikey
  asciikey = malloc(len*sizeof(unsigned char *));
  
  // Loop chars
  for (i=0;i<len;i++){
    // Use hexvalues 4 to 4+(len*2) in chunks of 2
    for(j=0;j<2;j++) hexbit[j]=*(keyf+4+(i*2)+j);
    hexbit[2]='\0';
    ascii = strtol(hexbit,NULL,16);
    asciichar = (char) ascii;
    
    // Insert char into asciikey
    *(asciikey+i)=asciichar;
  }
  *(asciikey+len)='\0';
  
  return asciikey;
}
// vim:set ts=2 sw=2 et:
