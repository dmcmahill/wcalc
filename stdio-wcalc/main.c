/* $Id: main.c,v 1.13 2004/09/02 02:23:22 dan Exp $ */

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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "mathutil.h"

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
static void exec_air_coil_calc(double *args);
static void exec_coax_calc(double *args);
static void exec_coupled_microstrip_calc(double *args);
static void exec_ic_microstrip_calc(double *args);
static void exec_microstrip_calc(double *args);
static void exec_stripline_calc(double *args);

static void exec_air_coil_syn(double *args);

/*
 * not implemented yet here
static void exec_coax_syn(double *args);
static void exec_coupled_microstrip_syn(double *args);
static void exec_ic_microstrip_syn(double *args);
static void exec_microstrip_syn(double *args);
static void exec_stripline_syn(double *args);
*/

static void exec_version(double *args);
static void usage(char *);
static void version(void);
static void execute_file(FILE *fp, char *fname); 

static int verbose = 0;

static void usage(char *p)
{

  printf("Usage:  %s [-v | --verbose] [input_files]\n", p);
  printf("        %s [-h | --help]\n", p);
  printf("        %s [-V | --version]\n", p);
}

static void version(void)
{

  printf("stdio-wcalc-%s\n", VERSION);
}

int main(int argc, char **argv)
{
  int i;
  FILE *fp;

  i = 1;
  while( i < argc ) {
    if( (strcmp(argv[i], "-h") == 0) ||
	(strcmp(argv[i], "--help") == 0) ) {
      usage( argv[0] );
      exit(0);
    } else if( (strcmp(argv[i], "-v") == 0) ||
	       (strcmp(argv[i], "--verbose") == 0) ) {
      verbose = 1;
    } else if( (strcmp(argv[i], "-V") == 0) ||
	       (strcmp(argv[i], "--version") == 0) ) {
      version();
      exit(0);
    } else if( (strcmp(argv[i], "-v") == 0) ||
	(strcmp(argv[i], "--verbose") == 0) ) {
      verbose = 1;
    } else if( argv[i][0] == '-') {
      printf("option \"%s\" is not understood\n",
	     argv[i]);
      usage(argv[0]);
      exit(1);
    } else {
      break;
    }

    i++;
  }

  if( i >= argc ) {
    execute_file( stdin, "stdin" );
  } else {
    while( i < argc ) {
      if( (fp = fopen(argv[i], "r")) == NULL) {
	fprintf(stderr, "ERROR:  Could not open %s\n", argv[i]);
	exit(1);
      }
      execute_file( fp, argv[i] );
      fclose( fp );
      i++;
    }
  }

  return 0;
}


static void execute_file(FILE *fp, char *fname) 
{
  char line[256];
  char *tmps = NULL;
  char *tok, *nl;
  int narg;
  int cnt;
  void (*fn) (double *);
  double params[15];
  int lineno = 1;

  while( fgets(line, sizeof(line), fp) != NULL ) {
    /* strip off the final newline */
    nl = strpbrk(line, "\n");
    if( nl != NULL ) *nl = '\0';

    /* make a copy for later use */
    if( tmps != NULL )
      free(tmps);

    tmps = strdup(line);

    tok = strtok(line, " \t");

    /* read the command part of the line */
    if(tok == NULL) {
      /* blank line */
      narg = -1;
      fn = NULL;
    } else if( ( strlen(tok) > 0 ) && (tok[0] == '#') ) {
      /* a silent comment line */
      narg = -1;
      fn = NULL;
    } else if( ( strlen(tok) > 0 ) && (tok[0] == '*') ) {
      /* a verbose comment line */
      narg = -1;
      printf("%s\n", tmps);
      fn = NULL;
    } else if(strcmp(tok, "version") == 0) {
      narg = 0;
      fn = &exec_version;
    } else if(strcmp(tok, "air_coil_calc") == 0) {
      narg = 8;
      fn = &exec_air_coil_calc;
    } else if(strcmp(tok, "air_coil_syn") == 0) {
      narg = 9;
      fn = &exec_air_coil_syn;
    } else if(strcmp(tok, "coax_calc") == 0) {
      narg = 10;
      fn = &exec_coax_calc;
    } else if(strcmp(tok, "coupled_microstrip_calc") == 0) {
      narg = 10;
      fn = &exec_coupled_microstrip_calc;
    } else if(strcmp(tok, "ic_microstrip_calc") == 0) {
      narg = 11;
      fn = &exec_ic_microstrip_calc;
    } else if(strcmp(tok, "microstrip_calc") == 0) {
      narg = 9;
      fn = &exec_microstrip_calc;
    } else if(strcmp(tok, "stripline_calc") == 0) {
      narg = 9;
      fn = &exec_stripline_calc;
    } else {
      fprintf(stderr, "stdio-wcalc: %s:%d: unknown command \"%s\"\n", 
	      fname, lineno, tok);
      exit(1);
    }

    if( verbose && (fn != NULL) ) {
      printf("--> %s\n", tmps);
    }

    /* 
     * read in the rest of the line which contains arguments to the
     * command 
     */
    if (narg >= 0) {
      cnt = 0;
      while( ((tok = strtok(NULL, " \t")) != NULL ) && 
	     (cnt < sizeof(params)/sizeof(double)) ) {
	if( cnt >= narg ) 
	  fprintf(stderr, "Ignoring extra argument: \"%s\"\n", tok);
	else 
	  params[cnt] = strtod(tok, NULL);
	
	cnt++;
      }
      
      /* make sure we got the right number of arguments */
      if( cnt < narg ) {
	fprintf(stderr, "Not enough arguments.  Needed %d, "
		"got %d on line %d of %s\n",
		narg, cnt, lineno, fname);
      } else {
	/* and execute the command */
	fn(params);
      }
    }

    lineno++;
    fflush(stdout);
  }
  
}

