/* $Id: main.c,v 1.27 2008/11/29 20:42:44 dan Exp $ */

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2009 Dan McMahill
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
#include "bars.h"
#include "coax.h"
#include "coplanar.h"
#include "coupled_microstrip.h"
#include "coupled_stripline.h"
#include "ic_microstrip.h"
#include "microstrip.h"
#include "stripline.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ERRMSG _("Error:  Calculation failed.  Please review any warnings.\n")

/* local prototypes */
static void exec_air_coil_calc(double *args);
static void exec_bars_calc(double *args);
static void exec_coax_calc(double *args);
static void exec_coplanar_calc(double *args);
static void exec_coupled_microstrip_calc(double *args);
static void exec_coupled_stripline_calc(double *args);
static void exec_ic_microstrip_calc(double *args);
static void exec_microstrip_calc(double *args);
static void exec_stripline_calc(double *args);

static void exec_air_coil_syn(double *args);
static void exec_coax_syn(double *args);
static void exec_coplanar_syn(double *args);
static void exec_coupled_microstrip_syn(double *args);
static void exec_coupled_stripline_syn(double *args);
static void exec_ic_microstrip_syn(double *args);
static void exec_microstrip_syn(double *args);
static void exec_stripline_syn(double *args);

static void exec_help(double *args);
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
    } else if(strcmp(tok, "?") == 0) {
      narg = 0;
      fn = &exec_help;
    } else if(strcmp(tok, "help") == 0) {
      narg = 0;
      fn = &exec_help;
    } else if(strcmp(tok, "version") == 0) {
      narg = 0;
      fn = &exec_version;
    } else if(strcmp(tok, "air_coil_calc") == 0) {
      narg = 8;
      fn = &exec_air_coil_calc;
    } else if(strcmp(tok, "air_coil_syn") == 0) {
      narg = 9;
      fn = &exec_air_coil_syn;
    } else if(strcmp(tok, "bars_calc") == 0) {
      narg = 10;
      fn = &exec_bars_calc;
    } else if(strcmp(tok, "coax_calc") == 0) {
      narg = 10;
      fn = &exec_coax_calc;
    } else if(strcmp(tok, "coax_syn") == 0) {
      narg = 12;
      fn = &exec_coax_syn;
    } else if(strcmp(tok, "coplanar_calc") == 0) {
      narg = 11;
      fn = &exec_coplanar_calc;
    } else if(strcmp(tok, "coplanar_syn") == 0) {
      narg = 14;
      fn = &exec_coplanar_syn;
    } else if(strcmp(tok, "coupled_microstrip_calc") == 0) {
      narg = 10;
      fn = &exec_coupled_microstrip_calc;
    } else if(strcmp(tok, "coupled_microstrip_syn") == 0) {
      narg = 14;
      fn = &exec_coupled_microstrip_syn;
    } else if(strcmp(tok, "coupled_stripline_calc") == 0) {
      narg = 10;
      fn = &exec_coupled_stripline_calc;
    } else if(strcmp(tok, "coupled_stripline_syn") == 0) {
      narg = 14;
      fn = &exec_coupled_stripline_syn;
    } else if(strcmp(tok, "ic_microstrip_calc") == 0) {
      narg = 11;
      fn = &exec_ic_microstrip_calc;
    } else if(strcmp(tok, "ic_microstrip_syn") == 0) {
      narg = 14;
      fn = &exec_ic_microstrip_syn;
    } else if(strcmp(tok, "microstrip_calc") == 0) {
      narg = 9;
      fn = &exec_microstrip_calc;
    } else if(strcmp(tok, "microstrip_syn") == 0) {
      narg = 12;
      fn = &exec_microstrip_syn;
    } else if(strcmp(tok, "stripline_calc") == 0) {
      narg = 9;
      fn = &exec_stripline_calc;
    } else if(strcmp(tok, "stripline_syn") == 0) {
      narg = 12;
      fn = &exec_stripline_syn;
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
  int i = 0;

  /* create the line and fill in the parameters */
  line = air_coil_new();
  line->Nf    = args[i++];
  line->len   = args[i++];
  line->fill  = args[i++];
  line->AWGf  = args[i++];
  line->rho   = args[i++];
  line->dia   = args[i++];
  line->freq  = args[i++];
  line->use_fill  = (int) rint(args[i++]);

  /* run the calculation */
  if(  air_coil_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else {
    /* print the outputs */
    printf("%g %g %g %g %g %g\n", 
	   line->L, line->Q, line->SRF,
	   line->len, line->fill, line->Lmax);
  }

  /* clean up */
  air_coil_free(line);
  
  return;
}

/*
 *  [N,LEN,FILL] = 
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
  if( air_coil_syn(line, line->freq, flag) ) {
    printf("%s", ERRMSG);
  } else {
    /* print the outputs */
    printf("%g %g %g\n", 
	   line->Nf, line->len, line->fill);
  }

  /* clean up */
  air_coil_free(line);
  
  return;
}

