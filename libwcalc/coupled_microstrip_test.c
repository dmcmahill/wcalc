/* $Id: coupled_microstrip_test.c,v 1.6 2005/01/06 22:53:35 dan Exp $ */

/*
 * Copyright (C) 2002, 2003, 2004 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#include "config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "coupled_microstrip.h"
#include "coupled_microstrip_loadsave.h"

#include "physconst.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FIELDSEP " \t"
#define MAXLINELEN 256

enum {ACTION_NONE, ACTION_CALC, ACTION_SYN} action;

int main(int argc, char **argv)
{
  double sf;

  char rline[MAXLINELEN];
  char *save_line;
  char *tok;
  FILE *fp;

  coupled_microstrip_line *line;

  printf("couple_microstrip_test():  Calling coupled_microstrip_line_new\n");
  line = coupled_microstrip_line_new();

  printf("couple_microstrip_test():  Calling wc_savestr_to_units\n");
  wc_savestr_to_units("5", line->units_lwst);

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
	  line->z0 = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->k = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->z0e = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->z0o = atof(tok);
	if ( (tok = strtok(NULL,FIELDSEP)) != NULL )
	  line->use_z0k = atoi(tok);
	action=ACTION_SYN;
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
	printf(" Metal width                 = %g mil\n",
	       M2MIL(line->w));
	printf(" Metal spacing               = %g mil\n",
	       M2MIL(line->s));
	printf(" Metal thickness             = %g mil\n",
	       M2MIL(line->subs->tmet));
	printf(" Metal resistivity           = %g ohm-m\n",
	       line->subs->rho);
	printf(" Metal surface roughness     = %g mil-rms\n",
	       M2MIL(line->subs->rough));
	printf(" Substrate thickness         = %g mil\n",
	       M2MIL(line->subs->h));
	printf(" Substrate dielectric const. = %g \n",
	       line->subs->er);
	printf(" Substrate loss tangent      = %g \n",
	       line->subs->tand);
	printf(" Frequency                   = %g MHz\n",
	       line->freq/1e6); 
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

      case ACTION_SYN:
	/*#ifdef DEBUG*/
	printf(" --------- Coupled_Microstrip Synthesis ----------\n");
	printf(" Metal width                 = %g mil\n",
	       M2MIL(line->w));
	printf(" Metal spacing               = %g mil\n",
	       M2MIL(line->s));
	printf(" Metal thickness             = %g mil\n",
	       M2MIL(line->subs->tmet));
	printf(" Metal resistivity           = %g ohm-m\n",
	       line->subs->rho);
	printf(" Metal surface roughness     = %g mil-rms\n",
	       M2MIL(line->subs->rough));
	printf(" Substrate thickness         = %g mil\n",
	       M2MIL(line->subs->h));
	printf(" Substrate dielectric const. = %g \n",
	       line->subs->er);
	printf(" Substrate loss tangent      = %g \n",
	       line->subs->tand);
	printf(" Frequency                   = %g MHz\n",
	       line->freq/1e6); 
	printf(" -------------- ---------------------- ----------\n");
	/*#endif*/
	printf("-----------------------------------------------------\n");
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
	coupled_microstrip_syn(line,line->freq);
	printf(" --------- Coupled_Microstrip Synthesis ----------\n");
	printf(" Metal width                 = %g mil\n",
	       M2MIL(line->w));
	printf(" Metal spacing               = %g mil\n",
	       M2MIL(line->s));
	printf(" Metal thickness             = %g mil\n",
	       M2MIL(line->subs->tmet));
	printf(" Metal resistivity           = %g ohm-m\n",
	       line->subs->rho);
	printf(" Metal surface roughness     = %g mil-rms\n",
	       M2MIL(line->subs->rough));
	printf(" Substrate thickness         = %g mil\n",
	       M2MIL(line->subs->h));
	printf(" Substrate dielectric const. = %g \n",
	       line->subs->er);
	printf(" Substrate loss tangent      = %g \n",
	       line->subs->tand);
	printf(" Frequency                   = %g MHz\n",
	       line->freq/1e6); 
	printf(" -------------- ---------------------- ----------\n");

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
