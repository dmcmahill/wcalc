/* $Id: stripline.cgi.c,v 1.3 2002/01/14 02:55:02 dan Exp $ */

/*
 * Copyright (c) 2001, 2002 Dan McMahill
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
 *        This product includes software developed Dan McMahill
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
 * a cgi interface to the stripline calculator
 */

/* #define DEBUG */

#include "config.h"

#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cgi-units.h"

/* libwcalc */
#include "stripline.h"
#include "misc.h"
#include "physconst.h"

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
  int i;


  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */
  
  /* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the stripline line */
  fprintf(cgiOut,"<pre>");
  line = stripline_line_new();
  fprintf(cgiOut,"</pre>");

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
    /* units */
    if (cgiFormRadio("tmet_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->subs->tmet_units = length_units[i].name;
    line->subs->tmet_sf = length_units[i].sf;

    /* Metalization roughness */
    if(cgiFormDoubleBounded("rough",&rough,0.0,1000.0,defRGH) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("rough_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->subs->rough_units = length_units[i].name;
    line->subs->rough_sf = length_units[i].sf;


    /* Stripline width */
    if(cgiFormDoubleBounded("w",&w,0.0001,1000.0,defW) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("w_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->w_units = length_units[i].name;
    line->w_sf = length_units[i].sf;

    /* Stripline length */
    if(cgiFormDoubleBounded("l",&l,1.0,100000.0,defL) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("l_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->l_units = length_units[i].name;
    line->l_sf = length_units[i].sf;


    /* Substrate dielectric thickness */
    if(cgiFormDoubleBounded("h",&h,0.0001,1000.0,defH) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("h_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->subs->h_units = length_units[i].name;
    line->subs->h_sf = length_units[i].sf;


    /* Substrate relative permittivity */
    if(cgiFormDoubleBounded("es",&es,0.0001,1000.0,defES) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Substrate loss tangent */
    if(cgiFormDoubleBounded("tand",&tand,0.0,1000.0,defTAND) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Frequency of operation (MHz) */
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* Frequency of operation units */
    if (cgiFormRadio("freq_units",units_strings_get(frequency_units),units_size(frequency_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->freq_units = frequency_units[i].name;
    line->freq_sf = frequency_units[i].sf;


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
  line->w           = w*line->w_sf;
  line->l           = l*line->l_sf;
  line->subs->h     = h*line->subs->h_sf;
  line->subs->tmet  = tmet*line->subs->tmet_sf;
  line->subs->rough = rough*line->subs->rough_sf;

  /* convert to Hz from MHz */
  line->freq = freq * line->freq_sf;
  
  /* copy over the other parameters */
  line->subs->tand  = tand;
  line->subs->er    = es;
  line->subs->rho   = rho;

  line->z0 = Ro;
  line->Ro = Ro;
  line->Xo = 0.0;
  line->len = elen;

  } /* if ( (action != RESET) && (action != LOAD) ) */

  
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Stripline  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Action                      = %d\n",action);
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"CGI: Metal width                 = %g %s\n",
	    line->w/line->w_sf,line->w_units);
    fprintf(cgiOut,"CGI: Metal length                = %g %s\n",
	    line->l/line->l_sf,line->l_units);
    fprintf(cgiOut,"CGI: Metal thickness             = %g %s\n",
	    line->subs->tmet/line->subs->tmet_sf,line->subs->tmet_units);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",line->subs->rho);
    fprintf(cgiOut,"CGI: Metal surface roughness     = %g %s-rms\n",
	    line->subs->rough/line->subs->rough_sf,line->subs->rough_units);
    fprintf(cgiOut,"CGI: Substrate thickness         = %g %s\n",
	    line->subs->h/line->subs->h_sf, line->subs->h_units);
    fprintf(cgiOut,"CGI: Substrate dielectric const. = %g \n",line->subs->er);
    fprintf(cgiOut,"CGI: Substrate loss tangent      = %g \n",line->subs->tand);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    line->freq/line->freq_sf, line->freq_units); 
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
    stripline_calc(line,line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line,line->freq,SLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line,line->freq,SLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line,line->freq,SLISYN_ER);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    stripline_syn(line,line->freq,SLISYN_L);
    fprintf(cgiOut,"</pre>\n");
    break;

  }


  /* autoscale the delay output */
  units_autoscale(time_units,&line->delay_sf,&line->delay_units,line->delay);

  /* include the HTML output */
#include "header_html.c"
#include "stripline_html.c"
#include "footer_html.c"
	
  return 0;
}

