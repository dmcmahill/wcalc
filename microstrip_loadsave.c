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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
 
#include "microstrip.h"

#define MICROSTRIP_FILE_VERSION "v1"
#define WCALC_FILE_VERSION      "v1"

void wcalc_open(char * fname);


void microstrip_save(microstrip_line *mymstrip, char *fname, char *id);
/*
 * opens the file "fname", saves the data associated with the
 * microstrip "mymstrip".  If "id" is non-NULL, the string
 * it points to is written as the first line of the file
 */


void microstrip_load(microstrip_line *mymstrip, char *fname);
/*
 * opens the file "fname", loads the microstrip data contained in it.
 * stores this data in the microstrip_line pointed to by "mymstrip".
 * Note:  the first line of the input file is skipped as it is assumed
 * to contain a version stamp for use by the top level program.
 * the second line is:
 *  #  microstrip:vstr
 * where 'microstrip' indicates that the file contains microstrip data
 * and 'vstr' is a version string in case the microstrip file format
 * needs to change.
 */

static void microstrip_cat(microstrip_line *mymstrip, FILE *fp, char *fname);
/* 
 * fp needs to be a valid file pointer to an open FILE.  if non-NULL,
 * the file name in 'fname' is also printed out.
 * 
 * prints the microstrip data to the stream 'fp'.  Can be 'stdout' for
 * debugging or a file pointer to save to a file.
 */

int main(int argc, char **argv)
{
  microstrip_subs mysubs;
  microstrip_line mymstrip;

  mymstrip.l=100.0;
  mymstrip.w=100.0;
  mymstrip.z0=100.0;
  mymstrip.len=100.0;

  mymstrip.subs=&mysubs;

  mysubs.h=62;
  mysubs.er=4.8;
  mysubs.tmet=1.4;
  mysubs.rho=1.0;
  mysubs.rough=0.055;
  mysubs.tand=0.001;


  microstrip_save(&mymstrip,"test.wc","# wcalc:v1");
  
  printf("Loading microstrip from test2.wc\n");

  microstrip_load(&mymstrip,"test2.wc");
  microstrip_cat(&mymstrip,stdout,"");
  
  wcalc_open("test.wc");
  wcalc_open("test3.wc");

  return 0;

}

void microstrip_save(microstrip_line *mymstrip, char *fname, char *id)
{
  FILE *fp;

  if ( (fp = fopen(fname,"w")) == NULL){
    fprintf(stderr,"microstrip_save:  could not open \"%s\"\n",fname);
    exit(1);
  }
 
  if (id != NULL){
    fprintf(fp,"%s\n",id);
  }
  microstrip_cat(mymstrip, fp, fname);

  fclose(fp);

}


#define MAXLINELEN 80
#define FIELDSEP " \t=\n"

#define SEC_OTHER 0
#define SEC_STRIP 1
#define SEC_SUBS  2

void microstrip_load(microstrip_line *mymstrip, char *fname)
{
  FILE *fp;
  char line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;

  int i;

  int got_l=0;
  int got_w=0;
  int got_z0=0;
  int got_len=0;
  int got_h=0;
  int got_er=0;
  int got_tmet=0;
  int got_rho=0;
  int got_rough=0;
  int got_tand=0;

  const char fsep[]=" \t:\n";

  if ( (fp = fopen(fname,"r")) == NULL){
    fprintf(stderr,"microstrip_load:  could not open \"%s\"\n",fname);
    exit(1);
  }
 
  /* read the wcalc file version */
  fgets(line,MAXLINELEN,fp);

  /* read the model type and version line */
  fgets(line,MAXLINELEN,fp);
  
  tok = strtok(line,fsep);
  if ( (tok = strtok(NULL,fsep)) == NULL){
    fprintf(stderr,"microstrip_load:  could not read microstrip version\n");
    exit(1);
  }

  if(strcmp(tok,"microstrip") != 0){
    fprintf(stderr,"microstrip_load:  Unknown file type: \"%s\"\n",tok);
    exit(1);
  }
  tok = strtok(NULL,fsep);
  if(strcmp(tok,MICROSTRIP_FILE_VERSION) != 0){
    fprintf(stderr,"microstrip_load:  unknown file version: \"%s\"\n",tok);
    exit(1);
  }

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
	  if (strcmp(tok,"[microstrip]") == 0){
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
	      fprintf(stderr,"microstrip_load:  could not read value to go"
		      " with %s= in \"%s\"\n",tok,fname);
	      exit(1);
	    }
	    
	    if (strcmp(tok,"w") == 0){
	      mymstrip->w = atof(val);
	      got_w = 1;
	    }
	    else if (strcmp(tok,"l") == 0){
	      mymstrip->l = atof(val);
	      got_l = 1;
	    }
	    else if (strcmp(tok,"z0") == 0){
	      mymstrip->z0 = atof(val);
	      got_z0 = 1;
	    }
	    else if (strcmp(tok,"elen") == 0){
	      mymstrip->len = atof(val);
	      got_len = 1;
	    }
	    else if (strcmp(tok,"h") == 0){
	      mymstrip->subs->h = atof(val);
	      got_h = 1;
	    }
	    else if (strcmp(tok,"er") == 0){
	      mymstrip->subs->er = atof(val);
	      got_er = 1;
	    }
	    else if (strcmp(tok,"tmet") == 0){
	      mymstrip->subs->tmet = atof(val);
	      got_tmet = 1;
	    }
	    else if (strcmp(tok,"rho") == 0){
	      mymstrip->subs->rho = atof(val);
	      got_rho = 1;
	    }
	    else if (strcmp(tok,"rough") == 0){
	      mymstrip->subs->rough = atof(val);
	      got_rough = 1;
	    }
	    else if (strcmp(tok,"tand") == 0){
	      mymstrip->subs->tand = atof(val);
	      got_tand = 1;
	    }
	    else {
	      fprintf(stderr,"microstrip_load:  unknown token \"%s\"\n",tok);
	      exit(1);
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
    fprintf(stderr,"microstrip_load:  missing data: L\n");   
    exit(1);  
  }
  if (!got_w) {  
    fprintf(stderr,"microstrip_load:  missing data: W\n");   
    exit(1);  
  }
  if (!got_z0) {  
    fprintf(stderr,"microstrip_load:  missing data: Z0\n");   
    exit(1);  
  }
  if (!got_len) {  
    fprintf(stderr,"microstrip_load:  missing data: Elen\n");   
    exit(1);  
  }
  if (!got_h) {  
    fprintf(stderr,"microstrip_load:  missing data: H\n");   
    exit(1);  
  }
  if (!got_er) {  
    fprintf(stderr,"microstrip_load:  missing data: ER\n");   
    exit(1);  
  }
  if (!got_tmet) {  
    fprintf(stderr,"microstrip_load:  missing data: TMET\n");   
    exit(1);  
  }
  if (!got_rho) {  
    fprintf(stderr,"microstrip_load:  missing data: RHO\n");   
    exit(1);  
  }
  if (!got_rough) {  
    fprintf(stderr,"microstrip_load:  missing data: ROUGH\n");   
    exit(1);  
  }
  if (!got_tand) {  
    fprintf(stderr,"microstrip_load:  missing data: TAND\n");   
    exit(1);  
  }
  
  fclose(fp);
}


