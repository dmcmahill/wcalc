/* $Id: coax.cgi.c,v 1.2 2002/01/14 23:18:30 dan Exp $ */

/*
 * Copyright (c) 2002 Dan McMahill
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
 * a cgi interface to the coax calculator
 */

/* #define DEBUG */

#include "config.h"

#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cgi-units.h"

/* libwcalc */
#include "coax.h"
#include "misc.h"
#include "physconst.h"

/* ID's for this module */
#include "coax_id.h"

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD      0
#define RESET     1
#define ANALYZE   2
#define SYNTH_A   3
#define SYNTH_B   4
#define SYNTH_C   5
#define SYNTH_ER  6
#define SYNTH_L   7



/* defaults for the various parameters */
#define defA      1.0
#define defB      2.0
#define defC      0.0
#define defLEN    10.0
#define defTSHIELD    1.4
#define defRHOA   1.72e-8
#define defRHOB   1.72e-8

#define defER     2.2
#define defTAND   0.01

#define defFREQ   1000.0

#define defRO     50.0
#define defELEN   90.0

static const char *name_string="coax.cgi";

int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int action;
  int input_err = 0;

  /* coax variables */
  coax_line *line;
  double freq;

  double a,b,c,tshield,len;
  double rhoa,rhob,er,tand;
  double Ro=0.0;
  double elen;
  int i;


  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /*cgiReadEnvironment("/tmp/capcgi.dat"); */
  
  /* Put out the CGI header */
  cgiHeaderContentType("text/html");  

  /* create the coax line */
  fprintf(cgiOut,"<pre>");
  line = coax_new();
  fprintf(cgiOut,"</pre>");

  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_a",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_A;
  }
  else if(cgiFormStringNoNewlines("synth_b",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_B;
  }
  else if(cgiFormStringNoNewlines("synth_c",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_C;
  }
  else if(cgiFormStringNoNewlines("synth_er",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_ER;
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
   * the coax structure
   */

  if ( (action != RESET) && (action != LOAD) ) {
    /* Metal resistivity relative to copper */
    if(cgiFormDoubleBounded("rhoa",&rhoa,0.0,1000.0,defRHOB) !=
       cgiFormSuccess){
      input_err=1;
    }
    
    /* Metal resistivity relative to copper */
    if(cgiFormDoubleBounded("rhob",&rhob,0.0,1000.0,defRHOB) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* shield thickness (m) */
    if(cgiFormDoubleBounded("tshield",&tshield,0.0,1000.0,defTSHIELD) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("tshield_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->tshield_units = length_units[i].name;
    line->tshield_sf = length_units[i].sf;


    /* units */
    if (cgiFormRadio("a_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->a_units = length_units[i].name;
    line->a_sf = length_units[i].sf;

    /* Coax inner conductor radius */
    if(cgiFormDoubleBounded("a",&a,0.0,1000.0,defA) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* units */
    if (cgiFormRadio("b_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->b_units = length_units[i].name;
    line->b_sf = length_units[i].sf;

    /* Coax outer conductor radius */
    if(cgiFormDoubleBounded("b",&b,a*line->a_sf/line->b_sf,1000.0,defB) !=
       cgiFormSuccess){
      input_err=1;
      fprintf(cgiOut,"<pre>ERROR:  b must be > a\n</pre>\n");
    }

    /* units */
    if (cgiFormRadio("c_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->c_units = length_units[i].name;
    line->c_sf = length_units[i].sf;

    /* Coax inner conductor offset */
    if(cgiFormDoubleBounded("c",&c,(b*line->b_sf-a*line->a_sf)/line->c_sf,1000.0,defC) !=
       cgiFormSuccess){
      input_err=1;
      fprintf(cgiOut,"<pre>ERROR:  c must be < b-a\n</pre>\n");
    }

    /* Coax length */
    if(cgiFormDoubleBounded("len",&len,0.0,100000.0,defLEN) !=
       cgiFormSuccess){
      input_err=1;
    }
    /* units */
    if (cgiFormRadio("len_units",units_strings_get(length_units),units_size(length_units),&i,0) !=
	cgiFormSuccess){
      input_err=1;
    }
    line->len_units = length_units[i].name;
    line->len_sf = length_units[i].sf;


    /* Dielectric relative permittivity */
    if(cgiFormDoubleBounded("er",&er,1.0,1000.0,defER) !=
       cgiFormSuccess){
      input_err=1;
    }

    /* Dielectric loss tangent */
    if(cgiFormDoubleBounded("tand",&tand,0.0,1000.0,defTAND) !=
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
    line->a           = a*line->a_sf;
    line->b           = b*line->b_sf;
    line->c           = c*line->c_sf;
    line->tshield     = tshield*line->tshield_sf;
    line->len         = len*line->len_sf;

    line->freq = freq * line->freq_sf;
  
    /* copy over the other parameters */
    line->tand  = tand;
    line->er    = er;
    line->rho_a = rhoa;
    line->rho_b = rhob;
    
    line->z0 = Ro;
    /* XXX do i want to add these?  
       line->Ro = Ro;
       line->Xo = 0.0;
    */
    line->elen = elen;
    
  } /* if ( (action != RESET) && (action != LOAD) ) */
  
  
#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Coax  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Action                      = %d\n",action);
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"CGI: Inner Radius                = %g %s\n",
	    line->a/line->a_sf,line->a_units);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->a);
    fprintf(cgiOut,"CGI: Shield Radius               = %g %s\n",
	    line->b/line->b_sf,line->b_units);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->b);
    fprintf(cgiOut,"CGI: Offset                      = %g %s\n",
	    line->c/line->c_sf,line->c_units);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->c);
    fprintf(cgiOut,"CGI: Line length                 = %g %s\n",
	    line->len/line->len_sf,line->len_units);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->len);
    fprintf(cgiOut,"CGI: Shield thickness            = %g %s\n",
	    line->tshield/line->tshield_sf,line->tshield_units);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->tshield);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",line->rho_a);
    fprintf(cgiOut,"CGI: Metal resistivity rel to Cu = %g \n",line->rho_b);
    fprintf(cgiOut,"CGI: Relative dielectric const.  = %g \n",line->er);
    fprintf(cgiOut,"CGI: Dielectric loss tangent     = %g \n",line->tand);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    line->freq/line->freq_sf, line->freq_units); 
    fprintf(cgiOut,"CGI:                             = %g Hz\n",
	    line->freq);
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"</pre>\n");
#endif
  
  switch (action){
  case ANALYZE:

    /* 
     * in case coax_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    coax_calc(line,line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_A:
    fprintf(cgiOut,"<pre>");
    coax_syn(line,line->freq,COAXSYN_A);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_B:
    fprintf(cgiOut,"<pre>");
    coax_syn(line,line->freq,COAXSYN_B);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_C:
    fprintf(cgiOut,"<pre>");
    coax_syn(line,line->freq,COAXSYN_C);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_ER:
    fprintf(cgiOut,"<pre>");
    coax_syn(line,line->freq,COAXSYN_ER);
    fprintf(cgiOut,"</pre>\n");
    break;

  case SYNTH_L:
    fprintf(cgiOut,"<pre>");
    coax_syn(line,line->freq,COAXSYN_L);
    fprintf(cgiOut,"</pre>\n");
    break;

  }


  /* autoscale some outputs */
  units_autoscale(time_units,&line->delay_sf,&line->delay_units,line->delay);
  units_autoscale(frequency_units,&line->fc_sf,&line->fc_units,line->fc);

  /* include the HTML output */
#include "coax_html.c"
#include "footer_html.c"
	
  return 0;
}

