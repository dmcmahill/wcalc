/* $Id: coupled_microstrip_test.c,v 1.1 2003/01/11 01:01:43 dan Exp $ */

/*
 * Copyright (c) 2002, 2003 Dan McMahill
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

#include "coupled_microstrip.h"
/* XXX need this #include "coupled_microstrip_loadsave.h" */

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FIELDSEP " \t"
#define MAXLINELEN 256

enum {ACTION_NONE, ACTION_CALC, ACTION_SYN} action;

int main(int argc, char **argv)
{
  double sf;
  int flag;

  char rline[MAXLINELEN];
  char *save_line;
  char *tok;
  FILE *fp;

  coupled_microstrip_line *line;

  line = coupled_microstrip_line_new();
  sf = 1.0;

  if ( argc < 2) {
    printf("No input args \n");
    exit(1);
  }

  if ( (fp = fopen(argv[1],"r")) == NULL ) {
    fprintf(stderr,"could not open \"%s\" for read.\n",argv[1]);
    exit(1);
  }

  while( fgets(rline,MAXLINELEN,fp) != NULL ){
    save_line=strdup(rline);

    action=ACTION_NONE;

#ifdef DEBUG
    printf("Reading new line\n");
#endif

    /* read the first token from the line */
    tok = strtok(rline,FIELDSEP);
    
    if (tok != NULL){
      if ( (tok[0] == '#') | (tok[0] == '*') | 
	   (tok[0] == ';') | (tok[0] == '$') ) {
#ifdef DEBUG
	printf("Skipping comment line\n");
#endif
      }
      else if(strcmp(tok,"calc") == 0) {
	/* calc freq */
#ifdef DEBUG
	printf("Reading \"calc\" line\n");
#endif
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL ) {
	  line->freq = atof(tok);
	}
	action=ACTION_CALC;
      }
      else if(strcmp(tok,"comment") == 0) {
	printf("* %s",save_line+7);
      }
      else if(strcmp(tok,"line") == 0) {
	/* line w s l */
#ifdef DEBUG
	printf("Reading \"line\" parameters\n");
#endif
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->w = sf*atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->s = sf*atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->l = sf*atof(tok);
      }
      else if(strcmp(tok,"scale") == 0) {
	/* scale sf */
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL ) {
	  sf = atof(tok);
#ifdef DEBUG
	  printf("Read new scale factor = %g meters/unit\n",sf);
#endif
	}
      }
      else if(strcmp(tok,"sub") == 0) {
	/* sub h er tmet rho rough tand */
#ifdef DEBUG
	printf("Reading \"sub\" parameters\n");
#endif
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->h = sf*atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->er = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->tmet = sf*atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->rho = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->rough = sf*atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->subs->tand = atof(tok);
      }
      else if(strcmp(tok,"syn") == 0) {
	/* syn freq z0 k z0e z0o len flag */
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->freq = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  flag = atoi(tok);
      }
      else {
#ifdef DEBUG
	printf("Skipping unknown line\n");
#endif
      }
      
      switch( action ) {
      case ACTION_NONE:
	break;
	
      case ACTION_CALC:
	/*#ifdef DEBUG*/
	printf(" --------- Coupled_Microstrip Analysis ----------\n");
	printf(" Metal width                 = %g mil\n",line->w);
	printf(" Metal spacing               = %g mil\n",line->s);
	printf(" Metal thickness             = %g mil\n",line->subs->tmet);
	printf(" Metal relative resistivity  = %g \n",line->subs->rho);
	printf(" Metal surface roughness     = %g mil-rms\n",
	       line->subs->rough);
	printf(" Substrate thickness         = %g mil\n",line->subs->h);
	printf(" Substrate dielectric const. = %g \n",line->subs->er);
	printf(" Substrate loss tangent      = %g \n",line->subs->tand);
	printf(" Frequency                   = %g MHz\n",line->freq/1e6); 
	printf(" -------------- ---------------------- ----------\n");
	/*#endif*/
	printf("-----------------------------------------------------\n");
	coupled_microstrip_calc(line,line->freq);
	printf("Z0E= %8.4f Ohms, Z0O= %8.4f Ohms, Z0= %8.4f Ohms, "
	       "k= %8.4f (%8.4f dB)\n",
	       line->z0e,
	       line->z0o,
	       line->z0,
	       line->k,
	       20.0*log10(line->k));
	printf("Keven= %8.5f, Kodd= %8.5f, Freq= %8.4f Hz\n",
	       line->kev,
	       line->kodd,
	       line->freq);
	break;

      default:
	printf("Error:  Unknown action=%d\n",action);
	break;
      } /* switch( action ) */
    } /* if(tok != NULL ) */
  } /* while(fgets ....) */

  fclose(fp);
  
  return 0;
}
