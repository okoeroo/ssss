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
 * Routines for splitting and joining of a hex key
 *
 * Authors: 
 *      Trygve Aspelien <trygve.aspelien@bccs.uib.no>
 *
 * $Id: shamir.c,v 1.7 2010/03/31 13:00:31 hahkala Exp $
 */

#include "ssssI.h"

/*
 *                  ======================
 *                   SHAMIR SCHEME STARTS
 *                  ======================
*/

// =============== SPLITKEY ===================================================================
// Routine allocates the char** with split keys to be returned. Deallocation has to be done 
// later by the user of the routine. 

/** Routine for splitting a hex key using SSSS*/
unsigned char ** glite_security_ssss_split_key(unsigned char * keyf,
  unsigned int nShares, unsigned int nNeeded) {
  unsigned int i,j,nBytes,iByte;
  unsigned char **keysf;
  unsigned int keyLength = strlen(keyf);
  unsigned char bit[5];

  // Test if nShares provided
  if (nShares <= 0) {
    SSSS_I_log4c_ERROR("nShares (%i) must be greater than 0",nShares);
    return NULL;
  }  

  // Test if nNeeded provided
  if (nNeeded <= 0) {
    SSSS_I_log4c_ERROR("nNeeded (%i) must be greater than 0",nNeeded);
    return NULL;
  }

  // If not enough shares => EXIT
  if (nShares < nNeeded) {
    SSSS_I_log4c_ERROR("nShares (%i) < nNeeded (%i)",nShares,nNeeded);
    return NULL;
  }
  // Test keylength
  if (! lengthTest(keyLength)) return NULL;

  // Test hex value for valid chars
  for(i=0;i<keyLength;i++)
    if (! hextest(keyf[i])) return NULL;

  // Everything OK, continues.....

  // Setting loop variable
  nBytes= (unsigned int) keyLength/4;

  // Allocate splitkeys
  keysf = (unsigned char **) malloc(nShares*sizeof(char *));
  if (!keysf) {
    SSSS_I_log4c_ERROR("Error allocate memory");
    return NULL;
  }
  for(i=0;i<nShares;i++){
    keysf[i]=(unsigned char *)malloc((keyLength+1)*sizeof(char));
    if (!keysf[i]) {
      SSSS_I_log4c_ERROR("Error allocate memory");
      while(--i >= 0) free(keysf[i]);
      free(keysf);
      return NULL;
    }
  } 

  // Initialize splitKeys
  for(i=0;i<nShares;i++){
    for(j=0;j<keyLength;j++){
      keysf[i][j]='0';
    }
    keysf[i][keyLength]='\0';
  }

  // Creating polynom (up to 2^16 = short)
  unsigned short polynom[nNeeded];

  // Loop over 4 bit's
  for(iByte=1;iByte<=nBytes;iByte++){

    // Initialize polynom
    for(i=0;i<nNeeded;i++) polynom[i]=0;

    // Randomizing polynom
    if (! RAND_bytes((unsigned char *)&polynom[0],sizeof(polynom))) {
      SSSS_I_log4c_ERROR("Error creating polynom");
      for(i=0;i<nShares;i++) free(keysf[i]);
      free(keysf);
      return NULL;
    }
    
    // Have to insert the secret for x=0, must convert hex string to short
    // Pointer to end of string
    for(i=0;i<4;i++) bit[i]='0';
    bit[4]='\0';
    for(i=0;i<4;i++){
      bit[i]=*(keyf+i+((iByte-1)*4));
    }
    bit[4]='\0';

    // Conversion of hex keystring to integer
    unsigned short s_key = strtol(bit,NULL,16);

    // Setting key as polynom for x=0
    polynom[nNeeded-1]=s_key;
    
    if(SSSS_I_log4c_check_loglevel(SSSS_I_LOG4C_DEBUG)){
      printf("\nRandom polynom:\n");    
      for(i=0;i<nNeeded;i++)
        printf("%i (x^%i) ",polynom[i],nNeeded-1-i);
      printf("\nHex: ");    
      for(i=0;i<nNeeded;i++)
        printf("x^%i=%x ",nNeeded-1-i,polynom[i]);
    }
    
    //--------------------------------------------------
    //Got a random polynom, creating nShares keys.
    //Sorting polynoms from highest to lowest order
    //-------------------------------------------------- 
    
    unsigned long xx; // x counter
    unsigned long xtemp,xtt,xt; // Summation variable
    
    for(xx=1;xx<=nShares;xx++){  
      xtemp=0;
      SSSS_I_log4c_DEBUG("\nx=%li ",xx );
      for(i=0;i<nNeeded;i++){
        //xt=((unsigned long) pow((xx),(nNeeded-1-i)))%prime;
        xt=1;
        for(j=0;j<(nNeeded-1-i);j++){
          xt=(unsigned long) (xt*xx)%prime;
          //printf(" i=%i j=%i xt=%i ",i,j,xt);
        }
        xtt= (unsigned long) (polynom[i]*xt)%prime;
        xtemp = (xtemp+xtt)%prime;
        SSSS_I_log4c_DEBUG("i=%i (%i) => %li & %li ",i,(nNeeded-1-i),xt,xtt);
      }
      SSSS_I_log4c_DEBUG("y=%li",xtemp);
     
      for(i=0;i<4;i++) bit[i]='0';
      bit[4]='\0';
      sprintf(bit,"%4lx",xtemp);
      
      for(i=0;i<4;i++){
        keysf[xx-1][i+(((iByte-1))*4)]=bit[i];
        if(keysf[xx-1][i+(((iByte-1))*4)]==' ') keysf[xx-1][i+(((iByte-1))*4)]='0';
      }
      // Add terminating 0
      if(iByte==nBytes){
        keysf[xx-1][nBytes*4]='\0';
      }
    }
    
  }
  return keysf;
}

