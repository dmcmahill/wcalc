/* $Id: stripline.cgi.c,v 1.11 2005/10/13 22:40:10 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004, 2005 Dan McMahill
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
 * a cgi interface to the stripline calculator
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
#include "stripline.h"
#include "stripline_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "stripline_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD      0
#define RESET     1
#define ANALYZE   2
#define SYNTH_H   3
#define SYNTH_W   4
#define SYNTH_ES  5
#define SYNTH_L   6



/* defaults for the various parameters */
#define defW      MIL2M(110.0)
#define defL      MIL2M(1000.0)

#define defTMET   MIL2M(1.4)
#define defRGH    MIL2M(0.05)
#define defRHO    1.72e-8

#define defH      MIL2M(62.0)
#define defES     4.8
#define defTAND   0.01

#define defFREQ   1000.0e9

#define defRO     50.0
#define defELEN   90.0

static const char *name_string="stripline.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* stripline variables */
  stripline_line *line;
  double freq;

  double rho,rough,tmet,w,l,h,es,tand;
  double Ro=0.0;
  double elen;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_lwht;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;
  cgi_units_menu *menu_len, *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal;


  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */
  
  /* create the stripline line */
  line = stripline_line_new();

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

  cgi_units_attach_entry(menu_lwht, "entry_len2");
  cgi_units_attach_entry(menu_lwht, "entry_t");
  cgi_units_attach_entry(menu_lwht, "entry_h");



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
  else if(cgiFormStringNoNewlines("synth_es",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_ES;
  }
  else if(cgiFormStringNoNewlines("synth_l",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_L;
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
   * the stripline structure
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
    
    /* trace width */
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
    if(cgiFormDouble("l", &l, defL/line->units_len->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading trace length");
    }
    if( l <= 0.0 ) {
      l = defL/line->units_len->sf;
      printFormError("Metal length must be %gt 0");
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

    /* Substrate loss tangent */
    if(cgiFormDoubleBounded("tand", &tand, 0.0, 1000.0, defTAND) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Loss tangent out of range");
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
    if(cgiFormDoubleBounded("Ro", &Ro, 0.0001, 1000.0, defRO) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Z0 out of range");
    }

    if(cgiFormDouble("elen", &elen, defELEN) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading electrical length");
    }
    if( freq <= 0.0 ) {
      elen = defELEN;
      printFormError("Electrical length must be %gt 0");
    }

  /* copy data over to the line structure */
  line->w           = w*line->units_lwht->sf;
  line->l           = l*line->units_len->sf;
  line->subs->h     = h*line->units_lwht->sf;
  line->subs->tmet  = tmet*line->units_lwht->sf;
  line->subs->rough = rough*line->units_rough->sf;


  line->freq = freq * line->units_freq->sf;
  
  /* copy over the other parameters */
  line->subs->tand  = tand;
  line->subs->er    = es;
  line->subs->rho   = rho*line->units_rho->sf;

  line->z0 = Ro;
  line->Ro = Ro;
  line->Xo = 0.0;
  line->len = elen;

  } /* if ( (action != RESET) && (action != LOAD) ) */
  else {

#ifdef DEBUG
    printf("stripline.cgi:  checking for a cookie to load\n");
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
    printf("stripline.cgi:  loading cookie \"%s\"\n", cookie_load_str);
#endif
      stripline_load_string(line, cookie_load_str);
#ifdef DEBUG
    printf("stripline.cgi:  finished loading cookie\n");
#endif
    }
    
  }
  
  if (!input_err){
    cookie_str = stripline_save_string(line);
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
    fprintf(cgiOut,"CGI: --------------- Stripline  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Action                      = %d\n",action);
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"CGI: Metal width                 = %g %s\n",
	    line->w/line->units_lwht->sf, line->units_lwht->name);
    fprintf(cgiOut,"CGI: Metal length                = %g %s\n",
	    line->l/line->units_len->sf, line->units_len->name);
    fprintf(cgiOut,"CGI: Metal thickness             = %g %s\n",
	    line->subs->tmet/line->units_lwht->sf, line->units_lwht->name);
    fprintf(cgiOut,"CGI: Metal resistivity           = %g %s\n",
	    line->subs->rho/line->units_rho->sf, line->units_rho->name);
    fprintf(cgiOut,"CGI: Metal surface roughness     = %g %s-rms\n",
	    line->subs->rough/line->units_rough->sf, line->units_rough->name);
    fprintf(cgiOut,"CGI: Substrate thickness         = %g %s\n",
	    line->subs->h/line->units_lwht->sf, line->units_lwht->name);
    fprintf(cgiOut,"CGI: Substrate dielectric const. = %g \n", line->subs->er);
    fprintf(cgiOut,"CGI: Substrate loss tangent      = %g \n", line->subs->tand);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    line->freq/line->units_freq->sf, line->units_freq->name); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif
  
  switch (action){
  case ANALYZE:

    /* 
     * in case stripline_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    stripline_calc(line, line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line, line->freq, SLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line, line->freq, SLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line, line->freq, SLISYN_ER);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line, line->freq, SLISYN_L);
    fprintf(cgiOut,"</pre>\n");
    break;

  }


  /* autoscale the delay output */
  /* XXX */

  /* include the HTML output */
#include "header_html.c"
#include "stripline_html.c"
#include "footer_html.c"
	
  return 0;
}