/*
 *  [L, Q, SRF, len_out, fill_out, Lmax] = 
 *    air_coil_calc(N,len,fill,AWG,rho,dia,freq,flag);
 */
static void exec_air_coil_calc(double *args)
{
  /* our air_coil for calculations */
  air_coil_coil *line;

  /* create the line and fill in the parameters */
  line = air_coil_new();
  line->Nf    = args[0];
  line->len   = args[1];
  line->fill  = args[2];
  line->AWGf  = args[3];
  line->rho   = args[4];
  line->dia   = args[5];
  line->freq  = args[6];
  line->use_fill  = (int) rint(args[7]);

  /* run the calculation */
  air_coil_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g %g %g %g\n", 
	 line->L, line->Q, line->SRF,
	 line->len, line->fill, line->Lmax);

  /* clean up */
  air_coil_free(line);
  
  return;
}

/*
 *  [N,LEN,FILL,Q,SRF,Lmax] = 
 *      air_coil_syn(L, N, len, fill, AWG, rho, dia, freq, flag) 
 */
static void exec_air_coil_syn(double *args)
{
  int i, flag;
  /* our air_coil for calculations */
  air_coil_coil *line;

  /* create the line and fill in the parameters */
  line = air_coil_new();
  i = 0;
  line->L     = args[i++];
  line->Nf    = args[i++];
  line->len   = args[i++];
  line->fill  = args[i++];
  line->AWGf  = args[i++];
  line->rho   = args[i++];
  line->dia   = args[i++];
  line->freq  = args[i++];
  flag        = (int) rint(args[i++]);

  /* run the calculation */
  air_coil_syn(line, line->freq, flag);
  
  /* print the outputs */
  printf("%g %g %g %g %g %g\n", 
	 line->Nf, line->len, line->fill, line->Q, line->SRF, line->Lmax);

  /* clean up */
  air_coil_free(line);
  
  return;
}

/*
 * [z0,elen,loss,L,R,C,G] = 
 *  coax_calc(a,b,c,t,rho_a,rho_b,er,tand,len,f);
 */
static void exec_coax_calc(double *args)
{
  /* our coax for calculations */
  coax_line *line;

  /* create the line and fill in the parameters */
  line = coax_new();
  line->a        = args[0];
  line->b        = args[1];
  line->c        = args[2];
  line->tshield  = args[3];
  line->rho_a    = args[4];
  line->rho_b    = args[5];
  line->er       = args[6];
  line->tand     = args[7];
  line->len      = args[8];
  line->freq     = args[9];

  /* run the calculation */
  coax_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g %g %g %g %g\n", 
	 line->z0, line->elen,
	 line->loss,
	 line->L, line->R, line->C, line->G);

  /* clean up */
  coax_free(line);
  
  return;
}

/* 
 * [z0,k,z0e,z0o,kev,kodd,loss_ev,loss_odd, deltal_ev, deltal_odd] =
 *   coupled_microstrip_calc(w,s,h,l,tmet,rho,rough,er,tand,f);
 */
static void exec_coupled_microstrip_calc(double *args)
{
  /* our coupled_microstrip for calculations */
  coupled_microstrip_line *line;

  /* create the line and fill in the parameters */
  line = coupled_microstrip_line_new();
  line->w           = args[0];
  line->s           = args[1];
  line->subs->h     = args[2];
  line->l           = args[3];
  line->subs->tmet  = args[4];
  line->subs->rho   = args[5];
  line->subs->rough = args[6];
  line->subs->er    = args[7];
  line->subs->tand  = args[8];
  line->freq        = args[9];

  /* run the calculation */
  coupled_microstrip_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g %g %g %g %g %g %g %g\n", 
	 line->z0, line->k,
	 line->z0e, line->z0o, 
	 line->kev, line->kodd,
	 line->loss_ev, line->loss_odd, 
	 line->deltale, line->deltalo);

  /* clean up */
  coupled_microstrip_line_free(line);
  
  return;
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
  /* our microstrip for calculations */
  microstrip_line *line;

  /* create the line and fill in the parameters */
  line = microstrip_line_new();
  line->w           = args[0];
  line->subs->h     = args[1];
  line->l           = args[2];
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

/* 
 * [z0,loss,deltal] = 
 *   stripline_calc(w,h,l,tmet,rho,rough,er,tand,f);
 */
static void exec_stripline_calc(double *args)
{
  /* our stripline for calculations */
  stripline_line *line;

  /* create the line and fill in the parameters */
  line = stripline_line_new();
  line->w           = args[0];
  line->subs->h     = args[1];
  line->l           = args[2];
  line->subs->tmet  = args[3];
  line->subs->rho   = args[4];
  line->subs->rough = args[5];
  line->subs->er    = args[6];
  line->subs->tand  = args[7];
  line->freq        = args[8];

  /* run the calculation */
  stripline_calc(line, line->freq);
  
  /* print the outputs */
  printf("%g %g %g\n", line->z0, line->loss, line->deltal);

  /* clean up */
  stripline_line_free(line);
  
  return;
}

/* 
 * version
 */
static void exec_version(double *args)
{
  printf("%s\n", VERSION);

  return;
}
