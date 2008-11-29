/* $Id: ic_microstrip.cgi.c,v 1.10 2006/03/01 15:31:36 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004, 2006 Dan McMahill
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
 * a cgi interface to the ic_microstrip calculator
 */

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
#include "ic_microstrip.h"
#include "ic_microstrip_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "ic_microstrip_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

#define LOAD      0
#define ANALYZE   1
#define SYNTH_H   2
#define SYNTH_W   3
#define SYNTH_TOX 4
#define RESET     5



/* defaults for the various parameters */
#define defW      160.0e-6
#define defL      1000.0e-6

#define defTMET   1.6e-6
#define defRGH    0.01e-6
#define defRHO    1.7e-8

#define defTOX    1.0e-6
#define defEOX    4.0

#define defH      250.0e-6
#define defES     11.8
#define defSIGMAS 1.0

#define defFREQ   1000.0e6

#define defR0     50.0
#define defELEN   90.0

static const char *name_string="ic_microstrip.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* microstrip variables */
  ic_microstrip_line *line;
  double freq;

  double rho, rough, tmet, w, l, tox, eox, h, es, sigmas;
  double Ro=0.0;
  double len;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_lwht;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;
  cgi_units_menu *menu_len, *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal, *menu_sigmas;

  /*
   * uncomment to be able to run in the debugger.
   * access the CGI URL that gives the problem, then change foo.cgi to 
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the IC microstrip line */
  line = ic_microstrip_line_new();
  
  menu_lwht = cgi_units_menu_new(line->units_lwht);
  menu_L = cgi_units_menu_new(line->units_L);
  menu_R = cgi_units_menu_new(line->units_R);
  menu_C = cgi_units_menu_new(line->units_C);
  menu_G = cgi_units_menu_new(line->units_G);
  menu_len = cgi_units_menu_new(line->units_len);
  menu_freq = cgi_units_menu_new(line->units_freq);
  menu_loss = cgi_units_menu_new(line->units_loss);
  menu_losslen = cgi_units_menu_new(line->units_losslen);
  menu_rho = cgi_units_menu_new(line->units_rho);
  menu_rough = cgi_units_menu_new(line->units_rough);
  menu_delay = cgi_units_menu_new(line->units_delay);
  menu_depth = cgi_units_menu_new(line->units_depth);
  menu_deltal = cgi_units_menu_new(line->units_deltal);
  menu_sigmas = cgi_units_menu_new(line->units_sigmas);

  cgi_units_attach_entry(menu_lwht, "entry_l");
  cgi_units_attach_entry(menu_lwht, "entry_h");
  cgi_units_attach_entry(menu_lwht, "entry_tox");
  cgi_units_attach_entry(menu_lwht, "entry_tmet");

  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_w",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_W;
  }
  else if(cgiFormStringNoNewlines("synth_h",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_H;
  }
  else if(cgiFormStringNoNewlines("synth_tox",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_TOX;
  }
  else if(cgiFormStringNoNewlines("reset",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = LOAD;
  }
  else{
    action = LOAD;
  }

  /*
   * extract the parameters from the CGI form and use them to populate
   * the microstrip structure
   */

  if ( (action != RESET) && (action != LOAD) ) {

    cgi_units_menu_read();

    /* Metal resistivity */
    if(cgiFormDouble("rho", &rho, defRHO/line->units_rho->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading resistivity");
    }
    if( rho < 0.0 ) {
      rho = defRHO/line->units_rho->sf;
      printFormError("Resistivity may not be negative");
    }

    /* Metal thickness (m) */
    if(cgiFormDouble("tmet", &tmet, defTMET/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal thickness");
    }
    if( tmet < 0.0 ) {
      tmet = defTMET/line->units_lwht->sf;
      printFormError("Metal thickness may not be negative");
    }

    /* Metalization roughness */
    if(cgiFormDouble("rough", &rough, defRGH/line->units_rough->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal surface roughness");
    }
    if( rough < 0.0 ) {
      rough = defRGH/line->units_rough->sf;
      printFormError("Metal surface roughness may not be negative");
    }
    
    /* Microstrip width */
    if(cgiFormDouble("w", &w, defW/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal width");
    }
    if( w <= 0.0 ) {
      w = defW/line->units_lwht->sf;
      printFormError("Metal width must be %gt 0");
    }


    /* trace length */
    if(cgiFormDouble("l", &l, defL/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading trace length");
    }
    if( l <= 0.0 ) {
      l = defL/line->units_lwht->sf;
      printFormError("Metal length must be %gt 0");
    }

    /* Oxide thickness */
    if(cgiFormDouble("tox", &tox, defTOX/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading oxide thickness");
    }
    if( tox <= 0.0 ) {
      h = defTOX/line->units_lwht->sf;
      printFormError("Oxide thickness must be %gt 0");
    }

    /* Oxide relative permittivity */
    if(cgiFormDoubleBounded("eox", &eox, 1.0, 1000.0, defEOX) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Oxide permitivity out of range");
    }

    /* Substrate dielectric thickness */
    if(cgiFormDouble("h", &h, defH/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading substrate height");
    }
    if( h <= 0.0 ) {
      h = defH/line->units_lwht->sf;
      printFormError("Substrate height must be %gt 0");
    }

    /* Substrate relative permittivity */
    if(cgiFormDoubleBounded("es", &es, 1.0, 1000.0, defES) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Substrate permitivity out of range");
    }

    /* Substrate conductivity s/m*/
    if(cgiFormDouble("sigmas", &sigmas, defSIGMAS) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading substrate conductivity");
    }
    if( sigmas <= 0.0 ) {
      sigmas = defSIGMAS/line->units_sigmas->sf;
      printFormError("Substrate conductivity must be %gt 0");
    }


    /* Frequency of operation */
    if(cgiFormDouble("freq", &freq, defFREQ/line->units_freq->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading frequency");
    }
    if( freq <= 0.0 ) {
      freq = defFREQ/line->units_freq->sf;
      printFormError("Frequency must be %gt 0");
    }

    /* electrical parameters: */
    if(cgiFormDoubleBounded("Ro", &Ro, 0.0001, 1000.0, defR0) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Z0 out of range");
    }

    if(cgiFormDouble("elen", &len, defELEN) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Electrical length");
    }
    if( len <= 0.0 ) {
      len = defELEN;
      printFormError("Electrical length must be %gt 0");
    }

    /* copy data over to the line structure */
    line->w           = w*line->units_lwht->sf;
    line->l           = l*line->units_lwht->sf;
    line->subs->h     = h*line->units_lwht->sf;
    line->subs->tox   = tox*line->units_lwht->sf;
    line->subs->tmet  = tmet*line->units_lwht->sf;
    line->subs->rough = rough*line->units_rough->sf;


    line->freq = freq * line->units_freq->sf;
  
    /* copy over the other parameters */
    line->subs->sigmas = sigmas*line->units_sigmas->sf;
    line->subs->eox    = eox;
    line->subs->es     = es;
    line->subs->rho    = rho*line->units_rho->sf;

    line->Ro = Ro;
    line->Xo = 0.0;
    line->len = len;

  } /* if ( (action != RESET) && (action != LOAD) ) */
  else {

#ifdef DEBUG
    printf("ic_microstrip.cgi:  checking for a cookie to load\n");
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
    printf("ic_microstrip.cgi:  loading cookie \"%s\"\n", cookie_load_str);
#endif
      ic_microstrip_load_string(line, cookie_load_str);
#ifdef DEBUG
    printf("ic_microstrip.cgi:  finished loading cookie\n");
#endif
    }
    
  }
  
  if (!input_err){
    cookie_str = ic_microstrip_save_string(line);
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
  fprintf(cgiOut,"-------------- IC Microstrip Analysis ----------\n");
  fprintf(cgiOut,"Metal width                 = %g %s\n",
	    line->w/line->units_lwht->sf, line->units_lwht->name);
  fprintf(cgiOut,"Oxide thickness             = %g %s\n",
	  line->tox/line->units_lwht->sf, line->units_lwht->name);
  fprintf(cgiOut,"Oxide dielectric const.     = %g \n",
	  line->subs->eox);
  fprintf(cgiOut,"Substrate thickness         = %g um\n",
	  line->subs->h/line->units_lwht->sf, line->units_lwht->name);
  fprintf(cgiOut,"Substrate dielectric const. = %g \n",
	  line->subs->es);
  fprintf(cgiOut,"Substrate conductivity      = %g %s\n",
	  line->subs->sigmas/line->units_sigmas->sf, line->units_sigmas->name);
  fprintf(cgiOut,"Substrate resistivity       = %g 1.0/%s\n",
	  1.0(line->subs->sigmas/line->units_sigmas->sf), line->units_sigmas->name);
  fprintf(cgiOut,"Frequency                   = %g %s\n",
	    line->freq/line->units_freq->sf, line->units_freq->name);
  fprintf(cgiOut,"-------------- ---------------------- ----------\n");
  fprintf(cgiOut,"</pre>\n");
#endif

  switch (action){
  case ANALYZE:
    /* 
     * in case ic_microstrip_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    ic_microstrip_calc(line, line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line, line->freq, IC_MLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line, line->freq, IC_MLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_TOX:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line, line->freq, IC_MLISYN_TOX);
    fprintf(cgiOut,"</pre>\n");
    break;

  }

  /* include the HTML output */
#include "header_html.c"
#include "ic_microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

