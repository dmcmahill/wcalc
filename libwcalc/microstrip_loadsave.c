/* $Id: microstrip_loadsave.c,v 1.1 2001/10/05 00:37:33 dan Exp $ */

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

/* #define DEBUG */

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

#include "alert.h"
#include "microstrip.h"
#include "microstrip_loadsave.h"
#include "wcalc_loadsave.h"

#define FILE_VERSION "0.1"

#define SEC_OTHER 0
#define SEC_STRIP 1
#define SEC_SUBS  2

int microstrip_load(microstrip_line *line, FILE *fp)
{
  char file_line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;

  int i;

  int got_l=0;
  int got_w=0;
  int got_z0=0;
  int got_len=0;
  int got_h=0;
  int got_er=0;
  int got_tmet=0;
  int got_rho=0;
  int got_rough=0;
  int got_tand=0;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val=file_read_val(fp,"[microstrip]","file_version")) == NULL ){
    alert("Could not determine the microstrip file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("microstrip_loadsave.c:microstrip_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  /* 
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */

  while( fgets(file_line,MAXLINELEN,fp) != NULL ){
    tok = strtok(file_line,FIELDSEP);
    if (tok != NULL){
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      /* skip comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,"[microstrip]") == 0){
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
	      alert("microstrip_load:  could not read value to go"
		      " with %s=\n",tok);
	      return -1;
	    }
	    
	    if (strcmp(tok,"w") == 0){
	      line->w = atof(val);
	      got_w = 1;
	    }
	    else if (strcmp(tok,"l") == 0){
	      line->l = atof(val);
	      got_l = 1;
	    }
	    else if (strcmp(tok,"z0") == 0){
	      line->z0 = atof(val);
	      got_z0 = 1;
	    }
	    else if (strcmp(tok,"elen") == 0){
	      line->len = atof(val);
	      got_len = 1;
	    }
	    else if (strcmp(tok,"h") == 0){
	      line->subs->h = atof(val);
	      got_h = 1;
	    }
	    else if (strcmp(tok,"er") == 0){
	      line->subs->er = atof(val);
	      got_er = 1;
	    }
	    else if (strcmp(tok,"tmet") == 0){
	      line->subs->tmet = atof(val);
	      got_tmet = 1;
	    }
	    else if (strcmp(tok,"rho") == 0){
	      line->subs->rho = atof(val);
	      got_rho = 1;
	    }
	    else if (strcmp(tok,"rough") == 0){
	      line->subs->rough = atof(val);
	      got_rough = 1;
	    }
	    else if (strcmp(tok,"tand") == 0){
	      line->subs->tand = atof(val);
	      got_tand = 1;
	    }
	    else {
	      alert("microstrip_load:  unknown token \"%s\"\n",tok);
	      return -1;
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
    alert("microstrip_load:  missing data: L\n");   
    return -1;  
  }
  if (!got_w) {  
    alert("microstrip_load:  missing data: W\n");   
    return -1;  
  }
  if (!got_z0) {  
    alert("microstrip_load:  missing data: Z0\n");   
    return -1;  
  }
  if (!got_len) {  
    alert("microstrip_load:  missing data: Elen\n");   
    return -1;  
  }
  if (!got_h) {  
    alert("microstrip_load:  missing data: H\n");   
    return -1;  
  }
  if (!got_er) {  
    alert("microstrip_load:  missing data: ER\n");   
    return -1;  
  }
  if (!got_tmet) {  
    alert("microstrip_load:  missing data: TMET\n");   
    return -1;  
  }
  if (!got_rho) {  
    alert("microstrip_load:  missing data: RHO\n");   
    return -1;  
  }
  if (!got_rough) {  
    alert("microstrip_load:  missing data: ROUGH\n");   
    return -1;  
  }
  if (!got_tand) {  
    alert("microstrip_load:  missing data: TAND\n");   
    return -1;  
  }
  
  return 0;
}


void microstrip_save(microstrip_line *line, FILE *fp, char *fname)
{
  wcalc_save_header(fp, fname, FILE_MICROSTRIP);

  fprintf(fp,"[microstrip]\n");
  fprintf(fp,"\n#\n#file format version\n#\n\n");

  fprintf(fp,"file_version = %s\n\n",FILE_VERSION);
  /*
   * The input values
   */

  fprintf(fp,"#\n# Input parameters\n#\n\n");

   fprintf(fp,"# Length (meters)\n");
  fprintf(fp,"L = %g\n",line->l);

  fprintf(fp,"# Width (meters)\n");
  fprintf(fp,"W = %g\n",line->w);

  fprintf(fp,"# Characteristic Impedance (ohms)\n");
  fprintf(fp,"Z0 = %g\n",line->z0);

  fprintf(fp,"# Electrical Length (degrees)\n");
  fprintf(fp,"Elen = %g\n",line->len);

  fprintf(fp,"\n");

  fprintf(fp,"[substrate]\n");

  fprintf(fp,"# Height (meters)\n");
  fprintf(fp,"H = %g\n",(line->subs)->h);

  fprintf(fp,"# Relative dielectric constant\n");
  fprintf(fp,"ER = %g\n",(line->subs)->er);

  fprintf(fp,"# Metalization thickness (meters)\n");
  fprintf(fp,"TMET = %g\n",(line->subs)->tmet);

  fprintf(fp,"# Metalization resistivity relative to copper\n");
  fprintf(fp,"RHO = %g\n",(line->subs)->rho);

  fprintf(fp,"# Metalization surface roughness (meters-RMS)\n");
  fprintf(fp,"ROUGH = %g\n",(line->subs)->rough);

  fprintf(fp,"# Dielectric loss tangent\n");
  fprintf(fp,"TAND = %g\n",(line->subs)->tand);

  fprintf(fp,"\n");

  /*
   * The desired user units
   */

  fprintf(fp,"#\n# Desired user units\n#\n\n");

  fprintf(fp,"# Length scale factor (meters/unit)\n");

}

