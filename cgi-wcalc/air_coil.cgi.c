/* $Id: air_coil.cgi.c,v 1.16 2004/07/28 04:46:53 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004 Dan McMahill
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

/*
 * a cgi interface to the air_coil calculator
 */

/* #define DEBUG */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cgi-common.h"
#include "cgi-units.h"
#include "cookie.h"

/* libwcalc */
#include "air_coil.h"
#include "air_coil_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "air_coil_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD         0
#define RESET        1
#define ANALYZE      2
#define SYNTH_NMIN   3
#define SYNTH_NFIX   4



/* defaults for the various parameters */
#define defN      4
#define defAWG    22
#define defRHO    1.0
#define defDIA    0.25
#define defLEN    0.50
#define defFILL   2.0
#define defFREQ   10.0
#define defIND    100.0

static const char *name_string="air_coil.cgi";
static int input_err;

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;

  /* air_coil variables */
  air_coil_coil *coil;
  double freq;

  double N,AWG,rho,dia,len,L;

  char *fill_choices[] = {"no","yes"};

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_len, *menu_dia;
  cgi_units_menu *menu_L, *menu_SRF;
  cgi_units_menu *menu_rho;
  cgi_units_menu *menu_freq;
  
  input_err = 0;

  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the air_coil  */
  coil = air_coil_new();

  menu_len = cgi_units_menu_new(coil->units_len);
  menu_dia = cgi_units_menu_new(coil->units_dia);
  menu_L = cgi_units_menu_new(coil->units_L);
  menu_SRF = cgi_units_menu_new(coil->units_SRF);
  menu_rho = cgi_units_menu_new(coil->units_rho);
  menu_freq = cgi_units_menu_new(coil->units_freq);

  cgi_units_attach_entry(menu_freq, "entry_Qfreq");
  cgi_units_attach_entry(menu_L, "entry_Lmax");


  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_n",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_NMIN;
  }
  else if(cgiFormStringNoNewlines("synth_nfix",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_NFIX;
  }
  else if(cgiFormStringNoNewlines("reset",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = RESET;
  }
  else{
    action = LOAD;
  }

  
  if ( (action != RESET) && (action != LOAD) ) {

    /*
     * extract the parameters from the CGI form and use them to populate
     * the air_coil structure
     */
    cgi_units_menu_read();
    

    /* Number of turns */
    if(cgiFormDoubleBounded("N",&N,0.0001,1000.0,defN) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }
    
    /* wire size */
    if(cgiFormDoubleBounded("AWG",&AWG,0.0001,1000.0,defAWG) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }
    
    /* Metal resistivity relative to copper */
    if(cgiFormDoubleBounded("rho",&rho,0.0001,1000.0,defRHO) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }
    
    /* inside diameter */
    if(cgiFormDoubleBounded("dia",&dia,0.0001,1000.0,defDIA) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }
    
    
    /* Solenoid length  */
    if(cgiFormDoubleBounded("len",&len,0.0001,1000.0,defLEN) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }
    

    /* Solenoid fill  */
    if(cgiFormDoubleBounded("fill",&coil->fill,1.0,10.0,defFILL) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }

    if (cgiFormRadio("use_fill",fill_choices,2,&coil->use_fill,0) !=
	cgiFormSuccess){
      inputErr(&input_err);
    }
    
    /* Frequency of operation  */
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }

    /* Desired Inductance */
    if(cgiFormDoubleBounded("L",&L,0.0001,1000.0,defIND) !=
       cgiFormSuccess){
      inputErr(&input_err);
    }


    /* copy data over to the coil structure */
    coil->Nf   = N;
    coil->AWGf = AWG;
    coil->rho  = rho*coil->units_rho->sf;
    coil->dia  = dia*coil->units_dia->sf;
    coil->len  = len*coil->units_len->sf;
    coil->L    = L*coil->units_L->sf;
    coil->freq = freq*coil->units_freq->sf;
    
  }  /* if ( (action != RESET) && (action != LOAD) ) */
  else {
#ifdef DEBUG
    printf(_("%s:  checking for a cookie to load\n"),name_string);
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string,cookie_load_str,COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
      printf(_("%s:  loading cookie \"%s\"\n"),name_string,cookie_load_str);
#endif
      air_coil_load_string(coil,cookie_load_str);
#ifdef DEBUG
      printf(_("%s:  finished loading cookie\n"),name_string);
#endif
    }
  }
  if (!input_err){
    cookie_str = air_coil_save_string(coil);
    cookie = cgiCookie_new(name_string,cookie_str);
    cgiCookie_MaxAge_set(cookie,COOKIE_AGE);
    cgiHeaderSetCookie(cookie);
    
    /* Put out the CGI header */
    cgiHeaderContentType("text/html");  
  }
  else {
    fixInputMsg();
  }


#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- ----- Air Coil ----- -----------\n");
    fprintf(cgiOut,"CGI: action = %d\n",action);
    fprintf(cgiOut,"CGI: --------------- -------------------- -----------\n");
    fprintf(cgiOut,"CGI: Number of turns             = %g \n",
	    coil->Nf);
    fprintf(cgiOut,"CGI: Wire Size                   = %g AWG\n",
	    coil->AWGf);
    fprintf(cgiOut,"CGI: Metal resistivity           = %g %s\n",
	    coil->rho/coil->units_rho->sf, coil->units_rho->name);
    fprintf(cgiOut,"CGI: Inside Diameter             = %g %s\n",
	    coil->dia/coil->units_dia->sf, coil->units_dia->name);
    fprintf(cgiOut,"CGI: Solenoid length             = %g %s\n",
	    coil->len/coil->units_len->sf, coil->units_len->name);
    fprintf(cgiOut,"CGI: Solenoid fill               = %g \n",
	    coil->fill);
    fprintf(cgiOut,"CGI: use_fill                    = %d \n",
	    coil->use_fill);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    coil->freq/coil->units_freq->sf, coil->units_freq->name); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif


  switch (action){
  case ANALYZE:
  case LOAD:
  case RESET:
    /* 
     * in case air_coil_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    air_coil_calc(coil, coil->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_NMIN:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil, coil->freq, AIRCOILSYN_NMIN);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_NFIX:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil, coil->freq, AIRCOILSYN_NFIX);
    fprintf(cgiOut,"</pre>\n");
    break;

  default:
    fprintf(stderr, "air_coil.cgi:  unknown action (%d)\n", action);
    exit(1);
    break;
  }

  /* autoscale the SRF output */
  /* XXX */

  /* include the HTML output */
#include "header_html.c"
#include "air_coil_html.c"
#include "footer_html.c"
	
  return 0;
}