/*
 * [L1, L2, M, k] = 
 *  bars_calc(a, b, l1, d, c, l2, E, P, l3, f);
 */
static void exec_bars_calc(double *args)
{
  /* our bars for calculations */
  bars *bar;
  int i = 0;

  /* create the bars and fill in the parameters */
  bar = bars_new();

  bar->a        = args[i++];
  bar->b        = args[i++];
  bar->l1       = args[i++];

  bar->d        = args[i++];
  bar->c        = args[i++];
  bar->l2       = args[i++];

  bar->E        = args[i++];
  bar->P        = args[i++];
  bar->l3       = args[i++];

  line->freq    = args[i++];

  /* run the calculation */
  if( bars_calc(bar, bar->freq) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g\n", 
	   bar->L1, bar->L2,
	   bar->M, bar->k);
  }

  /* clean up */
  bars_free(bar);
  
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
  int i = 0;

  /* create the line and fill in the parameters */
  line = coax_new();
  line->a        = args[i++];
  line->b        = args[i++];
  line->c        = args[i++];
  line->tshield  = args[i++];
  line->rho_a    = args[i++];
  line->rho_b    = args[i++];
  line->er       = args[i++];
  line->tand     = args[i++];
  line->len      = args[i++];
  line->freq     = args[i++];

  /* run the calculation */
  if( coax_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g %g %g %g\n", 
	   line->z0, line->elen,
	   line->loss,
	   line->L, line->R, line->C, line->G);
  }

  /* clean up */
  coax_free(line);
  
  return;
}

/*
 * [a,b,c,er,len] = ...
 *   coax_syn(z0,elen,a,b,c,t,rho_a,rho_b,er,tand,f,flag);
 */
static void exec_coax_syn(double *args)
{
  /* our coax for calculations */
  coax_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = coax_new();
  line->z0       = args[i++];
  line->elen     = args[i++];
  line->a        = args[i++];
  line->b        = args[i++];
  line->c        = args[i++];
  line->tshield  = args[i++];
  line->rho_a    = args[i++];
  line->rho_b    = args[i++];
  line->er       = args[i++];
  line->tand     = args[i++];
  line->freq     = args[i++];

  /* run the calculation */
  if( coax_syn(line, line->freq, args[i++]) ) {
    printf("%s", ERRMSG);
  } else {
    /* print the outputs */
    printf("%g %g %g %g %g\n", 
	   line->a, line->b, line->c,
	   line->er, line->len);
  }

  /* clean up */
  coax_free(line);
  
  return;
}

static void exec_coplanar_calc(double *args)
{
  /* our coax for calculations */
  coplanar_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = coplanar_line_new();
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->with_ground = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( coplanar_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g %g %g %g %g %g %g %g %g \n", line->z0, line->keff,
	   line->len, line->loss, line->Ls, line->Rs, line->Cs, line->Gs,
	   line->lc, line->ld, line->deltal, line->skindepth);
  }

  /* clean up */
  coplanar_line_free(line);
  
  return;
}

/*
 * FIXME
 */
