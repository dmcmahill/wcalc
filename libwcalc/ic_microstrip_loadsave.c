/* $Id: ic_microstrip_loadsave.c,v 1.2 2001/11/25 16:33:29 dan Exp $ */

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

/* #define DEBUG */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <sys/types.h>
#include <time.h>

#include "alert.h"
#include "ic_microstrip.h"
#include "ic_microstrip_loadsave.h"
#include "wcalc_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FILE_VERSION "0.1"


#define LINE_SPEC      0
#define SUBSTRATE_SPEC 1


static fspec * get_fspec(int which_one)
{
  static fspec *linespec=NULL;
  static fspec *subspec=NULL;
  ic_microstrip_line *line=0;
  ic_microstrip_subs *subs=0;

  if (linespec == NULL) {
    /* Build up the list which describes the file format */

    linespec=fspec_add_sect(NULL,"ic_microstrip");
    fspec_add_key(linespec,"file_version","I.C. microstrip file version",'f',FILE_VERSION);

    fspec_add_key(linespec,"L","Length (meters)",'d',&line->l);
    fspec_add_key(linespec,"W","Width (meters)",'d',&line->w);
    fspec_add_key(linespec,"Ro","Characteristic Impedance (ohms)",'d',&line->Ro);
    fspec_add_key(linespec,"Elen","Electrical Length (degrees)",'d',&line->len);
    fspec_add_key(linespec,"freq","Frequency of operation",'d',&line->freq);

    /*
     * The desired user units
     */
    fspec_add_comment(linespec,"Desired user units and associated scale factors");

    fspec_add_key(linespec,"l_sf","Length scale factor (meters/unit)",'d',&line->l_sf);
    fspec_add_key(linespec,"l_units","Length units",'s',&line->l_units);

    fspec_add_key(linespec,"w_sf","Width scale factor (meters/unit)",'d',&line->w_sf);
    fspec_add_key(linespec,"w_units","Width units",'s',&line->w_units);

    
    fspec_add_key(linespec,"freq_sf","Frequency scale factor (Hz/unit)",'d',&line->freq_sf);
    fspec_add_key(linespec,"freq_units","Frequency units",'s',&line->freq_units);
  }

  if (subspec == NULL) {
    /* Build up the list which describes the file format */

    subspec=fspec_add_sect(NULL,"substrate");

    fspec_add_key(subspec,"TMET","Metalization thickness (meters)",'d',&subs->tmet);
    fspec_add_key(subspec,"RHO","Metalization resistivity relative to copper",'d',&subs->rho);
    fspec_add_key(subspec,"ROUGH","Metalization surface roughness (meters-RMS)",'d',&subs->rough);
    fspec_add_key(subspec,"Eox","Oxide relative dielectric constant",'d',&subs->eox);
    fspec_add_key(subspec,"Tox","Oxide thickness (meters)",'d',&subs->tox);
    fspec_add_key(subspec,"H","Substrate thickness (meters)",'d',&subs->h);
    fspec_add_key(subspec,"Es","Substrate relative dielectric constant",'d',&subs->es);
    fspec_add_key(subspec,"sigmas","Substrate conductivity (1/(ohm-meters))",'d',&subs->sigmas);

    /*
     * The desired user units
     */
    fspec_add_comment(subspec,"Desired user units and associated scale factors");

    fspec_add_key(subspec,"tmet_sf","Tmet scale factor (meters/unit)",'d',&subs->tmet_sf);
    fspec_add_key(subspec,"tmet_units","Tmet units",'s',&subs->tmet_units);

    fspec_add_key(subspec,"rho_sf","Resistivity scale factor (ohm-meters/unit)",'d',&subs->rho_sf);
    fspec_add_key(subspec,"rho_units","Resistivity units",'s',&subs->rho_units);

    fspec_add_key(subspec,"rough_sf"," scale factor (meters/unit)",'d',&subs->rough_sf);
    fspec_add_key(subspec,"rough_units"," units",'s',&subs->rough_units);

    fspec_add_key(subspec,"tox_sf","Oxide thickness scale factor (meters/unit)",'d',&subs->tox_sf);
    fspec_add_key(subspec,"tox_units","Oxide thickness units",'s',&subs->tox_units);

    fspec_add_key(subspec,"h_sf","Substrate thickness scale factor (meters/unit)",'d',&subs->h_sf);
    fspec_add_key(subspec,"h_units","Substrate thickness units",'s',&subs->h_units);

    fspec_add_key(subspec,"sigmas_sf",
		  "Substrate conductivity scale factor (siemens/meter/unit)",'d',&subs->sigmas_sf);
    fspec_add_key(subspec,"sigmas_units","Substrate thickness units",'s',&subs->sigmas_units);

  }

  if (which_one == LINE_SPEC)
    return linespec;
  else
    return subspec;
}

int ic_microstrip_load(ic_microstrip_line *line, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt,tmpi;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val=file_read_val(fp,"[ic_microstrip]","file_version")) == NULL ){
    alert("Could not determine the ic_microstrip file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("ic_microstrip_loadsave.c:ic_microstrip_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */


  myspec=get_fspec(LINE_SPEC);
  rslt=fspec_read_file(myspec,fp,(unsigned long) line);
  rewind(fp);
  myspec=get_fspec(SUBSTRATE_SPEC);
  tmpi=fspec_read_file(myspec,fp,(unsigned long) line->subs);

  if (tmpi != 0)
    rslt=tmpi;

  return rslt;
}


void ic_microstrip_save(ic_microstrip_line *line, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_IC_MICROSTRIP);

  myspec=get_fspec(LINE_SPEC);
  fspec_write_file(myspec,fp,(unsigned long) line);

  myspec=get_fspec(SUBSTRATE_SPEC);
  fspec_write_file(myspec,fp,(unsigned long) line->subs);
}
