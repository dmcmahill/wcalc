/* $Id: microstrip_test.c,v 1.2 2004/08/02 21:03:01 dan Exp $ */

/*
 * Copyright (c) 2002 Dan McMahill
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
 *        This product includes software developed by Dan McMahill
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "microstrip.h"
#include "microstrip_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FIELDSEP " \t"
#define MAXLINELEN 256

int main(int argc, char **argv)
{
  /* inches to meters */
  double sf=0.0254;
#ifdef notdef
  double a=7;
  double b[]={3.040,3.038,3.026,3.040,3.040,3.040};
  double c[]={0,1,2,1,2,3};
  int i;
  int npts = sizeof(b)/sizeof(double);
#endif
  double freq;
  char *str, *str2;
  char rline[MAXLINELEN];
  char *tok;
  FILE *fp;

  microstrip_line *line, *line2;

  line = microstrip_line_new();
  line2 = microstrip_line_new();
  line2->w=500;
  line2->subs->er=9.3;

  str = microstrip_save_string(line);
  printf("Example of microstrip_save_string() output:\n\"%s\"\n\n",str);

  str2 = microstrip_save_string(line2);
  printf("Example of microstrip_save_string() output:\n\"%s\"\n\n",str2);

  microstrip_load_string(line2, str);
  str2 = microstrip_save_string(line2);
  printf("Example of microstrip_save_string() output:\n\"%s\"\n\n",str2);

  if(strcmp(str, str2) != 0) {
    printf("ERROR:  str != str2\n");
    printf("%s\n%s\n", str, str2);
  }

  if (argc < 2) {
    printf("No input args \n");
    return 0;
  }

  if ( (fp = fopen(argv[1],"r")) == NULL ) {
    fprintf(stderr,"could not open \"%s\" for read.\n",argv[1]);
    exit(1);
  }

  while( fgets(rline,MAXLINELEN,fp) != NULL ){
    printf("Reading new line\n");
    /* read the first token from the line */
    tok = strtok(rline,FIELDSEP);
    
    if (tok != NULL){
      if ( (tok[0] == '#') | (tok[0] == '*') | 
	   (tok[0] == ';') | (tok[0] == '$') ) {
	printf("Skipping comment line\n");
	}
      else {

	sf = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->w = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->h = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->tmet = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->er = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->tand = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->rho = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->rough = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->l = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->freq = atof(tok);
      }

      if (tok != NULL) {
	freq = line->freq;
	
	microstrip_calc(line,freq);
	
	printf("W= %8.4f, H= %8.4f, Tmet= %8.4f, Er= %8.4f, Freq= %8.4f\n",
	       line->w, 
	       line->subs->h,
	       line->subs->tmet,
	       line->subs->er,
	       line->freq);
	
	printf("Z0= %8.4f Ohms, Keff= %8.5f, Loss= %8.5f dB, DeltaL= %8.5f\n",
	       line->z0,
	       line->keff,
	       line->loss,
	       line->deltal);
      }
      else {
	fprintf(stderr,"Read incomplete line\n");
      }
    }
  }
  
  fclose(fp);
  
  return 0;
}