static void exec_coplanar_syn(double *args)
{
  /* our coax for calculations */
  coplanar_line *line;
  int i = 0;
  int flag;

  /* create the line and fill in the parameters */
  line = coplanar_line_new();
  line->Xo          = 0.0;
  line->Ro = line->z0 = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->with_ground = args[i++];
  line->freq        = args[i++];
  flag              = args[i++];

  /* run the calculation */
  if( coplanar_syn(line, line->freq, flag) ) {
    printf("%s", ERRMSG);
  } else {

    /* print the outputs */
    printf("%g %g %g %g %g\n", line->w, line->s, line->subs->h,
	   line->l, line->subs->er);
  }

  /* clean up */
  coplanar_line_free(line);
  
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
  int i = 0;

  /* create the line and fill in the parameters */
  line = coupled_microstrip_line_new();
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( coupled_microstrip_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g %g %g %g %g %g %g %g\n", 
	   line->z0, line->k,
	   line->z0e, line->z0o, 
	   line->kev, line->kodd,
	   line->loss_ev, line->loss_odd, 
	   line->deltale, line->deltalo);
  }

  /* clean up */
  coupled_microstrip_line_free(line);
}
  
/* 
 * [w_out,s_out,l_out] = 
 *   coupled_microstrip_syn(z0,k,elen,w,s,h,l,tmet,rho,rough,er,tand,f,0);
 * [w_out,s_out,l_out] = 
 *   coupled_microstrip_syn(z0e,z0o,elen,w,s,h,l,tmet,rho,rough,er,tand,f,1);
 */
static void exec_coupled_microstrip_syn(double *args)
{
  /* our coupled_microstrip for calculations */
  coupled_microstrip_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = coupled_microstrip_line_new();
  line->z0 = line->z0e = args[i++];
  line->k  = line->z0o = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];
  line->use_z0k     = !args[i++];

  /* run the calculation */
  if( coupled_microstrip_syn(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g\n", 
	   line->w, line->s, line->l);
  }

  /* clean up */
  coupled_microstrip_line_free(line);
  
  return;
}

/* 
 * [z0,k,z0e,z0o,kev,kodd,loss_ev,loss_odd, deltal_ev, deltal_odd] =
 *   coupled_stripline_calc(w,s,h,l,tmet,rho,rough,er,tand,f);
 */
static void exec_coupled_stripline_calc(double *args)
{
  /* our coupled_stripline for calculations */
  coupled_stripline_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = coupled_stripline_line_new();
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( coupled_stripline_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g %g %g %g %g %g\n", 
	   line->z0, line->k,
	   line->z0e, line->z0o, 
	   line->loss_ev, line->loss_odd, 
	   line->deltale, line->deltalo);
  }

  /* clean up */
  coupled_stripline_line_free(line);
}
  
/* 
 * [w_out,s_out,l_out] = 
 *   coupled_stripline_syn(z0,k,elen,w,s,h,l,tmet,rho,rough,er,tand,f,0);
 * [w_out,s_out,l_out] = 
 *   coupled_stripline_syn(z0e,z0o,elen,w,s,h,l,tmet,rho,rough,er,tand,f,1);
 */
static void exec_coupled_stripline_syn(double *args)
{
  /* our coupled_stripline for calculations */
  coupled_stripline_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = coupled_stripline_line_new();
  line->z0 = line->z0e = args[i++];
  line->k  = line->z0o = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->s           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];
  line->use_z0k     = !args[i++];

  /* run the calculation */
  if( coupled_stripline_syn(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g\n", 
	   line->w, line->s, line->l);
  }

  /* clean up */
  coupled_stripline_line_free(line);
  
  return;
}

/*
 * [Ro, Xo ,keff,elen,loss,L,R,C,G] = 
 *  ic_microstrip_calc(w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f);
 */
static void exec_ic_microstrip_calc(double *args)
{
  /* our ic_microstrip for calculations */
  ic_microstrip_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = ic_microstrip_line_new();
  line->w           = args[i++];
  line->l           = args[i++];
  line->subs->tox   = args[i++];
  line->subs->eox   = args[i++];
  line->subs->h     = args[i++];
  line->subs->es    = args[i++];
  line->subs->sigmas= args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( ic_microstrip_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g %g %g %g %g %g\n", line->Ro, line->Xo,
	   line->keff, line->len, line->loss,
	   line->Lmis, line->Rmis, line->Cmis, line->Gmis);
  }

  /* clean up */
  ic_microstrip_line_free(line);
  
  return;
}

/*
 * [w_out,h_out,tox_out,l_out] = ...
 *    ic_microstrip_syn(z0,elen,w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f,flag);
 */