static void microstrip_cat(microstrip_line *mymstrip, FILE *fp, char *fname)
{
  time_t now;

  now = time(NULL);
 
  fprintf(fp,"# microstrip:%s\n",MICROSTRIP_FILE_VERSION);
  fprintf(fp,"#\n");
  if (fname != NULL){
    fprintf(fp,"# File:      %s\n",fname);
  }
  fprintf(fp,"# Modified:  %s",ctime(&now));
  fprintf(fp,"#\n");
  fprintf(fp,"\n");

  fprintf(fp,"[microstrip]\n");
  fprintf(fp,"# Length (mils)\n");
  fprintf(fp,"L = %g\n",mymstrip->l);

  fprintf(fp,"# Width (mils)\n");
  fprintf(fp,"W = %g\n",mymstrip->w);

  fprintf(fp,"# Characteristic Impedance (ohms)\n");
  fprintf(fp,"Z0 = %g\n",mymstrip->z0);

  fprintf(fp,"# Electrical Length (degrees)\n");
  fprintf(fp,"Elen = %g\n",mymstrip->len);

  fprintf(fp,"\n");

  fprintf(fp,"[substrate]\n");

  fprintf(fp,"# Height (mils)\n");
  fprintf(fp,"H = %g\n",(mymstrip->subs)->h);

  fprintf(fp,"# Relative dielectric constant\n");
  fprintf(fp,"ER = %g\n",(mymstrip->subs)->er);

  fprintf(fp,"# Metalization thickness (mils)\n");
  fprintf(fp,"TMET = %g\n",(mymstrip->subs)->tmet);

  fprintf(fp,"# Metalization resistivity relative to copper\n");
  fprintf(fp,"RHO = %g\n",(mymstrip->subs)->rho);

  fprintf(fp,"# Metalization surface roughness (mils-RMS)\n");
  fprintf(fp,"ROUGH = %g\n",(mymstrip->subs)->rough);

  fprintf(fp,"# Dielectric loss tangent\n");
  fprintf(fp,"TAND = %g\n",(mymstrip->subs)->tand);

  fprintf(fp,"\n");

}

void wcalc_open(char * fname)
{
  FILE *fp;
  char line[MAXLINELEN];
  char *tok;
 
  const char fsep[]=" \t:\n";

  if ( (fp = fopen(fname,"r")) == NULL){
    fprintf(stderr,"wcalc_open:  could not open \"%s\"\n",fname);
    exit(1);
  }
 
  /* read the wcalc file version */
  fgets(line,MAXLINELEN,fp);

  tok = strtok(line,fsep);
  if ( (tok = strtok(NULL,fsep)) == NULL){
    fprintf(stderr,"wcalc_open:  could not read version and file type\n");
    exit(1);
  }

  if(strcmp(tok,"wcalc") != 0){
    fprintf(stderr,"wcalc_open:  Unknown file type: \"%s\"\n",tok);
    exit(1);
  }

  tok = strtok(NULL,fsep);
  if(strcmp(tok,WCALC_FILE_VERSION) != 0){
    fprintf(stderr,"wcalc_open:  unknown file version: \"%s\"\n",tok);
    exit(1);
  }
  
  /* 
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */

  /* read the model type and version */
  fgets(line,MAXLINELEN,fp);
  fclose(fp);

  tok = strtok(line,fsep);
  if ( (tok = strtok(NULL,fsep)) == NULL){
    fprintf(stderr,"wcalc_open:  could not determine model type/version\n");
    exit(1);
  }

  /* figure out what model is in this file */
  if(strcmp(tok,"microstrip") == 0){
    printf("wcalc_open:  microstrip file, ");
  }
  else if(strcmp(tok,"coupled_microstrip") == 0){
    printf("wcalc_open:  microstrip file, ");
  }
  else if(strcmp(tok,"stripline") == 0){
    printf("wcalc_open:  stripline file, ");
  }
  else {
    fprintf(stderr,"wcalc_open:  Unknown file type: \"%s\"\n",tok);
    exit(1);
  }

  if ( (tok = strtok(NULL,fsep)) == NULL){
    fprintf(stderr,"wcalc_open:  could not determine model version\n");
    exit(1);
  }
  printf("version %s\n",tok);

}




