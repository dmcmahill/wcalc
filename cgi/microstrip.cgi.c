/* $Id$ */

/*
 * Copyright (c) 2001 Dan McMahill
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

#include <stdio.h>
#include "cgic.h"
#include "microstrip.h"
#include "microstrip_id.h"

#define ACTION_LEN  20

#define LOAD      0
#define RESET     1
#define ANALYZE   2
#define SYNTH_H   3
#define SYNTH_W   4
#define SYNTH_ES  5



/* defaults for the various parameters */
#define defW      110.0
#define defL      1000.0

#define defTMET   1.4
#define defRGH    0.01
#define defRHO    1.0

#define defH      62.0
#define defES     4.8
#define defTAND   0.0001

#define defFREQ   1000.0

#define defELEN   90.0

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
  if(cgiFormDoubleBounded("l",&l,0.0001,1000.0,defL) !=
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
  if(cgiFormDoubleBounded("Ro",&Ro,0.0001,1000.0,50.0) !=
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
  }

  if (action != LOAD){
    /* copy over */
    line->w           = w     ;
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
    line->len = elen;
  }
  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"--------------- Microstrip  Analysis -----------\n");
    fprintf(cgiOut,"Metal width                 = %g mil\n",line->w);
    fprintf(cgiOut,"Metal thickness             = %g mil\n",line->subs->tmet);
    fprintf(cgiOut,"Metal length                = %g mil\n",line->l);
    fprintf(cgiOut,"Substrate thickness         = %g mil\n",line->subs->h);
    fprintf(cgiOut,"Substrate dielectric const. = %g \n",line->subs->es);
    fprintf(cgiOut,"Substrate loss tangent      = %g \n",line->subs->tand);
    fprintf(cgiOut,"Frequency                   = %g MHz\n",freq_Hz/1e6); 
    fprintf(cgiOut,"-------------- ---------------------- ----------\n");
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
    //microstrip_syn(line,freq_Hz,MLISYN_H);
    microstrip_syn(line,freq_Hz);
    fprintf(cgiOut,"</pre>\n");
    h = line->subs->h;
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    //microstrip_syn(line,freq_Hz,MLISYN_W);
    microstrip_syn(line,freq_Hz);
    fprintf(cgiOut,"</pre>\n");
    w = line->w;
    break;

  case SYNTH_ES:
    fprintf(cgiOut,"<pre>");
    //microstrip_syn(line,freq_Hz,MLISYN_ES);
    microstrip_syn(line,freq_Hz);
    fprintf(cgiOut,"</pre>\n");
    es = line->subs->er;
    break;

  }

  /* extract the incremental circuit model */
  Ls = line->Ls;
  Rs = line->Rs;
  Cs = line->Cs;
  Gs = line->Gs;
  Ro   = line->Ro;
  Xo   = line->Xo;


  /* include the HTML output */
#include "microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