// ============          joinKeySSS   =============================================
/** Routine for joining a hex key using SSSS*/
unsigned char * glite_security_ssss_join_keys(unsigned char **keysf,
  unsigned int nShares){
  unsigned char * jKey;
  unsigned long x[nShares];
  long ii,jj,k;
  unsigned int i,j;
  long num=0;
  long denom=0;
  unsigned long isecret=0; 
  long inarray[nShares];
  unsigned long c[nShares];
  unsigned long ikeys[nShares];
  long  nn;
  unsigned int iByte,nBytes; 
  unsigned char bit[5];
  unsigned int keyLength;
  unsigned int start;

  // Test if nShares provided
  if (nShares <= 0) {
    SSSS_I_log4c_ERROR("nShares (%i) must be greater than 0",nShares);
    return NULL;
  }  
  // Set keyLength for first well defined string
  start=0;
  for (i=0;keysf[i]==NULL;i++)
    start=i+1;
  keyLength=(unsigned int) strlen(keysf[start]);

  // Check length of split-keys
  for (i=start;i<nShares;i++){
    if(keysf[i]!=NULL){
      
      if(keyLength!=strlen(keysf[i])){
        SSSS_I_log4c_ERROR("All the split keys have to have the same length");
        return NULL;
      }    
    }
  }
  
  // Find active x-points
  nn=0;
  for (i=0;i<nShares;i++){
    inarray[i]=0;
    x[i]=0;
    if(keysf[i]!=NULL) {

      inarray[i]=1;
      x[i]=i+1;
      nn++;
    }
  }

  SSSS_I_log4c_DEBUG("Joining key with %li split keys...",nn);

  if (! lengthTest(keyLength)) return NULL;

  // Setting loop variable
  nBytes= (unsigned int) keyLength/4;
  jKey=malloc(sizeof(char)*(keyLength+1));
  if (!jKey) {
    SSSS_I_log4c_ERROR("Error allocate memory");
    return NULL;
  }
  

  for(i=0;i<(nBytes*4);i++) *(jKey+i)='0';
  *(jKey+(nBytes*4))='\0';

  if(SSSS_I_log4c_check_loglevel(SSSS_I_LOG4C_DEBUG)){
    for(i=0;i<nShares;i++){
      printf("\nsplitKey  x=%li ",x[i]);
      for(iByte=1;iByte<=nBytes;iByte++){
        if(inarray[i]!=0){
          for(j=0;j<4;j++){
            bit[j]=keysf[i][j+((iByte-1)*4)];
          }
          bit[4]='\0';
          for(k=0;k<4;k++)
            printf("%c",bit[k]);
          printf(" (%li) ",strtol(bit,NULL,16)); 
        }
      }
    }
  }
  
  // Loop over 4 chars
  for(iByte=1;iByte<=nBytes;iByte++){
     
    // Convert hex keys to integers
    for(i=0;i<nShares;i++){
      if(inarray[i]!=0){

        for(j=0;j<4;j++){
          ii=j+((iByte-1)*4);
          bit[j]=keysf[i][ii];
          // Test hex value
          if (! hextest(bit[j])) {
            free(jKey);
            return NULL;
          }
        }
        bit[4]='\0';

        ikeys[i]=strtol(bit,NULL,16);
      }else{
        ikeys[i]=0;
      }
    }
    
    // Get key back from split keys
    for (i=0;i<nShares;i++){
      ii=i+1;
      if(inarray[i]!=0){
        denom=1;
        num=1;
        for (j=0;j<nShares;j++){
          jj=j+1;
          if(inarray[j]!=0){
            if(jj!=ii){
              num*=-jj%prime;
              denom*=(ii-jj)%prime;
              num=num%prime;
              denom=denom%prime;
            }
          }
        }
        num=num%prime;
        denom=denom%prime;
        // Start array from 0
        while(num<0) num+=prime;
        while(denom<0) denom+=prime;
        if (! inverseModulo(denom, &denom)) {
          free(jKey);
          return NULL;
        }
        while(denom<0) denom+=prime;
        c[i]= (unsigned long) (num*denom)%prime;
        SSSS_I_log4c_DEBUG("\nc=%li",c[i]);
      }else{
        c[i]=0;
      }
    }
    
    // Defined c and have nNeeded ikeys. Make summation
    unsigned long xt;
    isecret=0;
    for  (i=0;i<nShares;i++){
      if(inarray[i]!=0){
        xt=0;
        xt= (unsigned long) (c[i]*ikeys[i])%prime;
        isecret= (unsigned long) (isecret+xt)%prime;
        SSSS_I_log4c_DEBUG("c[%li] = %lu x(%li)=%lu ==> %lu %lu %lu %li",
          i,c[i],i,ikeys[i],(unsigned long) c[i]*ikeys[i],
          (unsigned long)(c[i]*ikeys[i])/prime,xt,isecret);
      }    
    }
    
    
    isecret=isecret%prime;
    SSSS_I_log4c_DEBUG("Isecret: %li",isecret);
    
    
    for(i=0;i<4;i++) bit[i]='0';
    bit[4]='\0';
    sprintf(bit,"%4lx",isecret);
    for(i=0;i<4;i++){
      if(bit[i]==' ') bit[i]='0';
    }
    for(i=0;i<4;i++) *(jKey+i+((iByte-1)*4))=bit[i];
  }
  *(jKey+(nBytes*4))='\0';

  if(SSSS_I_log4c_check_loglevel(SSSS_I_LOG4C_DEBUG)){
    SSSS_I_log4c_DEBUG("Joined Key bits= ");
    for(iByte=1;iByte<=nBytes;iByte++){
      for(i=0;i<4;i++){
        bit[i]=*(jKey+i+((iByte-1)*4));
      }
      bit[4]='\0';
      for(j=0;j<4;j++)
        printf("%c",bit[j]);
      printf(" (%li) ",strtol(bit,NULL,16));
    }
  }
 
  return jKey;
}

// ============          glite_security_ssss_split_passwd   =============================================
/** Routine for splitting a ascii key using SSSS. Returns a char ** with split hex strings */
unsigned char ** glite_security_ssss_split_passwd(unsigned char * keyf,
						  unsigned int nShares, unsigned int nNeeded) {
  unsigned char *hexKey;
  unsigned char **keys;

  // Convert ascii to hex
  hexKey=ascii2hex(keyf);
  // Submit hex key
  keys=glite_security_ssss_split_key(hexKey,nShares,nNeeded);

  return keys;
}

// ============          glite_security_ssss_join_passwd   =============================================
/** Routine for joining an ascii key using SSSS*/
unsigned char * glite_security_ssss_join_passwd(unsigned char **keysf,
						unsigned int nShares){
  unsigned char *hexKey;
  unsigned char *asciiKey;
  
  // Find joined hex key
  hexKey = glite_security_ssss_join_keys(keysf,nShares);
  if(hexKey!=NULL){
    // Convert hex to ascii
    asciiKey=hex2ascii(hexKey);
  }else{
    asciiKey=NULL;
  }

  return asciiKey;
}
// vim:set ts=2 sw=2 et:
