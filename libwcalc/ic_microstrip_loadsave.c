/* $Id: ic_microstrip_loadsave.c,v 1.2 2001/11/11 15:54:22 dan Exp $ */

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

#define FILE_VERSION "0.1"

#define SEC_OTHER 0
#define SEC_STRIP 1
#define SEC_SUBS  2

int ic_microstrip_load(ic_microstrip_line *line, FILE *fp)
{
  char file_line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;

  int i;

  int got_l=0;
  int got_l_sf=0;
  int got_l_units=0;
  int got_w=0;
  int got_w_sf=0;
  int got_w_units=0;
  int got_z0=0;
  int got_len=0;
  int got_h=0;
  int got_h_sf=0;
  int got_h_units=0;
  int got_tox=0;
  int got_tox_sf=0;
  int got_tox_units=0;
  int got_eox=0;
  int got_es=0;
  int got_tmet=0;
  int got_tmet_sf=0;
  int got_tmet_units=0;
  int got_rho=0;
  int got_rho_sf=0;
  int got_rho_units=0;
  int got_rough=0;
  int got_rough_sf=0;
  int got_rough_units=0;
  int got_sigmas=0;
  int got_freq=0;
  int got_freq_sf=0;
  int got_freq_units=0;

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

  while( fgets(file_line,MAXLINELEN,fp) != NULL ){
    tok = strtok(file_line,FIELDSEP);
    if (tok != NULL){
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      /* skip comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,"[ic_microstrip]") == 0){
	    section=SEC_STRIP;
	  }
	  else if (strcmp(tok,"[substrate]") == 0){
	    section=SEC_SUBS;
	  }
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    section=SEC_OTHER;
	  }
	  else if ( (section==SEC_STRIP) || (section==SEC_SUBS) ){
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
	      alert("ic_microstrip_load:  could not read value to go"
		      " with %s=\n",tok);
	      return -1;
	    }
	    
	    if (strcmp(tok,"w") == 0){
	      line->w = atof(val);
	      got_w = 1;
	    }
	    else if (strcmp(tok,"w_sf") == 0){
	      line->w_sf = atof(val);
	      got_w_sf = 1;
	    }
	    else if (strcmp(tok,"w_units") == 0){
	      line->w_units = strdup(val);
	      got_w_units = 1;
	    }
	    else if (strcmp(tok,"l") == 0){
	      line->l = atof(val);
	      got_l = 1;
	    }
	    else if (strcmp(tok,"l_sf") == 0){
	      line->l_sf = atof(val);
	      got_l_sf = 1;
	    }
	    else if (strcmp(tok,"l_units") == 0){
	      line->l_units = strdup(val);
	      got_l_units = 1;
	    }
	    else if (strcmp(tok,"Ro") == 0){
	      line->Ro = atof(val);
	      got_z0 = 1;
	    }
	    else if (strcmp(tok,"elen") == 0){
	      line->len = atof(val);
	      got_len = 1;
	    }
	    else if (strcmp(tok,"h") == 0){
	      line->subs->h = atof(val);
	      got_h = 1;
	    }
	    else if (strcmp(tok,"h_sf") == 0){
	      line->subs->h_sf = atof(val);
	      got_h_sf = 1;
	    }
	    else if (strcmp(tok,"h_units") == 0){
	      line->subs->h_units = strdup(val);
	      got_h_units = 1;
	    }
	    else if (strcmp(tok,"es") == 0){
	      line->subs->es = atof(val);
	      got_es = 1;
	    }
	    else if (strcmp(tok,"eox") == 0){
	      line->subs->eox = atof(val);
	      got_eox = 1;
	    }
	    else if (strcmp(tok,"tox") == 0){
	      line->subs->tox = atof(val);
	      got_tox = 1;
	    }
	    else if (strcmp(tok,"tox_sf") == 0){
	      line->subs->tox_sf = atof(val);
	      got_tox_sf = 1;
	    }
	    else if (strcmp(tok,"tox_units") == 0){
	      line->subs->tox_units = strdup(val);
	      got_tox_units = 1;
	    }
	    else if (strcmp(tok,"tmet") == 0){
	      line->subs->tmet = atof(val);
	      got_tmet = 1;
	    }
	    else if (strcmp(tok,"tmet_sf") == 0){
	      line->subs->tmet_sf = atof(val);
	      got_tmet_sf = 1;
	    }
	    else if (strcmp(tok,"tmet_units") == 0){
	      line->subs->tmet_units = strdup(val);
	      got_tmet_units = 1;
	    }
	    else if (strcmp(tok,"rho") == 0){
	      line->subs->rho = atof(val);
	      got_rho = 1;
	    }
	    else if (strcmp(tok,"rho_sf") == 0){
	      line->subs->rho_sf = atof(val);
	      got_rho_sf = 1;
	    }
	    else if (strcmp(tok,"rho_units") == 0){
	      line->subs->rho_units = strdup(val);
	      got_rho_units = 1;
	    }
	    else if (strcmp(tok,"rough") == 0){
	      line->subs->rough = atof(val);
	      got_rough = 1;
	    }
	    else if (strcmp(tok,"rough_sf") == 0){
	      line->subs->rough_sf = atof(val);
	      got_rough_sf = 1;
	    }
	    else if (strcmp(tok,"rough_units") == 0){
	      line->subs->rough_units = strdup(val);
	      got_rough_units = 1;
	    }
	    else if (strcmp(tok,"sigmas") == 0){
	      line->subs->sigmas = atof(val);
	      got_sigmas = 1;
	    }
	    else if (strcmp(tok,"freq") == 0){
	      line->freq = atof(val);
	      got_freq = 1;
	    }
	    else if (strcmp(tok,"freq_sf") == 0){
	      line->freq_sf = atof(val);
	      got_freq_sf = 1;
	    }
	    else if (strcmp(tok,"freq_units") == 0){
	      line->freq_units = strdup(val);
	      got_freq_units = 1;
	    }
	    else if (strcmp(tok,"file_version") == 0){
	      /* ignore */
	    }
	    else {
	      alert("ic_microstrip_load:  unknown token \"%s\"\n",tok);
	      return -1;
	    }
	  }
	  else {
	    /* 
	     * skip because this is another secion of the file.
	     * probably from global settings
	     */
	  }

	} while ( (tok = strtok(NULL,FIELDSEP)) != NULL );
      }
    }
  }
  
  if (!got_l) {  
    alert("ic_microstrip_load:  missing data: L\n");   
    return -1;  
  }
  if (!got_l_sf) {  
    alert("ic_microstrip_load:  missing data: l scale factor\n");   
    return -1;  
  }
  if (!got_l_units) {  
    alert("ic_microstrip_load:  missing data: l units\n");   
    return -1;  
  }
  if (!got_w) {  
    alert("ic_microstrip_load:  missing data: W\n");   
    return -1;  
  }
  if (!got_w_sf) {  
    alert("ic_microstrip_load:  missing data: w scale factor\n");   
    return -1;  
  }
  if (!got_w_units) {  
    alert("ic_microstrip_load:  missing data: w units\n");   
    return -1;  
  }
  if (!got_z0) {  
    alert("ic_microstrip_load:  missing data: Z0\n");   
    return -1;  
  }
  if (!got_len) {  
    alert("ic_microstrip_load:  missing data: Elen\n");   
    return -1;  
  }
  if (!got_h) {  
    alert("ic_microstrip_load:  missing data: H\n");   
    return -1;  
  }
  if (!got_h_sf) {  
    alert("ic_microstrip_load:  missing data: h scale factor\n");   
    return -1;  
  }
  if (!got_h_units) {  
    alert("ic_microstrip_load:  missing data: h units\n");   
    return -1;  
  }
  if (!got_es) {  
    alert("ic_microstrip_load:  missing data: ES\n");   
    return -1;  
  }
  if (!got_eox) {  
    alert("ic_microstrip_load:  missing data: EOX\n");   
    return -1;  
  }
  if (!got_tox) {  
    alert("ic_microstrip_load:  missing data: TOX\n");   
    return -1;  
  }
  if (!got_tox_sf) {  
    alert("ic_microstrip_load:  missing data: TOX sf\n");   
    return -1;  
  }
  if (!got_tox_units) {  
    alert("ic_microstrip_load:  missing data: TOX units\n");   
    return -1;  
  }
  if (!got_tmet) {  
    alert("ic_microstrip_load:  missing data: TMET\n");   
    return -1;  
  }
  if (!got_tmet_sf) {  
    alert("ic_microstrip_load:  missing data: tmet scale factor\n");   
    return -1;  
  }
  if (!got_tmet_units) {  
    alert("ic_microstrip_load:  missing data: tmet units\n");   
    return -1;  
  }
  if (!got_rho) {  
    alert("ic_microstrip_load:  missing data: RHO\n");   
    return -1;  
  }
  if (!got_rho_sf) {  
    alert("ic_microstrip_load:  missing data: rho scale factor\n");   
    return -1;  
  }
  if (!got_rho_units) {  
    alert("ic_microstrip_load:  missing data: rho units\n");   
    return -1;  
  }
  if (!got_rough) {  
    alert("ic_microstrip_load:  missing data: ROUGH\n");   
    return -1;  
  }
  if (!got_rough_sf) {  
    alert("ic_microstrip_load:  missing data: rough scale factor\n");   
    return -1;  
  }
  if (!got_rough_units) {  
    alert("ic_microstrip_load:  missing data: rough units\n");   
    return -1;  
  }
  if (!got_sigmas) {  
    alert("ic_microstrip_load:  missing data: sigmas\n");   
    return -1;  
  }
  if (!got_freq) {  
    alert("ic_microstrip_load:  missing data: FREQ\n");   
    return -1;  
  }
  if (!got_freq_sf) {  
    alert("ic_microstrip_load:  missing data: freq scale factor\n");   
    return -1;  
  }
  if (!got_freq_units) {  
    alert("ic_microstrip_load:  missing data: freq units\n");   
    return -1;  
  }
  
  return 0;
}