static void exec_ic_microstrip_syn(double *args)
{
  /* our ic_microstrip for calculations */
  ic_microstrip_line *line;
  int flag;
  int i = 0;

  /* create the line and fill in the parameters */
  line = ic_microstrip_line_new();
  line->Xo          = 0.0;
  line->Ro          = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->l           = args[i++];
  line->subs->tox   = args[i++];
  line->subs->eox   = args[i++];
  line->subs->h     = args[i++];
  line->subs->es    = args[i++];
  line->subs->sigmas= args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->freq        = args[i++];
  flag              = args[i++];

  /* run the calculation */
  if( ic_microstrip_syn(line, line->freq, flag) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g\n",
	   line->w, line->subs->h, line->subs->tox, line->l);
  }

  /* clean up */
  ic_microstrip_line_free(line);
  
  return;
}

/* 
 * [z0,keff,elen,loss,L,R,C,G,lc,ld,deltal,depth] =
 *   microstrip_calc(w,h,l,tmet,rho,rough,er,tand,f);
 */
static void exec_microstrip_calc(double *args)
{
  /* our microstrip for calculations */
  microstrip_line *line;
  int i = 0;

  /* create the line and fill in the parameters */
  line = microstrip_line_new();
  line->w           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( microstrip_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g %g %g %g %g %g %g %g %g %g \n", line->z0, line->keff,
	   line->len, line->loss, line->Ls, line->Rs, line->Cs, line->Gs,
      line->alpha_c, line->alpha_d, line->deltal, line->skindepth);
  }

  /* clean up */
  microstrip_line_free(line);
  
  return;
}

/* 
 * [w_out,h_out,l_out,er_out] = 
 *    microstrip_syn(z0,elen,w,h,l,tmet,rho,rough,er,tand,f,flag);
 */
static void exec_microstrip_syn(double *args)
{
  /* our microstrip for calculations */
  microstrip_line *line;
  int flag;
  int i = 0;

  /* create the line and fill in the parameters */
  line = microstrip_line_new();
  line->Xo          = 0.0;
  line->Ro = line->z0 = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];
  flag              = args[i++];

  /* run the calculation */
  if( microstrip_syn(line, line->freq, flag) ) {
    printf("%s", ERRMSG);
  } else { 
  
    /* print the outputs */
    printf("%g %g %g %g\n", line->w, line->subs->h,
	   line->l, line->subs->er);
  }

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
  int i = 0;

  /* create the line and fill in the parameters */
  line = stripline_line_new();
  line->w           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];

  /* run the calculation */
  if( stripline_calc(line, line->freq) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g\n", line->z0, line->loss, line->deltal);
  }

  /* clean up */
  stripline_line_free(line);
  
  return;
}

/* 
 * [w_out,h_out,l_out,er_out] = 
 *    stripline_syn(z0,elen,w,h,l,tmet,rho,rough,er,tand,f,flag);
 */
static void exec_stripline_syn(double *args)
{
  /* our stripline for calculations */
  stripline_line *line;
  int flag;
  int i = 0;

  /* create the line and fill in the parameters */
  line = stripline_line_new();
  line->Xo          = 0.0;
  line->Ro = line->z0 = args[i++];
  line->len         = args[i++];
  line->w           = args[i++];
  line->subs->h     = args[i++];
  line->l           = args[i++];
  line->subs->tmet  = args[i++];
  line->subs->rho   = args[i++];
  line->subs->rough = args[i++];
  line->subs->er    = args[i++];
  line->subs->tand  = args[i++];
  line->freq        = args[i++];
  flag              = args[i++];

  /* run the calculation */
  printf("Calling stripline syn with z0 = %g\n", line->z0);
  if( stripline_syn(line, line->freq, flag) ) {
    printf("%s", ERRMSG);
  } else { 
    /* print the outputs */
    printf("%g %g %g %g\n", line->w, line->subs->h, line->l, line->subs->er);
  }

  /* clean up */
  stripline_line_free(line);
  
  return;
}

/* 
 * help
 */
static void exec_help(double *args)
{
  printf("Sorry.  There is no online help.\n"
	"Try \"man stdio-wcalc\"\n\n");

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


