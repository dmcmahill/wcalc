/* $Id: wcalc_loadsave.c,v 1.1 2001/09/23 17:38:11 dan Exp $ */

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
  fprintf(fp,"wcalc_file_version %s\n",WCALC_FILE_VERSION);
  fprintf(fp,"model_name %s\n",model_name);

  fprintf(fp,"\n");

}
  


#define MAXLINELEN 80

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
	    section=SEC_STRIP;
	  }
	  else if (strcmp(tok,"[substrate]") == 0){
	    section=SEC_SUBS;
	  }
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    section=SEC_OTHER;
	  }
	  else if ( (section==SEC_STRIP) || (section==SEC_SUBS) ){
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
	      fprintf(stderr,"wcalc_load:  could not read value to go"
		      " with %s= in \"%s\"\n",tok,fname);
	      exit(1);
	    }
	    
	    if (strcmp(tok,"w") == 0){
	      mymstrip->w = atof(val);
	      got_w = 1;
	    }
	    else if (strcmp(tok,"l") == 0){
	      mymstrip->l = atof(val);
	      got_l = 1;
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
  
  if (!got_l) {  
    fprintf(stderr,"wcalc_load:  missing data: L\n");   
    exit(1);  
  }
  if (!got_w) {  
    fprintf(stderr,"wcalc_load:  missing data: W\n");   
    exit(1);  
  }
  
  fclose(fp);
}





