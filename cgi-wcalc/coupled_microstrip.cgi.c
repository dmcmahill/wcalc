/* $Id: coupled_microstrip.cgi.c,v 1.14 2006/03/01 15:31:35 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2006 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/*
 * a cgi interface to the coupled_microstrip calculator
 */

/* #define DEBUG */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

/* CGI specific */
#include "cgic.h"
#include "cgi-common.h"
#include "cgi-units.h"
#include "cookie.h"

/* libwcalc */
#include "coupled_microstrip.h"
#include "coupled_microstrip_loadsave.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

/* ID's for this module */
#include "coupled_microstrip_id.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN  20

/* possible actions we're supposed to take */
#define LOAD      0
#define RESET     1
#define ANALYZE   2
#define SYNTH_W   3



/* defaults for the various parameters */
#define defW      MIL2M(110.0)
#define defS      MIL2M(20.0)
#define defL      MIL2M(1000.0)

#define defTMET   MIL2M(1.4)
#define defRGH    MIL2M(0.05)
#define defRHO    1.72e-8

#define defH      MIL2M(62.0)
#define defES     4.8
#define defTAND   0.01

#define defFREQ   1000.0e6

#define defRO     50.0
#define defK      0.2
#define defZEVEN  60.0
#define defZODD   40.0
#define defELEN   90.0


/* default for z0/k vs even/odd input for synthesis */
#define defSTYPE  0
#define NSTYPE    2
static char *stypeStrings[]={"evod" , "zk"};
	     
static const char *name_string="coupled_microstrip.cgi";
		
