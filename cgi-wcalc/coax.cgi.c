/* $Id: coax.cgi.c,v 1.17 2004/08/31 21:53:03 dan Exp $ */

/*
 * Copyright (c) 2002, 2004, 2005 Dan McMahill
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
 * a cgi interface to the coax calculator
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
#include "coax.h"
#include "coax_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "coax_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD      0
#define RESET     1
#define ANALYZE   2
#define SYNTH_A   3
#define SYNTH_B   4
#define SYNTH_C   5
#define SYNTH_ER  6



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
static int input_err;


int cgiMain(void){
  
  /* CGI variables */
  char str_action[ACTION_LEN];
  
  int action;

  /* coax variables */
  coax_line *line;
  double freq;

  double a, b, c, tshield, len;
  double rhoa, rhob, er, tand;
  double Ro=0.0;
  double elen;

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_emax, *menu_loss, *menu_losslen, *menu_fc, *menu_delay;
  cgi_units_menu *menu_rho, *menu_abct, *menu_freq, *menu_len;
  cgi_units_menu *menu_L, *menu_R, *menu_C, *menu_G;

  input_err = 0;

  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the coax line */
#ifdef DEBUG
  printf("coax.cgi:  calling coax_new()\n");
#endif
  line = coax_new();

  menu_emax = cgi_units_menu_new(line->units_emax);
  menu_fc = cgi_units_menu_new(line->units_fc);
  menu_delay = cgi_units_menu_new(line->units_delay);
  menu_loss = cgi_units_menu_new(line->units_loss);
  menu_losslen = cgi_units_menu_new(line->units_losslen);
  cgi_units_attach_entry(menu_losslen, "entry_alphac");
  cgi_units_attach_entry(menu_losslen, "entry_alphad");

  menu_rho = cgi_units_menu_new(line->units_rho);
  cgi_units_attach_entry(menu_rho, "entry_rhob");

  menu_abct = cgi_units_menu_new(line->units_abct);
  cgi_units_attach_entry(menu_abct, "entry_b");
  cgi_units_attach_entry(menu_abct, "entry_c");
  cgi_units_attach_entry(menu_abct, "entry_t");

  menu_len = cgi_units_menu_new(line->units_len);
  menu_freq = cgi_units_menu_new(line->units_freq);

  menu_L = cgi_units_menu_new(line->units_L);
  menu_R = cgi_units_menu_new(line->units_R);
  menu_C = cgi_units_menu_new(line->units_C);
  menu_G = cgi_units_menu_new(line->units_G);

#ifdef DEBUG
  printf("coax.cgi:  checking out which button was pressed\n");
#endif

  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze", str_action, ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_a", str_action, ACTION_LEN) ==
	  cgiFormSuccess){
    action = SYNTH_A;
  }
  else if(cgiFormStringNoNewlines("synth_b", str_action, ACTION_LEN) ==
	  cgiFormSuccess){
    action = SYNTH_B;
  }
  else if(cgiFormStringNoNewlines("synth_c", str_action, ACTION_LEN) ==
	  cgiFormSuccess){
    action = SYNTH_C;
  }
  else if(cgiFormStringNoNewlines("synth_er", str_action, ACTION_LEN) ==
	  cgiFormSuccess){
    action = SYNTH_ER;
  }
  else if(cgiFormStringNoNewlines("reset", str_action, ACTION_LEN) ==
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
#ifdef DEBUG
    printf("coax.cgi:  reading form values\n");
#endif
    
    cgi_units_menu_read();

    /* Center conductor metal resistivity */
    if(cgiFormDouble("rhoa", &rhoa, defRHOA/line->units_rho->sf) 
       != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Center conductor resistivity out of range");
    }
    if( rhoa < 0.0 ) {
      rhoa = defRHOA/line->units_rho->sf;
      printFormError("Center conductor resistivity may not be negative");
    }
    
    /* Shield metal resistivity  */
    if(cgiFormDouble("rhob", &rhob, defRHOB/line->units_rho->sf) 
       != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading shield resistivity");
    }
    if( rhob < 0.0 ) {
      rhob = defRHOB/line->units_rho->sf;
      printFormError("Shield conductor resistivity may not be negative");
    }
    
    /* shield thickness */
    if(cgiFormDouble("tshield", &tshield, defTSHIELD/line->units_abct->sf) !=
       cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading shield thickness");
    }
    if( tshield < 0.0 ) {
      tshield = defTSHIELD/line->units_abct->sf;
      printFormError("Shield thickness may not be negative");
    }

    /* Coax inner conductor radius */
    if(cgiFormDouble("a", &a, defA/line->units_abct->sf) 
       != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading a");
    }
    if( a <= 0.0 ) {
      a = defA/line->units_abct->sf;
      printFormError("Center conductor radius must be %gt 0");
    }

    /* Coax outer conductor radius */
    if(cgiFormDouble("b", &b, defB/line->units_abct->sf) 
       != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading b");
    }
    if( b <= a ) {
      b = 2*a;
      printFormError("b must be &gt a");
    }
     

    /* Coax inner conductor offset */
    if(cgiFormDouble("c", &c, defC/line->units_abct->sf) !=
       cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading c");
    }
   if( (c < 0.0) || ( c >= b - a ) ) {
      c = 0.0;
      printFormError("c must be in the range 0 &lt;= c &lt b-a");
    }
 
    /* Coax length */
    if(cgiFormDouble("len", &len, defLEN/line->units_len->sf) !=
       cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading length");
    }
    if( len <= 0.0 ) {
      len = defLEN/line->units_len->sf;
      printFormError("Cable length must be &gt 0");
    }

    /* Dielectric relative permitivity */
    if(cgiFormDoubleBounded("er", &er, 1.0, 1000.0, defER) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Permitivity out of range");
    }
    
    /* Dielectric loss tangent */
    if(cgiFormDoubleBounded("tand", &tand, 0.0, 1000.0, defTAND) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("loss tangent out of range");
    }
    
    /* Frequency of operation  */
    if(cgiFormDouble("freq", &freq, defFREQ/line->units_freq->sf) !=
       cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading frequency");
    }
    if( freq < 0.0 ) {
      freq = defFREQ/line->units_freq->sf;
      printFormError("Frequency may not be negative");
    }    

    /* electrical parameters: */
    if(cgiFormDoubleBounded("Ro", &Ro, 0.0001, 1000.0, defRO) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("input Z0 out of range");
    }

    if(cgiFormDouble("elen", &elen, defELEN) != cgiFormSuccess) {
      inputErr(&input_err);
      printFormError("Error reading electrical length");
    } 
    if( elen <= 0.0 ) {
      elen = defELEN;
      printFormError("Electrical length must be &gt 0");
    }

    /* copy data over to the line structure */
    line->a           = a*line->units_abct->sf;
    line->b           = b*line->units_abct->sf;
    line->c           = c*line->units_abct->sf;
    line->tshield     = tshield*line->units_abct->sf;
    line->len         = len*line->units_len->sf;

    line->freq = freq * line->units_freq->sf;
  
    /* copy over the other parameters */
    line->tand  = tand;
    line->er    = er;
    line->rho_a = rhoa*line->units_rho->sf;
    line->rho_b = rhob*line->units_rho->sf;
    
    line->z0 = Ro;
    /* XXX do i want to add these?  
       line->Ro = Ro;
       line->Xo = 0.0;
    */
    line->elen = elen;
    
  } /* if ( (action != RESET) && (action != LOAD) ) */
  else {

#ifdef DEBUG
    printf("coax.cgi:  checking for a cookie to load\n");
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
    printf("coax.cgi:  loading cookie \"%s\"\n", cookie_load_str);
#endif
      coax_load_string(line, cookie_load_str);
#ifdef DEBUG
    printf("coax.cgi:  finished loading cookie\n");
#endif
    }
    
  }
  
  if (!input_err){
    cookie_str = coax_save_string(line);
    cookie = cgiCookie_new(name_string, cookie_str);
    cgiCookie_MaxAge_set(cookie, COOKIE_AGE);
    cgiHeaderSetCookie(cookie);
    
    /* Put out the CGI header */
    cgiHeaderContentType("text/html");  
  }
  else {
    fixInputMsg();
  }

