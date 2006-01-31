/* $Id: coplanar.cgi.c,v 1.10 2005/10/13 22:40:09 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004, 2005, 2006 Dan McMahill
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
 * a cgi interface to the coplanar waveguide calculator
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
#include "coplanar.h"
#include "coplanar_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "coplanar_id.h"

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
#define defS      MIL2M(50.0)
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

static const char *name_string="coplanar.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* coplanar variables */
  coplanar_line *line;
  double freq;

  double rho, rough, tmet, w, s, l, h, es, tand;
  double Ro = 0.0;
  double elen;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_lwht;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;
  cgi_units_menu *menu_len, *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal;


  /* create the coplanar line */
  line = coplanar_line_new();

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
  cgi_units_attach_entry(menu_lwht, "entry_s");
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
  else if(cgiFormStringNoNewlines("synth_s", str_action, ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_S;
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
   * the coplanar structure
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
    
    /* trace spacing */
    if(cgiFormDouble("s", &w, defS/line->units_lwht->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal spacing");
    }
    if( s <= 0.0 ) {
      s = defS/line->units_lwht->sf;
      printFormError("Metal spacing must be %gt 0");
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
    line->s           = s*line->units_lwht->sf;
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
    printf("coplanar.cgi:  checking for a cookie to load\n");
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
    printf("coplanar.cgi:  loading cookie \"%s\"\n", cookie_load_str);
#endif
      coplanar_load_string(line, cookie_load_str);
#ifdef DEBUG
    printf("coplanar.cgi:  finished loading cookie\n");
#endif
    }
    
  }
  
  if (!input_err){
    cookie_str = coplanar_save_string(line);
    cookie = cgiCookie_new(name_string, cookie_str);
    cgiCookie_MaxAge_set(cookie, COOKIE_AGE);
    cgiHeaderSetCookie(cookie);
    
    /* Put out the CGI header */
    cgiHeaderContentType("text/html");  
  }
  else {
    fixInputMsg();
  }


  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Microstrip  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Metal width                 = %g %s\n",
	    line->w/line->units_lwht->sf, line->units_lwht->name);
    fprintf(cgiOut,"CGI: Metal spacing               = %g %s\n",
	    line->s/line->units_lwht->sf, line->units_lwht->name);
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

    /* 
     * in case coplanar_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    coplanar_calc(line, line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    coplanar_syn(line, line->freq, CPWSYN_H);
    fprintf(cgiOut,"</pre>\n");
    h = line->subs->h;
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    coplanar_syn(line, line->freq, CPWSYN_W);
    fprintf(cgiOut,"</pre>\n");
    w = line->w;
    break;

  case SYNTH_S:
    fprintf(cgiOut,"<pre>");
    coplanar_syn(line, line->freq, CPWSYN_S);
    fprintf(cgiOut,"</pre>\n");
    s = line->s;
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    coplanar_syn(line, line->freq, CPWSYN_E$);
    fprintf(cgiOut,"</pre>\n");
    es = line->subs->er;
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    coplanar_syn(line, line->freq, CPWSYN_L);
    fprintf(cgiOut,"</pre>\n");
    l = line->l;
    break;

  }

  /* include the HTML output */
#include "header_html.c"
#include "coplanar_html.c"
#include "footer_html.c"
	
  return 0;
}