int cgiMain(void){

  /* CGI variables */
  char str_action[ACTION_LEN];

  int stype;

  int action;
  int input_err = 0;

  /* coupled_microstrip variables */
  coupled_microstrip_line *line;
  double freq;

  double rho,rough,tmet,w,s,l,h,es,tand;
  double zeven,zodd,k;
  double Ro=0.0;
  double elen;

  /* delay on the line (ns) */
  double delay;
  
  /* these are either "" or "checked\0" */
  char zkchecked[8];
  char evodchecked[8];

  char *cookie_str;
  char cookie_load_str[COOKIE_MAX+1];
  cgiCookieType *cookie;

  cgi_units_menu *menu_lwst;
  cgi_units_menu *menu_freq, *menu_loss, *menu_losslen;
  cgi_units_menu *menu_rho, *menu_rough, *menu_delay, *menu_depth;
  cgi_units_menu *menu_deltal;

  /*
   * uncomment to be able to run in the debugger.
   * access the CGI URL that gives the problem, then change foo.cgi to 
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /* cgiReadEnvironment("/tmp/capcgi.dat"); */

  /* create the coupled_microstrip line */
  line = coupled_microstrip_line_new();

  menu_lwst = cgi_units_menu_new(line->units_lwst);
  menu_freq = cgi_units_menu_new(line->units_freq);
  menu_loss = cgi_units_menu_new(line->units_loss);
  menu_losslen = cgi_units_menu_new(line->units_losslen);
  menu_rho = cgi_units_menu_new(line->units_rho);
  menu_rough = cgi_units_menu_new(line->units_rough);
  menu_delay = cgi_units_menu_new(line->units_delay);
  menu_depth = cgi_units_menu_new(line->units_depth);
  menu_deltal = cgi_units_menu_new(line->units_deltal);

  cgi_units_attach_entry(menu_lwst, "entry_s");
  cgi_units_attach_entry(menu_lwst, "entry_l");
  cgi_units_attach_entry(menu_lwst, "entry_tmet");
  cgi_units_attach_entry(menu_lwst, "entry_h");

  cgi_units_attach_entry(menu_loss, "entry_loss_odd");
  cgi_units_attach_entry(menu_losslen, "entry_losslen_odd");
  cgi_units_attach_entry(menu_deltal, "entry_deltal");

  /* flags to the program: */
  if(cgiFormStringNoNewlines("analyze",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = ANALYZE;
  }
  else if(cgiFormStringNoNewlines("synth_w",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = SYNTH_W;
  }
  else if(cgiFormStringNoNewlines("reset",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    action = RESET;
  }
  else{
    action = LOAD;
  }

  /*
   * extract the parameters from the CGI form and use them to populate
   * the coupled_microstrip structure
   */

  if ( (action != RESET) && (action != LOAD) ) {
    
    cgi_units_menu_read();
        
    /* Metal resistivity */
    if(cgiFormDouble("rho", &rho, defRHO/line->units_rho->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading resistivity");
    }
    if( rho < 0.0 ) {
      rho = defRHO/line->units_rho->sf;
      printFormError("Resistivity may not be negative");
    }

    /* Metal thickness (m) */
    if(cgiFormDouble("tmet", &tmet, defTMET/line->units_lwst->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal thickness");
    }
    if( tmet < 0.0 ) {
      tmet = defTMET/line->units_lwst->sf;
      printFormError("Metal thickness may not be negative");
    }

    /* Metalization roughness */
    if(cgiFormDouble("rough", &rough, defRGH/line->units_rough->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal surface roughness");
    }
    if( rough < 0.0 ) {
      rough = defRGH/line->units_rough->sf;
      printFormError("Metal surface roughness may not be negative");
    }
    
    /* Coupled_Microstrip width */
    if(cgiFormDouble("w", &w, defW/line->units_lwst->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal width");
    }
    if( w <= 0.0 ) {
      w = defW/line->units_lwst->sf;
      printFormError("Metal width must be %gt 0");
    }

    /* Coupled_Microstrip spacing */
    if(cgiFormDouble("s", &s, defS/line->units_lwst->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading metal width");
    }
    if( s <= 0.0 ) {
      s = defS/line->units_lwst->sf;
      printFormError("Metal spacing must be %gt 0");
    }
    
    /* Coupled_Microstrip length */
    if(cgiFormDouble("l", &l, defL/line->units_lwst->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading coupled microstrip length");
    }
    if( l <= 0.0 ) {
      l = defL/line->units_lwst->sf;
      printFormError("Metal length must be %gt 0");
    }

    /* Substrate dielectric thickness */
    if(cgiFormDouble("h", &h, defH/line->units_lwst->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading substrate height");
    }
    if( h <= 0.0 ) {
      h = defH/line->units_lwst->sf;
      printFormError("Substrate height must be %gt 0");
    }
    
    /* Substrate relative permittivity */
    if(cgiFormDoubleBounded("es", &es, 1.0, 1000.0, defES) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Substrate permitivity out of range");
    }

    /* Substrate loss tangent */
    if(cgiFormDoubleBounded("tand", &tand, 0.0, 1000.0, defTAND) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Loss tangent out of range");
    }

    /* Frequency of operation */
    if(cgiFormDouble("freq", &freq, defFREQ/line->units_freq->sf) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading frequency");
    }
    if( freq <= 0.0 ) {
      freq = defFREQ/line->units_freq->sf;
      printFormError("Frequency must be %gt 0");
    }


    /* electrical parameters: */
  cgiFormRadio("stype", stypeStrings, NSTYPE, &stype, defSTYPE);

    if (cgiFormRadio("stype", stypeStrings, NSTYPE, 
		     &line->use_z0k, defSTYPE) !=
        cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading z0-k/Zeven-Zodd radio button");
    }

    if(cgiFormDoubleBounded("Ro", &Ro, 0.0001, 1000.0, defRO) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Z0 out of range");
    }

    if(cgiFormDoubleBounded("k", &k, 0.0, 1.0, defK) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Coupling (k) must be in the range 0 %lt k %lt 1");
    }

    if(cgiFormDoubleBounded("zeven", &zeven, 0.0001, 1000.0, defZEVEN) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Z0-even out of range");
    }

    if(cgiFormDoubleBounded("zodd", &zodd, 0.0001, 1000.0, defZODD) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Z0-odd out of range");
    }

    if(cgiFormDouble("elen", &elen, defELEN) !=
       cgiFormSuccess){
      inputErr(&input_err);
      printFormError("Error reading electrical length");
    }
    if( freq <= 0.0 ) {
      elen = defELEN;
      printFormError("Electrical length must be %gt 0");
    }


#ifdef DEBUG
  fprintf(cgiOut,"<PRE>\n");
  fprintf(cgiOut,"CGI:  action = %d\n",action);
  fprintf(cgiOut,"</PRE>\n");
#endif


  /* copy data over to the line structure */
  line->w           = w*line->units_lwst->sf;
  line->s           = s*line->units_lwst->sf;
  line->l           = l*line->units_lwst->sf;
  line->subs->h     = h*line->units_lwst->sf;
  line->subs->tmet  = tmet*line->units_lwst->sf;
  line->subs->rough = rough*line->units_rough->sf;


  line->freq = freq * line->units_freq->sf;
  
  /* copy over the other parameters */
  line->subs->tand  = tand;
  line->subs->er    = es;
  line->subs->rho   = rho*line->units_rho->sf;

  line->len = elen;

  line->z0 = Ro;
  line->k = k;
  line->z0e = zeven;
  line->z0o = zodd;
  
  } /* if ( (action != RESET) && (action != LOAD) ) */
  else {

#ifdef DEBUG
    printf("coupled_microstrip.cgi:  checking for a cookie to load\n");
#endif
    /* load a stored cookie if it exists */
    if(cgiCookieStringNoNewlines(name_string, cookie_load_str, COOKIE_MAX) ==
       cgiCookieSuccess) {
#ifdef DEBUG
    printf("coupled_microstrip.cgi:  loading cookie \"%s\"\n", cookie_load_str);
#endif
      coupled_microstrip_load_string(line, cookie_load_str);
#ifdef DEBUG
    printf("coupled_microstrip.cgi:  finished loading cookie\n");
#endif
    }
    
  }
  
  if (!input_err){
    cookie_str = coupled_microstrip_save_string(line);
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
    fprintf(cgiOut, "<pre>use_z0k = %d\n</pre>\n", line->use_z0k);
#endif

  switch (line->use_z0k){
    case 1:
      sprintf(zkchecked,"checked");
      sprintf(evodchecked," ");
      break;
  case 0:
    sprintf(zkchecked," ");
      sprintf(evodchecked,"checked");
      break;
    default:
      fprintf(cgiOut,"<PRE>\n");
      fprintf(cgiOut,"CGI:  illegal stype (%d)\n", line->use_z0k);
      fprintf(cgiOut,"</PRE>\n");
      exit(1);
      break;
  }

  
#ifdef DEBUG
  fprintf(cgiOut,"<pre>\n");
  fprintf(cgiOut,"CGI: --------------- Coupled_Microstrip  Analysis -----------\n");
  fprintf(cgiOut,"CGI: Metal width                 = %g %s\n",
	  line->w/line->units_lwst->sf, line->units_lwst->name);
  fprintf(cgiOut,"CGI: Trace spacing               = %g %s\n",
	  line->s/line->units_lwst->sf, line->units_lwst->name);
  fprintf(cgiOut,"CGI: Metal length                = %g %s\n",
	  line->l/line->units_lwst->sf, line->units_lwst->name);
  fprintf(cgiOut,"CGI: Metal thickness             = %g %s\n",
	  line->subs->tmet/line->units_lwst->sf, line->units_lwst->name);
  fprintf(cgiOut,"CGI: Metal resistivity           = %g %s\n",
	  line->subs->rho/line->units_rho->sf, line->units_rho->name);
  fprintf(cgiOut,"CGI: Metal surface roughness     = %g %s-rms\n",
	  line->subs->rough/line->units_rough->sf, line->units_rough->name);
  fprintf(cgiOut,"CGI: Substrate thickness         = %g %s\n",
	  line->subs->h/line->units_lwst->sf, line->units_lwst->name);
  fprintf(cgiOut,"CGI: Substrate dielectric const. = %g \n", line->subs->er);
  fprintf(cgiOut,"CGI: Substrate loss tangent      = %g \n", line->subs->tand);
  fprintf(cgiOut,"CGI: Frequency                   = %g %s\n",
	  line->freq/line->units_freq->sf, line->units_freq->name); 
  fprintf(cgiOut,"CGI: -------------- ---------------------- ----------\n");
  fprintf(cgiOut,"</pre>\n");
#endif

  switch (action){
  case ANALYZE:
    /* 
     * in case coupled_microstrip_calc has some error output, surround it
     * with <pre></pre> so we can read it ok.
     */
    fprintf(cgiOut,"<pre>");
    coupled_microstrip_calc(line, line->freq);
    fprintf(cgiOut,"</pre>\n");

    break;

  case SYNTH_W:
    fprintf(cgiOut,"<pre>");
    coupled_microstrip_syn(line, line->freq);
    fprintf(cgiOut,"</pre>\n");
    break;

  }

  /* XXX this last section is bogus */

  /* electrical and physical length */
  elen = line->len;

  /*
   * delay on line (ns)
   * 2 pi f Td = elen pi/180
   * Td = (pi/180) elen/(2 pi f) = elen/(360 f)
   * and in ns,
   * Td = elen/(360 f *1e-9)
   */
  delay = elen /(360.0 * line->freq);

  /* include the HTML output */
#include "header_html.c"
#include "coupled_microstrip_html.c"
#include "footer_html.c"
	
  return 0;
}