#ifdef DEBUG
    fprintf(cgiOut,"<pre>\n");
    fprintf(cgiOut,"CGI: --------------- Coax  Analysis -----------\n");
    fprintf(cgiOut,"CGI: Action                      = %d\n", action);
    fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
    fprintf(cgiOut,"CGI: Inner Radius                = %g %s\n",
	    line->a/line->units_abct->sf, line->units_abct->name);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->a);
    fprintf(cgiOut,"CGI: Shield Radius               = %g %s\n",
	    line->b/line->units_abct->sf, line->units_abct->name);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->b);
    fprintf(cgiOut,"CGI: Offset                      = %g %s\n",
	    line->c/line->units_abct->sf, line->units_abct->name);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->c);
    fprintf(cgiOut,"CGI: Line length                 = %g %s\n",
	    line->len/line->units_len->sf, line->units_len->name);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->len);
    fprintf(cgiOut,"CGI: Shield thickness            = %g %s\n",
	    line->tshield/line->units_abct->sf, line->units_abct->name);
    fprintf(cgiOut,"CGI:                             = %g m\n",
	    line->tshield);
    fprintf(cgiOut,"CGI: Center metal resistivity    = %g %s\n",
	    line->rho_a/line->units_rho->sf, line->units_rho->name);
    fprintf(cgiOut,"CGI: Shield metal resistivity    = %g %s\n",
	    line->rho_b/line->units_rho->sf, line->units_rho->name);
    fprintf(cgiOut,"CGI: Relative dielectric const.  = %g \n", line->er);
    fprintf(cgiOut,"CGI: Dielectric loss tangent     = %g \n", line->tand);
    fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	    line->freq/line->units_freq->sf, line->units_freq->name);
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
    fprintf(cgiOut, "<pre>");
    coax_calc(line, line->freq);
    fprintf(cgiOut, "</pre>\n");

    break;

  case SYNTH_A:
    fprintf(cgiOut, "<pre>");
    coax_syn(line, line->freq, COAXSYN_A);
    fprintf(cgiOut, "</pre>\n");
    break;

  case SYNTH_B:
    fprintf(cgiOut, "<pre>");
    coax_syn(line, line->freq, COAXSYN_B);
    fprintf(cgiOut, "</pre>\n");
    break;

  case SYNTH_C:
    fprintf(cgiOut, "<pre>");
    coax_syn(line, line->freq, COAXSYN_C);
    fprintf(cgiOut, "</pre>\n");
    break;

  case SYNTH_ER:
    fprintf(cgiOut, "<pre>");
    coax_syn(line, line->freq, COAXSYN_ER);
    fprintf(cgiOut, "</pre>\n");
    break;

  }

  /* include the HTML output */
#include "header_html.c"
#include "coax_html.c"
#include "footer_html.c"
	
  return 0;
}

