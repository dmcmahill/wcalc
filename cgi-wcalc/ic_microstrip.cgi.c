/* $Id: ic_microstrip.cgi.c,v 1.3 2002/06/12 11:30:06 dan Exp $ */

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
 * a cgi interface to the ic_microstrip calculator
 */

#include <stdio.h>
#include "cgic.h"
#include "cgi-units.h"
#include "ic_microstrip.h"
#include "ic_microstrip_id.h"
#include "units.h"

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
#define defW      160.0
#define defL      1000.0

#define defTMET   1.6
#define defRGH    0.01
#define defRHO    1.0

#define defTOX    1.0
#define defEOX    4.0

#define defH      250.0
#define defES     11.8
#define defSIGMAS 1.0

#define defFREQ   1000.0

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

  cgi_units_menu *menu_lwht;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;
  cgi_units_menu *menu_len, *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal, *menu_sigmas;

  /* Put out the CGI header */
  cgiHeaderContentType("text/html");  
  
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

    /* Metal resistivity relative to copper */
    if(cgiFormDoubleBounded("rho",&rho,0.0001,1000.0,defRHO) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Metal thickness (m) */
    if(cgiFormDoubleBounded("tmet",&tmet,0.0001,1000.0,defTMET) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Metalization roughness */
    if(cgiFormDoubleBounded("rough",&rough,0.0001,1000.0,defRGH) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Microstrip width */
    if(cgiFormDoubleBounded("w",&w,0.0001,1000.0,defW) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Microstrip length */
    if(cgiFormDoubleBounded("l",&l,0.0001,1000.0,defL) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Oxide thickness */
    if(cgiFormDoubleBounded("tox",&tox,0.0001,1000.0,defTOX) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Oxide relative permittivity */
    if(cgiFormDoubleBounded("eox",&eox,0.0001,1000.0,defEOX) !=
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

    /* Substrate conductivity s/m*/
    if(cgiFormDoubleBounded("sigmas",&sigmas,0.0001,1000.0,defSIGMAS) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Frequency of operation (MHz) */
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      input_err=1;
    }


    /* electrical parameters: */
    if(cgiFormDoubleBounded("Ro",&Ro,0.0001,1000.0,50.0) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* copy data over to the line structure */
    line->w           = w*line->units_lwht->sf;
    line->l           = l*line->units_lwht->sf;
    line->subs->h     = h*line->units_lwht->sf;
    line->subs->tox   = h*line->units_lwht->sf;
    line->subs->tmet  = tmet*line->units_lwht->sf;
    line->subs->rough = rough*line->units_rough->sf;


    line->freq = freq * line->units_freq->sf;
  
    /* copy over the other parameters */
    /* XXX */
    line->subs->sigmas  = sigmas*line->units_sigmas->sf;
    line->subs->eox    = eox;
    line->subs->es    = es;
    line->subs->rho   = rho*line->units_rho->sf;

    line->Ro = Ro;
    line->Xo = 0.0;

  } /* if ( (action != RESET) && (action != LOAD) ) */

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

