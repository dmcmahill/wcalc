/* $Id: air_coil.cgi.c,v 1.7 2002/01/07 01:24:14 dan Exp $ */

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

/* #define DEBUG */

#include "config.h"

#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cgi-units.h"

/* libwcalc */
#include "air_coil.h"
#include "misc.h"
#include "physconst.h"

/* ID's for this module */
#include "air_coil_id.h"

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
#define defFILL   2.0
#define defFREQ   10.0
#define defIND    100.0

static const char *name_string="air_coil.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;
  int i;

  /* air_coil variables */
  air_coil_coil *coil;
  double freq;

  double N,AWG,rho,dia,len,L;

  char *fill_choices[] = {"no","yes"};

  /* uncomment to be able to run in the debugger. */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

/* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the air_coil  */
  coil = air_coil_new();


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

  
  if ( (action == RESET) || (action == LOAD) ) {
    /*
    N     = defN;
    AWG   = defAWG;
    rho   = defRHO;
    dia   = defDIA;
    len   = defLEN;
    L     = defIND;
    freq  = defFREQ;
    coil->fill = defFILL; */
  }

  else {
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
    
    /* inside diameter units */
    if (cgiFormRadio("dia_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    coil->dia_units = length_units[i].name;
    coil->dia_sf = length_units[i].sf;
#ifdef DEBUG
    fprintf(cgiOut,"<pre>CGI:  dia_units = %s</pre>\n",coil->dia_units);
#endif
    
    /* Solenoid length  */
    if(cgiFormDoubleBounded("len",&len,0.0001,1000.0,defLEN) !=
       cgiFormSuccess){
      input_err=1;
    }
    
    /* Solenoid length units */
    if (cgiFormRadio("len_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    coil->len_units = length_units[i].name;
    coil->len_sf = length_units[i].sf;
#ifdef DEBUG
    fprintf(cgiOut,"<pre>CGI:  len_units = %s</pre>\n",coil->len_units);
#endif

    /* Solenoid fill  */
    if(cgiFormDoubleBounded("fill",&coil->fill,1.0,10.0,defFILL) !=
       cgiFormSuccess){
      input_err=1;
    }

    if (cgiFormRadio("use_fill",fill_choices,2,&coil->use_fill,0) !=
	cgiFormSuccess){
      input_err=1;
    }
      
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      input_err=1;
    }  

    /* Frequency of operation  */
    if(cgiFormDoubleBounded("freq",&freq,1e-6,1e6,defFREQ) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Frequency of operation units */
    if (cgiFormRadio("freq_units",units_strings_get(frequency_units),units_size(frequency_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    coil->freq_units = frequency_units[i].name;
    coil->freq_sf = frequency_units[i].sf;
#ifdef DEBUG
    fprintf(cgiOut,"<pre>CGI:  freq_units = %s</pre>\n",coil->freq_units);
#endif

    /* Inductance units */
    if (cgiFormRadio("L_units",units_strings_get(inductance_units),units_size(inductance_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    coil->L_units = inductance_units[i].name;
    coil->L_sf = inductance_units[i].sf;
#ifdef DEBUG
    fprintf(cgiOut,"<pre>CGI:  L_units = %s</pre>\n",coil->L_units);
#endif

    /* Desired Inductance */
    if(cgiFormDoubleBounded("L",&L,0.0001,1000.0,defIND) !=
       cgiFormSuccess){
      input_err=1;
    }


    /* copy data over to the coil structure */
    coil->Nf   = N;
    coil->AWGf = AWG;
    coil->rho  = rho*coil->rho_sf;
    coil->dia  = dia*coil->dia_sf;
    coil->len  = len*coil->len_sf;
    coil->L    = L*coil->L_sf;
    coil->freq = freq*coil->freq_sf;
    
  } 
  



#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- ----- Air Coil ----- -----------\n");
    fprintf(cgiOut,"CGI: action = %d\n",action);
    fprintf(cgiOut,"CGI: --------------- -------------------- -----------\n");
    fprintf(cgiOut,"CGI: Number of turns             = %g \n",coil->Nf);
    fprintf(cgiOut,"CGI: Wire Size                   = %g AWG\n",coil->AWGf);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",coil->rho);
    fprintf(cgiOut,"CGI: Metal resistivity           = %g %s\n",coil->rho/coil->rho_sf,coil->rho_units);
    fprintf(cgiOut,"CGI: Inside Diameter             = %g %s\n",coil->dia/coil->dia_sf,coil->dia_units);
    fprintf(cgiOut,"CGI: Solenoid length             = %g %s\n",coil->len/coil->len_sf,coil->len_units);
    fprintf(cgiOut,"CGI: Solenoid fill               = %g \n",coil->fill);
    fprintf(cgiOut,"CGI: use_fill                    = %d \n",coil->use_fill);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",coil->freq/coil->freq_sf,coil->freq_units); 
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif


  switch (action){
  case ANALYZE:
    /* 
     * in case air_coil_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    air_coil_calc(coil,coil->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_NMIN:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil,coil->freq,AIRCOILSYN_NMIN);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_NFIX:
    fprintf(cgiOut,"<pre>");
    air_coil_syn(coil,coil->freq,AIRCOILSYN_NFIX);
    fprintf(cgiOut,"</pre>\n");
    break;

  case LOAD:
  case RESET:
    break;

  default:
    fprintf(stderr,"air_coil.cgi:  unknown action (%d)\n",action);
    exit(1);
    break;
  }

  /* autoscale the SRF output */
  units_autoscale(frequency_units,&coil->SRF_sf,&coil->SRF_units,coil->SRF);

  /* include the HTML output */
#include "air_coil_html.c"
#include "footer_html.c"
	
  return 0;
}

