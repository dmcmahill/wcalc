/* $Id: wcalc_loadsave.c,v 1.2 2001/11/02 01:25:42 dan Exp $ */

/*
 * Copyright (c) 2001 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <sys/types.h>
#include <time.h>
 
#include "wcalc_loadsave.h"


/*
 * Prints out the required header to a file.  
 *   fp         = a valid, open, file pointer where the data should be 
 *                written to. 
 *   fname      = if not NULL, then the string in fname (the file
 *                name) is written to a comment line in the output
 *                file. 
 *   model_name = Unique name for the particular model being saved in
 *                the file.  This must be in the list given in
 *                wcalc_loadsave.h 
 */
void wcalc_save_header(FILE *fp, char *fname, char *model_name)
{
  time_t now;

  now = time(NULL);
 
  assert(fp != NULL);

  fprintf(fp,"#\n");
  if (fname != NULL){
    fprintf(fp,"# File:      %s\n",fname);
  }
  fprintf(fp,"# Modified:  %s",ctime(&now));
  fprintf(fp,"# Wcalc Version %s\n",VER);
  fprintf(fp,"\n");

  fprintf(fp,"[wcalc]\n");
  fprintf(fp,"wcalc_file_version = %s\n",WCALC_FILE_VERSION);
  fprintf(fp,"model_name = %s\n",model_name);

  fprintf(fp,"\n");

}
  


#define SEC_WCALC 0
#define SEC_OTHER 1

void wcalc_load(FILE *fp)
{
  char line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;
  int i;

  /* we should never be given a NULL file pointer */
  assert(fp != NULL);

  while( fgets(line,MAXLINELEN,fp) != NULL ){
    tok = strtok(line,FIELDSEP);
    if (tok != NULL){
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      /* skip comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,"[wcalc]") == 0){
	    section=SEC_WCALC;
	  }
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    section=SEC_OTHER;
	  }
	  else if (section==SEC_WCALC) {
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
	      fprintf(stderr,"wcalc_load:  could not read value to go"
		      " with %s= \n",tok);
	      exit(1);
	    }
	    
	    if (strcmp(tok,"w") == 0){
	    }
	    else if (strcmp(tok,"l") == 0){
	    }
	    else {
	      fprintf(stderr,"wcalc_load:  unknown token \"%s\"\n",tok);
	      exit(1);
	    }
	  }
	  else {
	    /* 
	     * skip because this is another secion of the file.
	     * probably from global settings
	     */
	  }

	} while ( (tok = strtok(NULL,FIELDSEP)) != NULL );
      }
    }
  }
  
#ifdef notdef
  if (!got_w) {  
    fprintf(stderr,"wcalc_load:  missing data: W\n");   
    exit(1);  
  }
#endif
  
  fclose(fp);
}


/* 
 * Reads the value of a particular key from a particular section of
 * a wcalc file.  For example:
 *  val=file_read_val(fp,"[air_coil]","rho"); 
 * will look for the variable "rho" in the [air_coil] section.
 * If found, 'val' will point to the string value for rho otherwise
 * NULL is returned.
 */

char * file_read_val(FILE *fp, const char *section, const char *key)
{
  char line[MAXLINELEN];
  char *tok, *val, *ret;
  int sec_ok=0;
  int i;
  
  rewind(fp);
  
  while( fgets(line,MAXLINELEN,fp) != NULL ){
    /* read the first token from the line */
    tok = strtok(line,FIELDSEP);
    
    if (tok != NULL){
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      
      /* process each token on the line, skipping comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,section) == 0)
	    sec_ok=1;
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    sec_ok=0;
	  }
	  else if ( sec_ok ){
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
#ifdef DEBUG
	      fprintf(stderr,"wcalc_loadsave.c:file_read_val():  could not read value to go"
		      " with %s=.\n",tok);
#endif
	      return NULL;
	    }
	    if (strcmp(tok,key) == 0){
	      ret=strdup(val);
	      rewind(fp);
	      return ret;
	    }
	  } 
	} while ( (tok = strtok(NULL,FIELDSEP)) != NULL );
      }
    }
  }
  
  /* we shouldn't have gotten here if the key was found */
  rewind(fp);
  return NULL;
}


