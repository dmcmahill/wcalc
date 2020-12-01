/*
 * Copyright (C) 2009, 2020 Dan McMahill
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
 * a cgi interface to the rods calculator
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
#include "rods.h"
#include "rods_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD         0
#define RESET        1
#define ANALYZE      2


/* defaults for the various parameters (in MKS) */
#define defD1     1e-3
#define defL1     10e-3

#define defD2     defD1
#define defL2     defL1

#define defDISTANCE 3e-3
#define defOFFSET 0

#define defRHO   1.72e-8
#define defFREQ   100.0e6


static const char *name_string="rods.cgi";
static int input_err;

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;

  /* rods variables */
  rods *rod;
  double rho, freq;

  double d1, l1, d2, l2, distance, offset;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_xy;
  cgi_units_menu *menu_L;
  cgi_units_menu *menu_R;
  cgi_units_menu *menu_rho;
  cgi_units_menu *menu_freq;
  
  input_err = 0;

  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the rods */
  rod = rods_new();

  menu_xy = cgi_units_menu_new(rod->units_xy);
  menu_L = cgi_units_menu_new(rod->units_L);
  menu_R = cgi_units_menu_new(rod->units_R);
  menu_rho = cgi_units_menu_new(rod->units_rho);
  menu_freq = cgi_units_menu_new(rod->units_freq);

  cgi_units_attach_entry(menu_xy, "entry_l1");

  cgi_units_attach_entry(menu_xy, "entry_d2");
  cgi_units_attach_entry(menu_xy, "entry_l2");

  cgi_units_attach_entry(menu_xy, "entry_distance");
  cgi_units_attach_entry(menu_xy, "entry_offset");

  cgi_units_attach_entry(menu_L, "entry_L2");
  cgi_units_attach_entry(menu_L, "entry_M");
  cgi_units_attach_entry(menu_R, "entry_R2");

  /* force d2 = d1 and l2 = l1 */
  cgi_sync_entry("d1", "d2");
  cgi_sync_entry("d2", "d1");

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
     * the rods structure
     */
    cgi_units_menu_read();
    

    /* Diameter of wire #1 */
    if(cgiFormDouble("d1", &d1, defD1/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading width of wire #1");
    }
    if( d1 < 0.0 ) {
      d1 = defD1/rod->units_xy->sf;
      printFormError("Diameter of wire must be &gt = 0");
    }
    
    /* Length of wire #1 */
    if(cgiFormDouble("l1", &l1, defL1/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading length of wire #1");
    }
    if( l1 <= 0.0 ) {
      l1 = defL1/rod->units_xy->sf;
      printFormError("Length of wire must be &gt 0");
    }
   
    /* Diameter of wire #2 */
    if(cgiFormDouble("d2", &d2, defD2/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading diameter of wire #2");
    }
    if( d2 < 0.0 ) {
      d2 = defD2/rod->units_xy->sf;
      printFormError("Diameter of wire must be &gt = 0");
    }
    
    /* Length of wire #2 */
    if(cgiFormDouble("l2", &l2, defL2/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading length of rod #2");
    }
    if( l2 <= 0.0 ) {
      l2 = defL2/rod->units_xy->sf;
      printFormError("Length of wire must be &gt 0");
    }
    
    /* Axial offset between ends */
    if(cgiFormDouble("offset", &offset, defOFFSET/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading axial offset");
    }

    /* Radial distance between centers */
    if(cgiFormDouble("distance", &distance, defDISTANCE/rod->units_xy->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading radial distance between centers");
    }
    if( distance <= 0.5*(d1+d2) ) {
      distance = (d1+d2)/rod->units_xy->sf;
      printFormError("Wires may not touch or have a negative distance");
    }
    
    
    /* Resistivity  */
    if(cgiFormDouble("rho", &rho, defRHO/rod->units_rho->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading resistivity");
    }
    if( rho < 0.0 ) {
      rho = defRHO/rod->units_rho->sf;
      printFormError("Resistivity may not be negative");
    }

    /* Frequency of operation  */
    if(cgiFormDouble("freq", &freq, defFREQ/rod->units_freq->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading frequency");
    }
    if( freq < 0.0 ) {
      freq = defFREQ/rod->units_freq->sf;
      printFormError("Frequency may not be negative");
    }


    /* copy data over to the rods structure */
    rod->d1  = d1 * rod->units_xy->sf;
    rod->l1  = l1 * rod->units_xy->sf;

    rod->d2  = d2 * rod->units_xy->sf;
    rod->l2  = l2 * rod->units_xy->sf;

    rod->distance  = distance * rod->units_xy->sf;
    rod->offset    = offset * rod->units_xy->sf;

    rod->rho = rho*rod->units_rho->sf;
    rod->freq = freq*rod->units_freq->sf;
    
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
      rods_load_string(rod, cookie_load_str);
#ifdef DEBUG
      printf(_("%s:  finished loading cookie\n"),name_string);
#endif
    }
  }

  if (!input_err){
    cookie_str = rods_save_string(rod);
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
	    rod->a/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 1 thickness       = %g %s\n",
	    rod->b/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 1 length          = %g %s\n",
	    rod->l1/rod->units_xy->sf, rod->units_xy->name);

    fprintf(cgiOut,"CGI: Bar 2 width           = %g %s\n",
	    rod->d/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 thickness       = %g %s\n",
	    rod->c/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 length          = %g %s\n",
	    rod->l2/rod->units_xy->sf, rod->units_xy->name);

    fprintf(cgiOut,"CGI: Bar 2 width position  = %g %s\n",
	    rod->E/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 thickness pos.  = %g %s\n",
	    rod->P/rod->units_xy->sf, rod->units_xy->name);
    fprintf(cgiOut,"CGI: Bar 2 length position = %g %s\n",
	    rod->l3/rod->units_xy->sf, rod->units_xy->name);

    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    rod->freq/rod->units_freq->sf, rod->units_freq->name); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif


  switch (action){
  case ANALYZE:
  case LOAD:
  case RESET:
    /* 
     * in case rods_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    rods_calc(rod, rod->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  default:
    fprintf(stderr, "rods.cgi:  unknown action (%d)\n", action);
    exit(1);
    break;
  }

  /* include the HTML output */
#include "header_html.c"
#include "rods_html.c"
#include "footer_html.c"
	
  return 0;
}

