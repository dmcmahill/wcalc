/* $Id: air_coil_loadsave.c,v 1.4 2001/11/03 04:12:27 dan Exp $ */

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

#include "air_coil.h"
#include "air_coil_loadsave.h"
#include "alert.h"
#include "wcalc_loadsave.h"

#define FILE_VERSION "0.1"


#define SEC_OTHER 0
#define SEC_COIL 1


int air_coil_load(air_coil_coil *coil, FILE *fp)
{
  char line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;

  int i;

  int got_Nf=0;
  int got_len=0;
  int got_fill=0;
  int got_AWGf=0;
  int got_rho=0;
  int got_dia=0;
  int got_L=0;
  int got_freq=0;

  int got_use_fill=0;

  int got_len_sf=0;
  int got_len_units=0;
  int got_dia_sf=0;
  int got_dia_units=0;
  int got_L_sf=0;
  int got_L_units=0;
  int got_SRF_sf=0;
  int got_SRF_units=0;
  int got_freq_sf=0;
  int got_freq_units=0;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val=file_read_val(fp,"[air_coil]","file_version")) == NULL ){
    alert("Could not determine the air_coil file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("air_coil_loadsave.c:air_coil_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */

  while( fgets(line,MAXLINELEN,fp) != NULL ){
    tok = strtok(line,FIELDSEP);
    if (tok != NULL){
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      /* skip comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,"[air_coil]") == 0){
	    section=SEC_COIL;
	  }
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    section=SEC_OTHER;
	  }
	  else if ( section==SEC_COIL ){
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
	      alert("air_coil_load:  could not read value to go"
		      " with %s=.\n",tok);
	      return -1;
	    }
	
	    if (strcmp(tok,"nf") == 0){
	      coil->Nf = atof(val);
	      got_Nf = 1;
	    }
	    else if (strcmp(tok,"len") == 0){
	      coil->len = atof(val);
	      got_len = 1;
	    }
	    else if (strcmp(tok,"fill") == 0){
	      coil->fill = atof(val);
	      got_fill = 1;
	    }
	    else if (strcmp(tok,"awgf") == 0){
	      coil->AWGf = atof(val);
	      got_AWGf = 1;
	    }
	    else if (strcmp(tok,"rho") == 0){
	      coil->rho = atof(val);
	      got_rho = 1;
	    }
	    else if (strcmp(tok,"dia") == 0){
	      coil->dia = atof(val);
	      got_dia = 1;
	    }
	    else if (strcmp(tok,"l") == 0){
	      coil->L = atof(val);
	      got_L = 1;
	    }
	    else if (strcmp(tok,"freq") == 0){
	      coil->freq = atof(val);
	      got_freq = 1;
	    }
	    else if (strcmp(tok,"use_fill") == 0){
	      coil->use_fill = atoi(val);
	      got_use_fill = 1;
	    }
	    else if (strcmp(tok,"len_sf") == 0){
	      coil->len_sf = atof(val);
	      got_len_sf = 1;
	    }
	    else if (strcmp(tok,"len_units") == 0){
	      coil->len_units = strdup(val);
	      got_len_units = 1;
	    }
	    else if (strcmp(tok,"dia_sf") == 0){
	      coil->dia_sf = atof(val);
	      got_dia_sf = 1;
	    }
	    else if (strcmp(tok,"dia_units") == 0){
	      coil->dia_units = strdup(val);
	      got_dia_units = 1;
	    }
	    else if (strcmp(tok,"l_sf") == 0){
	      coil->L_sf = atof(val);
	      got_L_sf = 1;
	    }
	    else if (strcmp(tok,"l_units") == 0){
	      coil->L_units = strdup(val);
	      got_L_units = 1;
	    }
	    else if (strcmp(tok,"srf_sf") == 0){
	      coil->SRF_sf = atof(val);
	      got_SRF_sf = 1;
	    }
	    else if (strcmp(tok,"srf_units") == 0){
	      coil->SRF_units = strdup(val);
	      got_SRF_units = 1;
	    }
	    else if (strcmp(tok,"freq_sf") == 0){
	      coil->freq_sf = atof(val);
	      got_freq_sf = 1;
	    }
	    else if (strcmp(tok,"freq_units") == 0){
	      coil->freq_units = strdup(val);
	      got_freq_units = 1;
	    }
	    else if (strcmp(tok,"file_version") == 0){
	      /* ignore */
	    }
	    else {
	      alert("air_coil_load:  unknown token \"%s\"\n",tok);
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

  if (!got_Nf) {
    alert("air_coil_load:  missing data: Nf\n");
    return -1;
  }
  if (!got_len) {
    alert("air_coil_load:  missing data: len\n");
    return -1;
  }
  if (!got_AWGf) {
    alert("air_coil_load:  missing data: AWGf\n");
    return -1;
  }
  if (!got_rho) {
    alert("air_coil_load:  missing data: rho\n");
    return -1;
  }
  if (!got_dia) {
    alert("air_coil_load:  missing data: dia\n");
    return -1;
  }
  if (!got_L) {
    alert("air_coil_load:  missing data: L\n");
    return -1;
  }
  if (!got_freq) {
    alert("air_coil_load:  missing data: freq\n");
    return -1;
  }
  if (!got_use_fill) {
    alert("air_coil_load:  missing data: use_fill\n");
    return -1;
  }

  if (!got_len_sf) {
    alert("air_coil_load:  missing data: len_sf\n");
    return -1;
  }
  if (!got_len_units) {
    alert("air_coil_load:  missing data: len_units\n");
    return -1;
  }

  if (!got_dia_sf) {
    alert("air_coil_load:  missing data: dia_sf\n");
    return -1;
  }
  if (!got_dia_units) {
    alert("air_coil_load:  missing data: dia_units\n");
    return -1;
  }

  if (!got_L_sf) {
    alert("air_coil_load:  missing data: L_sf\n");
    return -1;
  }
  if (!got_L_units) {
    alert("air_coil_load:  missing data: L_units\n");
    return -1;
  }

  if (!got_SRF_sf) {
    alert("air_coil_load:  missing data: SRF_sf\n");
    return -1;
  }
  if (!got_SRF_units) {
    alert("air_coil_load:  missing data: SRF_units\n");
    return -1;
  }

  if (!got_freq_sf) {
    alert("air_coil_load:  missing data: freq_sf\n");
    return -1;
  }
  if (!got_freq_units) {
    alert("air_coil_load:  missing data: freq_units\n");
    return -1;
  }

  return 0;
}


void air_coil_save(air_coil_coil *coil, FILE *fp, char *fname)
{
  wcalc_save_header(fp, fname, FILE_AIR_COIL);

  fprintf(fp,"[air_coil]\n");
  fprintf(fp,"\n#\n#file format version\n#\n\n");

  fprintf(fp,"file_version = %s\n\n",FILE_VERSION);
  /*
   * The input values
   */

  fprintf(fp,"#\n# Input parameters\n#\n\n");

  fprintf(fp,"# Number of turns\n");
  fprintf(fp,"Nf = %g\n",coil->Nf);

  fprintf(fp,"# Length of coil (meters)\n");
  fprintf(fp,"len = %g\n",coil->len);

  fprintf(fp,"# Ratio of coil length to close wound length\n");
  fprintf(fp,"fill = %g\n",coil->fill);

  fprintf(fp,"# Wire size (AWG)\n");
  fprintf(fp,"AWGf = %g\n",coil->AWGf);

  fprintf(fp,"# Wire resistivity (ohms/meter)\n");
  fprintf(fp,"rho = %g\n",coil->rho);

  fprintf(fp,"# Inside diameter of coil (meters)\n");
  fprintf(fp,"dia = %g\n",coil->dia);

  fprintf(fp,"# Desired Inductance (H)\n");
  fprintf(fp,"L = %g\n",coil->L);

  fprintf(fp,"# Frequency of operation (Hz)\n");
  fprintf(fp,"freq = %g\n",coil->freq);

  fprintf(fp,"\n");

  fprintf(fp,"# Use fill to calculate length?\n");
  fprintf(fp,"use_fill = %d\n",coil->use_fill);

  fprintf(fp,"\n");

  /*
   * The desired user units
   */

  fprintf(fp,"#\n# Desired user units\n#\n\n");

  fprintf(fp,"# Length scale factor (meters/unit)\n");
  fprintf(fp,"len_sf= %g\n",coil->len_sf);
  fprintf(fp,"# Length units\n");
  fprintf(fp,"len_units= %s\n",coil->len_units);

  fprintf(fp,"# Diameter scale factor (meters/unit)\n");
  fprintf(fp,"dia_sf= %g\n",coil->dia_sf);
  fprintf(fp,"# Diameter units\n");
  fprintf(fp,"dia_units= %s\n",coil->dia_units);

  fprintf(fp,"# Inductance scale factor (H/unit)\n");
  fprintf(fp,"L_sf= %g\n",coil->L_sf);
  fprintf(fp,"# Inductance units\n");
  fprintf(fp,"L_units= %s\n",coil->L_units);

  fprintf(fp,"# Self resonant frequency scale factor (Hz/unit)\n");
  fprintf(fp,"SRF_sf= %g\n",coil->SRF_sf);
  fprintf(fp,"# Self resonant frequency units\n");
  fprintf(fp,"SRF_units= %s\n",coil->SRF_units);

  fprintf(fp,"# Frequency scale factor (Hz/unit)\n");
  fprintf(fp,"freq_sf= %g\n",coil->freq_sf);
  fprintf(fp,"# Frequency units\n");
  fprintf(fp,"freq_units= %s\n",coil->freq_units);

  fprintf(fp,"\n");
}
