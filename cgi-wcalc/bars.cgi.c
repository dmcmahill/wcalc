/* $Id: air_coil.cgi.c,v 1.22 2008/11/29 20:41:31 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
 * All rights reserved.
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
 * a cgi interface to the bars calculator
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
#include "bars.h"
#include "bars_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "bars_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD         0
#define RESET        1
#define ANALYZE      2


/* defaults for the various parameters (in MKS) */
#define defA      10e-6
#define defB      1e-6
#define defL1     1e-3

#define defD      10e-6
#define defC      1e-6
#define defL2     1e-3

#define defE      12e-6
#define defP      0
#define defL3     0

#define defFREQ   100.0e6


static const char *name_string="bars.cgi";
static int input_err;

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;

  /* bars variables */
  bars *bar;
  double freq;

  double a, b, l1, d, c, l2, E, P, l3, L1, L2, M, k;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_xy;
  cgi_units_menu *menu_L;
  cgi_units_menu *menu_freq;
  
  input_err = 0;

  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the bars */
  bar = bars_new();

  menu_xy = cgi_units_menu_new(bar->units_xy);
  menu_L = cgi_units_menu_new(coil->units_L);
  menu_freq = cgi_units_menu_new(coil->units_freq);

  cgi_units_attach_entry(menu_xy, "entry_b");
  cgi_units_attach_entry(menu_xy, "entry_l1");

  cgi_units_attach_entry(menu_xy, "entry_d");
  cgi_units_attach_entry(menu_xy, "entry_c");
  cgi_units_attach_entry(menu_xy, "entry_l2");

  cgi_units_attach_entry(menu_xy, "entry_E");
  cgi_units_attach_entry(menu_xy, "entry_P");
  cgi_units_attach_entry(menu_xy, "entry_l3");

  cgi_units_attach_entry(menu_L, "entry_L2");
  cgi_units_attach_entry(menu_L, "entry_M");


  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
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
     * the bars structure
     */
    cgi_units_menu_read();
    

    /* Number of turns */
    if(cgiFormDouble("a", &a, defA/bar->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading width of bar #1");
    }
    if( a < 0.0 ) {
      a = defA/bar->units_xy->sf;
      printFormError("Width of bar must be &gt = 0");
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


    /* copy data over to the coil structure */
    bar->a   = a * bar->units_xy->sf;
    bar->b   = b * bar->units_xy->sf;
    bar->l1  = l1 * bar->units_xy->sf;

    bar->d   = d * bar->units_xy->sf;
    bar->c   = c * bar->units_xy->sf;
    bar->l2  = l2 * bar->units_xy->sf;

    bar->E   = E * bar->units_xy->sf;
    bar->P   = P * bar->units_xy->sf;
    bar->l3  = l3 * bar->units_xy->sf;

    bar->freq = freq*bar->units_freq->sf;
    
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
    cookie_str = bars_save_string(bar);
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
    fprintf(cgiOut,"CGI: --------------- ----- - Bars - ----- -----------\n");
    fprintf(cgiOut,"CGI: action = %d\n",action);
    fprintf(cgiOut,"CGI: --------------- -------------------- -----------\n");
    fprintf(cgiOut,"CGI: Bar 1 width           = %g %s\n",
	    bar->a/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 1 thickness       = %g %s\n",
	    bar->b/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 1 length          = %g %s\n",
	    bar->l1/bar->units_xy->sf, bar->units_xy->name);

    fprintf(cgiOut,"CGI: Bar 2 width           = %g %s\n",
	    bar->d/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 thickness       = %g %s\n",
	    bar->c/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 length          = %g %s\n",
	    bar->l2/bar->units_xy->sf, bar->units_xy->name);

    fprintf(cgiOut,"CGI: Bar 2 width position  = %g %s\n",
	    bar->E/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 thickness pos.  = %g %s\n",
	    bar->P/bar->units_xy->sf, bar->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 length position = %g %s\n",
	    bar->l3/bar->units_xy->sf, bar->units_xy->name);

    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    bar->freq/bar->units_freq->sf, bar->units_freq->name); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif


  switch (action){
  case ANALYZE:
  case LOAD:
  case RESET:
    /* 
     * in case bars_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    bars_calc(bar, bar->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  default:
    fprintf(stderr, "bars.cgi:  unknown action (%d)\n", action);
    exit(1);
    break;
  }

  /* include the HTML output */
#include "header_html.c"
#include "bars_html.c"
#include "footer_html.c"
	
  return 0;
}

