/* $Id: main.c,v 1.6 2004/01/10 13:35:58 dan Exp $ */

/*
 * Copyright (c) 2004 Dan McMahill
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

/* echo "ic_microstrip_calc 20e-6 1e-3 5e-6 4.0 200e-6 11.8 1.0 1e-6
   3.9e-8 0.0 100e6" | ./stdio-wcalc */

/* #define DEBUG */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* i18n */
#include "gettext.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifndef _
#define _(String) gettext (String)
#endif
#ifndef gettext_noop
#define gettext_noop(String) (String)
#endif
#ifndef N_
#define N_(String) gettext_noop (String)
#endif

/* Headers for the various structures we can analyze */

#include "air_coil.h"
#include "coax.h"
#include "coupled_microstrip.h"
#include "ic_microstrip.h"
#include "microstrip.h"
#include "stripline.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/* local prototypes */
static void exec_ic_microstrip_calc(double *args);
static void exec_microstrip_calc(double *args);

int main(int argc, char **argv)
{
  char line[256];
  char *tok;
  int narg;
  int cnt;
  void (*fn) (double *);
  double params[15];
  int lineno = 1;

  while( fgets(line, sizeof(line), stdin) != NULL ) {
    tok = strtok(line, " \t");
    if(tok == NULL) {
      fprintf(stderr, "stdio-wcalc: null token\n");
      exit(1);
    }

    if(strcmp(tok, "ic_microstrip_calc") == 0) {
      narg = 11;
      fn = &exec_ic_microstrip_calc;
    } else if(strcmp(tok, "microstrip_calc") == 0) {
      narg = 9;
      fn = &exec_microstrip_calc;
    } else {
      fprintf(stderr, "stdio-wcalc: unknown command \"%s\"\n", tok);
      exit(1);
    }

    cnt = 0;
    while( ((tok = strtok(NULL, " \t")) != NULL ) && 
	   (cnt < sizeof(params)/sizeof(double)) ) {
      if( cnt >= narg ) 
	fprintf(stderr, "Ignoring extra argument: \"%s\"\n", tok);
      else 
	params[cnt] = strtod(tok, NULL);

      cnt++;
    }
    
    if( cnt < narg ) 
      fprintf(stderr, "Not enough arguments.  Needed %d, got %d on line %d\n",
	      narg, cnt, lineno);
    else
      fn(params);
      /* exec_ic_microstrip_calc(params); */

    lineno++;
  }
  
  return 0;
}

/*
 * [z0,L,R,C,G] = 
 *  ic_microstrip_calc(w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f);
 */
static void exec_ic_microstrip_calc(double *args)
{
  /* our ic_microstrip for calculations */
  ic_microstrip_line *line;

  /* create the line and fill in the parameters */
  line = ic_microstrip_line_new();
  line->w           = args[0];
  line->l           = args[1];
  line->subs->tox   = args[2];
  line->subs->eox   = args[3];
  line->subs->h     = args[4];
  line->subs->es    = args[5];
  line->subs->sigmas= args[6];
  line->subs->tmet  = args[7];
  line->subs->rho   = args[8];
  line->subs->rough = args[9];
  line->freq        = args[10];

  /* run the calculation */
  ic_microstrip_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g %g %g %g\n", line->Ro, line->Xo,
	 line->Lmis, line->Rmis, line->Cmis, line->Gmis);

  /* clean up */
  ic_microstrip_line_free(line);
  
  return;
}

/* 
 * [z0,keff,loss,deltal] =
 *   microstrip_calc(w,h,l,tmet,rho,rough,er,tand,f);
 */
static void exec_microstrip_calc(double *args)
{
  /* our ic_microstrip for calculations */
  microstrip_line *line;

  /* create the line and fill in the parameters */
  line = microstrip_line_new();
  line->w           = args[0];
  line->l           = args[1];
  line->subs->h     = args[2];
  line->subs->tmet  = args[3];
  line->subs->rho   = args[4];
  line->subs->rough = args[5];
  line->subs->er    = args[6];
  line->subs->tand  = args[7];
  line->freq        = args[8];

  /* run the calculation */
  microstrip_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g %g\n", line->z0, line->keff,
	 line->loss, line->deltal);

  /* clean up */
  microstrip_line_free(line);
  
  return;
}