void ic_microstrip_save(ic_microstrip_line *line, FILE *fp, char *fname)
{
  wcalc_save_header(fp, fname, FILE_IC_MICROSTRIP);

  fprintf(fp,"[ic_microstrip]\n");
  fprintf(fp,"\n#\n#file format version\n#\n\n");

  fprintf(fp,"file_version = %s\n\n",FILE_VERSION);
  /*
   * The input values
   */

  fprintf(fp,"#\n# Input parameters\n#\n\n");

   fprintf(fp,"# Length (meters)\n");
  fprintf(fp,"L = %g\n",line->l);

  fprintf(fp,"# Width (meters)\n");
  fprintf(fp,"W = %g\n",line->w);

  fprintf(fp,"# Characteristic Impedance (ohms)\n");
  fprintf(fp,"Z0 = %g\n",line->Ro);

  fprintf(fp,"# Electrical Length (degrees)\n");
  fprintf(fp,"Elen = %g\n",line->len);

  fprintf(fp,"# Operating frequency (Hertz)\n");
  fprintf(fp,"freq = %g\n",line->freq);

  fprintf(fp,"\n");

  /*
   * The desired user units
   */

  fprintf(fp,"#\n# Desired user units\n#\n\n");

  fprintf(fp,"# Length scale factor (meters/unit)\n");
  fprintf(fp,"l_sf= %g\n",line->l_sf);
  fprintf(fp,"# Length units\n");
  fprintf(fp,"l_units= %s\n",line->l_units);

  fprintf(fp,"# Width scale factor (meters/unit)\n");
  fprintf(fp,"w_sf= %g\n",line->w_sf);
  fprintf(fp,"# Width units\n");
  fprintf(fp,"w_units= %s\n",line->w_units);

  fprintf(fp,"# Frequency scale factor (Hertz/unit)\n");
  fprintf(fp,"freq_sf= %g\n",line->freq_sf);
  fprintf(fp,"# Frequency units\n");
  fprintf(fp,"freq_units= %s\n",line->freq_units);

  fprintf(fp,"[substrate]\n");

  fprintf(fp,"# Height (meters)\n");
  fprintf(fp,"H = %g\n",(line->subs)->h);

  fprintf(fp,"# Relative dielectric constant\n");
  fprintf(fp,"ER = %g\n",(line->subs)->es);

  fprintf(fp,"# Metalization thickness (meters)\n");
  fprintf(fp,"TMET = %g\n",(line->subs)->tmet);

  fprintf(fp,"# Metalization resistivity (ohm-meters)\n");
  fprintf(fp,"RHO = %g\n",(line->subs)->rho);

  fprintf(fp,"# Metalization surface roughness (meters-RMS)\n");
  fprintf(fp,"ROUGH = %g\n",(line->subs)->rough);

  fprintf(fp,"# Substrate conductivity\n");
  fprintf(fp,"sigmas = %g\n",(line->subs)->sigmas);

  fprintf(fp,"\n");

  /*
   * The desired user units
   */

  fprintf(fp,"#\n# Desired user units\n#\n\n");

  fprintf(fp,"# Height scale factor (meters/unit)\n");
  fprintf(fp,"h_sf= %g\n",line->subs->h_sf);
  fprintf(fp,"# Height units\n");
  fprintf(fp,"h_units= %s\n",line->subs->h_units);

  fprintf(fp,"#  Metal thickness scale factor (meters/unit)\n");
  fprintf(fp,"tmet_sf= %g\n",line->subs->tmet_sf);
  fprintf(fp,"#  Metal thickness units\n");
  fprintf(fp,"tmet_units= %s\n",line->subs->tmet_units);

  fprintf(fp,"#  Rho scale factor (ohm-meters/unit)\n");
  fprintf(fp,"rho_sf= %g\n",line->subs->rho_sf);
  fprintf(fp,"#  units\n");
  fprintf(fp,"rho_units= %s\n",line->subs->rho_units);

  fprintf(fp,"#  roughness scale factor (meters/unit)\n");
  fprintf(fp,"rough_sf= %g\n",line->subs->rough_sf);
  fprintf(fp,"#  roughness units\n");
  fprintf(fp,"rough_units= %s\n",line->subs->rough_units);


}
