/* $Id: air_coil.cgi.c,v 1.2 2001/09/15 02:57:38 dan Exp $ */

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
 * a cgi interface to the air_coil calculator
 */

//#define DEBUG

#include <stdio.h>
#include "cgic.h"
#include "air_coil.h"
#include "air_coil_id.h"
#include "physconst.h"

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD         0
#define RESET        1
#define ANALYZE      2
#define SYNTH_NMIN   3
#define SYNTH_NFIX   4



/* defaults for the various parameters */
#define defN      4
#define defAWG    22
#define defRHO    1.0
#define defDIA    0.25
#define defLEN    0.50
#define defFREQ   10.0
#define defIND    100.0

static const char *name_string="air_coil.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* air_coil variables */
  air_coil *coil;
  double freq, freq_Hz;

  double N,AWG,rho,dia,len,L;

/* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the air_coil  */
  coil = air_coil_new();

  /*
   * extract the parameters from the CGI form and use them to populate
   * the air_coil structure
   */


  /* Number of turns */
  if(cgiFormDoubleBounded("N",&N,0.0001,1000.0,defN) !=
     cgiFormSuccess){
    input_err=1;
  }

  /* wire size */
  if(cgiFormDoubleBounded("AWG",&AWG,0.0001,1000.0,defAWG) !=
     cgiFormSuccess){
    input_err=1;
  }

  /* Metal resistivity relative to copper */
  if(cgiFormDoubleBounded("rho",&rho,0.0001,1000.0,defRHO) !=
     cgiFormSuccess){
    input_err=1;
  }


  /* inside diameter */
  if(cgiFormDoubleBounded("dia",&dia,0.0001,1000.0,defDIA) !=
     cgiFormSuccess){
    input_err=1;
  }

  /* Solenoid length (inches) */
  if(cgiFormDoubleBounded("len",&len,0.0001,1000.0,defLEN) !=
     cgiFormSuccess){
    input_err=1;
  }

  /* Frequency of operation (MHz) */
  if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
     cgiFormSuccess){
    input_err=1;
  }


  /* Desired Inductance (nH) */
  if(cgiFormDoubleBounded("L",&L,0.0001,1000.0,defIND) !=
     cgiFormSuccess){
    input_err=1;
  }


  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_n",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_NMIN;
  }
  else if(cgiFormStringNoNewlines("synth_nfix",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_NFIX;
  }
  else if(cgiFormStringNoNewlines("reset",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = RESET;
  }
  else{
    action = LOAD;
  }

#ifdef DEBUG
  fprintf(cgiOut,"<pre>\n");
  fprintf(cgiOut,"CGI: --------------- Air_Coil  Analysis -----------\n");
  fprintf(cgiOut,"CGI: action = %d\n",action);
  fprintf(cgiOut,"CGI: --------------- ------------------ -----------\n");
  fprintf(cgiOut,"</pre>\n");
#endif


  if ( (action == RESET) || (action == LOAD) ){
    N     = defN;
    AWG   = defAWG;
    rho   = defRHO;
    dia   = defDIA;
    len   = defLEN;
    L     = defIND;
    freq  = defFREQ;
  }


  /* copy data over to the coil structure */
  coil->Nf     = N      ;
  coil->AWGf   = AWG    ;
  coil->rho    = rho    ;
  coil->dia    = INCH2M(dia)    ;
  coil->len    = INCH2M(len)    ;

  /* nH -> H */
  coil->L      = L*1e-9 ;

  /* MHz -> MHz */
  coil->freq  = freq*1e6   ;

  /* convert to Hz from MHz */
  freq_Hz = freq * 1.0e6;
  
  
  switch (action){
  case ANALYZE:
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Air_Coil  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Number of turns             = %g \n",coil->Nf);
    fprintf(cgiOut,"CGI: Wire Size                   = %g AWG\n",coil->AWGf);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",coil->rho);
    fprintf(cgiOut,"CGI: Inside Diameter             = %g inches\n",M2INCH(coil->dia));
    fprintf(cgiOut,"CGI: Solenoid length             = %g inches\n",M2INCH(coil->len));
    fprintf(cgiOut,"CGI: Frequency                   = %g MHz\n",freq_Hz/1e6); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif

    /* 
     * in case air_coil_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    air_coil_calc(coil,freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_NMIN:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil,freq,AIRCOILSYN_NMIN);
    fprintf(cgiOut,"</pre>\n");
    N = coil->Nf;
    len = M2INCH(coil->len);
    break;

  case SYNTH_NFIX:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil,freq,AIRCOILSYN_NFIX);
    fprintf(cgiOut,"</pre>\n");
    len = M2INCH(coil->len);
    break;

  case LOAD:
  case RESET:
    break;

  default:
    fprintf(stderr,"air_coil.cgi:  unknown action (%d)\n",action);
    exit(1);
    break;
  }

  /* extract results */
  N = coil->Nf;
  L = coil->L;

  /* include the HTML output */
#include "air_coil_html.c"
#include "footer_html.c"
	
  return 0;
}

