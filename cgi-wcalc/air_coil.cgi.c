/* $Id: air_coil.cgi.c,v 1.21 2004/08/31 10:25:28 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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

/*
 * a cgi interface to the air_coil calculator
 */

/* #define DEBUG */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

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



/* defaults for the various parameters (in MKS) */
#define defN      4
#define defAWG    22
#define defRHO    1.72e-8
#define defDIA    INCH2M(0.25)
#define defLEN    INCH2M(0.50)
#define defFILL   2.0
#define defFREQ   100.0e6
#define defIND    100.0e-9

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
    if(cgiFormDouble("N", &N, defN) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading number of turns");
    }
    if( N < 1.0 ) {
      N = defN;
      printFormError("Number of turns must be &gt = 1");
    }
    
    /* wire size */
    if(cgiFormDoubleBounded("AWG", &AWG, 1.0, 60.0, defAWG) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Wire size must be in the range 1 <= AWG <= 60");
    }
    
    /* Metal resistivity */
    if(cgiFormDouble("rho", &rho, defRHO/coil->units_rho->sf) 
       != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading resistivity");
    }
    if( rho < 0.0 ) {
      rho = defRHO/coil->units_rho->sf;
      printFormError("Conductor resistivity may not be negative");
    }
    
    /* inside diameter */
    if(cgiFormDouble("dia", &dia, defDIA/coil->units_dia->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading diameter");
    }
    if( dia < 0.0 ) {
      dia = defDIA/coil->units_dia->sf;
      printFormError("Coil diameter may not be negative");
    }
    
    
    /* Solenoid length  */
    if(cgiFormDouble("len", &len, defLEN/coil->units_dia->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading length");
    }
    if( len < 0.0 ) {
      len = defLEN/coil->units_dia->sf;
      printFormError("Coil length may not be negative");
    }    

    /* Solenoid fill  */
    if(cgiFormDoubleBounded("fill", &coil->fill, 1.0, 10.0, defFILL) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Fill out of range.  Must be between 1 and 10");
    }

    if (cgiFormRadio("use_fill", fill_choices, 2, &coil->use_fill, 0) !=
	cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading fill/len radio button");
    }
    
    /* Frequency of operation  */
    if(cgiFormDouble("freq", &freq, defFREQ/coil->units_freq->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading frequency");
    }
    if( freq < 0.0 ) {
      freq = defFREQ/coil->units_freq->sf;
      printFormError("Frequency may not be negative");
    }    

    /* Desired Inductance */
    if(cgiFormDouble("L", &L, defIND/coil->units_L->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading inductance");
    }
    if( L <= 0.0 ) {
      L = defIND/coil->units_L->sf;
      printFormError("Inductance must be &gt 0");
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
    printf(_("%s:  checking for a cookie to load\n"), name_string);
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
      printf(_("%s:  loading cookie \"%s\"\n"), name_string, cookie_load_str);
#endif
      air_coil_load_string(coil, cookie_load_str);
#ifdef DEBUG
      printf(_("%s:  finished loading cookie\n"),name_string);
#endif
    }
  }

  if (!input_err){
    cookie_str = air_coil_save_string(coil);
    cookie = cgiCookie_new(name_string, cookie_str);
    cgiCookie_MaxAge_set(cookie, COOKIE_AGE);
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

