/* $Id: ic_microstrip.cgi.c,v 1.1 2001/10/05 01:28:30 dan Exp $ */

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
 * a cgi interface to the ic_microstrip calculator
 */

#include <stdio.h>
#include "cgic.h"
#include "ic_microstrip.h"
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
#define defW      160.0

#define defTMET   1.6
#define defRGH    0.01
#define defRHO    1.0

#define defTOX    1.0
#define defEOX    4.0

#define defH      250.0
#define defES     11.8
#define defRHOS   1.0

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
  double freq_Hz=0;

  double rho,rough,tmet,w,tox,eox,h,es,rhos;
  double Ro=0.0;
  double Xo=0.0;

  /* The circuit model */
  double Lmis, Rmis, Cmis, Gmis;

/* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the IC microstrip line */
  line = ic_microstrip_line_new();

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
  if(cgiFormDoubleBounded("rhos",&rhos,0.0001,1000.0,defRHOS) !=
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
  

  if ( (action == RESET) || (action == LOAD) ){
    w     = defW;
    
    tmet  = defTMET;
    rho   = defRHO;
    rough = defRGH;

    tox   = defTOX;
    eox   = defEOX;
    
    h     = defH;
    es    = defES;
    rhos  = defRHOS;
  }

  if (action != LOAD){
    /* convert to meters from um */
    line->w           = w     * 1.0e-6;
    line->subs->h     = h     * 1.0e-6;
    line->subs->tox   = tox   * 1.0e-6;
    line->subs->tmet  = tmet  * 1.0e-6;
    line->subs->rough = rough *1.0e-6;
    
    /* convert to Hz from MHz */
    freq_Hz = freq * 1.0e6;
    
    /* calculate conductivity in 1/(ohm-m) from resistivity in ohm-cm */
    line->subs->sigmas = 100.0 / rhos;
    
    /* copy over the other parameters */
    line->subs->eox = eox;
    line->subs->es = es;
    line->subs->rho = rho;

    line->Ro = Ro;
  }
  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"-------------- IC Microstrip Analysis ----------\n");
    fprintf(cgiOut,"Metal width                 = %g um\n",line->w*1e6);
    fprintf(cgiOut,"Oxide thickness             = %g um\n",line->subs->tox*1e6);
    fprintf(cgiOut,"Oxide dielectric const.     = %g \n",line->subs->eox);
    fprintf(cgiOut,"Substrate thickness         = %g um\n",line->subs->h*1e6);
    fprintf(cgiOut,"Substrate dielectric const. = %g \n",line->subs->es);
    fprintf(cgiOut,"Substrate conductivity      = %g 1/(ohm-cm)\n",line->subs->sigmas*0.01);
    fprintf(cgiOut,"Substrate resistivity       = %g ohm-cm\n",100.0/(line->subs->sigmas));
    fprintf(cgiOut,"Frequency                   = %g GHz\n",freq_Hz/1e9); 
    fprintf(cgiOut,"-------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif

    /* 
     * in case ic_microstrip_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    ic_microstrip_calc(line,freq_Hz);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_H:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line,freq_Hz,IC_MLISYN_H);
    fprintf(cgiOut,"</pre>\n");
    h = line->subs->h*1e6;
    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line,freq_Hz,IC_MLISYN_W);
    fprintf(cgiOut,"</pre>\n");
    w = line->w*1e6;
    break;

  case SYNTH_TOX:
    fprintf(cgiOut,"<pre>");
    ic_microstrip_syn(line,freq_Hz,IC_MLISYN_TOX);
    fprintf(cgiOut,"</pre>\n");
    tox = line->subs->tox*1e6;
    break;

  }

  /* extract the incremental circuit model */
  Lmis = line->Lmis;
  Rmis = line->Rmis;
  Cmis = line->Cmis;
  Gmis = line->Gmis;
  Ro   = line->Ro;
  Xo   = line->Xo;


  /* include the HTML output */
#include "header_html.c"
#include "ic_microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

