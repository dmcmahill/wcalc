/* $Id: microstrip.cgi.c,v 1.5 2003/01/24 11:05:10 dan Exp $ */

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
 * a cgi interface to the microstrip calculator
 */

/* #define DEBUG */

#include <stdio.h>
#include "cgic.h"
#include "cgi-units.h"
#include "microstrip.h"
#include "microstrip_id.h"
#include "units.h"

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
#define defW      110.0
#define defL      1000.0

#define defTMET   1.4
#define defRGH    0.05
#define defRHO    1.0

#define defH      62.0
#define defES     4.8
#define defTAND   0.01

#define defFREQ   1000.0

#define defRO     50.0
#define defELEN   90.0

static const char *name_string="microstrip.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* microstrip variables */
  microstrip_line *line;
  double freq;

  double rho,rough,tmet,w,l,h,es,tand;
  double Ro=0.0;
  double elen;

  cgi_units_menu *menu_lwht;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;
  cgi_units_menu *menu_len, *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal;

/* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the IC microstrip line */
  line = microstrip_line_new();

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
  cgi_units_attach_entry(menu_lwht, "entry_l");
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
   * the microstrip structure
   */

  if ( (action != RESET) && (action != LOAD) ) {

    cgi_units_menu_read();

    /* Metal resistivity relative to copper */
    if(cgiFormDoubleBounded("rho",&rho,0.0,1000.0,defRHO) !=
       cgiFormSuccess){
      input_err=1;
    }
    
    /* Metal thickness (m) */
    if(cgiFormDoubleBounded("tmet",&tmet,0.0,1000.0,defTMET) !=
       cgiFormSuccess){
      input_err=1;
    }
    
    /* Metalization roughness */
    if(cgiFormDoubleBounded("rough",&rough,0.0,1000.0,defRGH) !=
       cgiFormSuccess){
      input_err=1;
    }
    
    /* Microstrip width */
    if(cgiFormDoubleBounded("w",&w,0.0001,1000.0,defW) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Microstrip length */
    if(cgiFormDoubleBounded("l",&l,1.0,100000.0,defL) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Substrate dielectric thickness */
    if(cgiFormDoubleBounded("h",&h,0.0001,1000.0,defH) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Substrate relative permittivity */
    if(cgiFormDoubleBounded("es",&es,0.0001,1000.0,defES) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Substrate loss tangent */
    if(cgiFormDoubleBounded("tand",&tand,0.0001,1000.0,defTAND) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Frequency of operation (MHz) */
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      input_err=1;
    }


    /* electrical parameters: */
    if(cgiFormDoubleBounded("Ro",&Ro,0.0001,1000.0,defRO) !=
       cgiFormSuccess){
      input_err=1;
    }

    if(cgiFormDoubleBounded("elen",&elen,0.0001,1000.0,defELEN) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* copy data over to the line structure */
    line->w           = w*line->units_lwht->sf;
    line->l           = l*line->units_lwht->sf;
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


  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Microstrip  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Metal width                 = %g %s\n",
	    line->w/line->units_lwht->sf, line->units_lwht->name);
    fprintf(cgiOut,"CGI: Metal length                = %g %s\n",
	    line->l/line->units_lwht->sf, line->units_lwht->name);
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
     * in case microstrip_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    microstrip_calc(line, line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line, line->freq, MLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    h = line->subs->h;
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line, line->freq, MLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    w = line->w;
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line, line->freq, MLISYN_ES);
    fprintf(cgiOut,"</pre>\n");
    es = line->subs->er;
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line, line->freq, MLISYN_L);
    fprintf(cgiOut,"</pre>\n");
    l = line->l;
    break;

  }

  /* include the HTML output */
#include "header_html.c"
#include "microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

