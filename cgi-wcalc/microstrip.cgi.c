/* $Id: microstrip.cgi.c,v 1.2 2002/02/20 01:07:31 dan Exp $ */

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
 * a cgi interface to the microstrip calculator
 */

/* #define DEBUG */

#include <stdio.h>
#include "cgic.h"
#include "microstrip.h"
#include "microstrip_id.h"

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
  double freq, freq_Hz;

  double rho,rough,tmet,w,l,h,es,tand;
  double Ro=0.0;
  double Xo=0.0;
  double elen;

  /* delay on the line (ns) */
  double delay;

  /* The circuit model */
  double Ls, Rs, Cs, Gs;

/* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the IC microstrip line */
  line = microstrip_line_new();

  /*
   * extract the parameters from the CGI form and use them to populate
   * the microstrip structure
   */


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
  

  if ( (action == RESET) || (action == LOAD) ){
    w     = defW;
    l     = defL;
    
    tmet  = defTMET;
    rho   = defRHO;
    rough = defRGH;

    h     = defH;
    es    = defES;
    tand  = defTAND;

    Ro    = defRO;
    elen  = defELEN;
  }


  /* copy data over to the line structure */
  line->w           = w     ;
  line->l           = l     ;
  line->subs->h     = h     ;
  line->subs->tmet  = tmet  ;
  line->subs->rough = rough ;
  
  /* convert to Hz from MHz */
  freq_Hz = freq * 1.0e6;
  
  /* copy over the other parameters */
  line->subs->tand  = tand;
  line->subs->er    = es;
  line->subs->rho   = rho;

  line->z0 = Ro;
  line->Ro = Ro;
  line->Xo = 0.0;
  line->len = elen;

  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Microstrip  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Metal width                 = %g mil\n",line->w);
    fprintf(cgiOut,"CGI: Metal length                = %g mil\n",line->l);
    fprintf(cgiOut,"CGI: Metal thickness             = %g mil\n",line->subs->tmet);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",line->subs->rho);
    fprintf(cgiOut,"CGI: Metal surface roughness     = %g mil-rms\n",line->subs->rough);
    fprintf(cgiOut,"CGI: Substrate thickness         = %g mil\n",line->subs->h);
    fprintf(cgiOut,"CGI: Substrate dielectric const. = %g \n",line->subs->er);
    fprintf(cgiOut,"CGI: Substrate loss tangent      = %g \n",line->subs->tand);
    fprintf(cgiOut,"CGI: Frequency                   = %g MHz\n",freq_Hz/1e6); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif

    /* 
     * in case microstrip_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    microstrip_calc(line,freq_Hz);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line,freq_Hz,MLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    h = line->subs->h;
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line,freq_Hz,MLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    w = line->w;
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line,freq_Hz,MLISYN_ES);
    fprintf(cgiOut,"</pre>\n");
    es = line->subs->er;
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    microstrip_syn(line,freq_Hz,MLISYN_L);
    fprintf(cgiOut,"</pre>\n");
    l = line->l;
    break;

  }

  /* extract the incremental circuit model */
  Ls = line->Ls;
  Rs = line->Rs;
  Cs = line->Cs;
  Gs = line->Gs;
  Ro   = line->Ro;
  Xo   = line->Xo;

  /* electrical and physical length */
  elen = line->len;
  l = line->l;

  /*
   * delay on line (ns)
   * 2 pi f Td = elen pi/180
   * Td = (pi/180) elen/(2 pi f) = elen/(360 f)
   * and in ns,
   * Td = elen/(360 f *1e-9)
   */
  delay = elen /(360.0 * freq_Hz * 1e-9);

  /* include the HTML output */
#include "header_html.c"
#include "microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

