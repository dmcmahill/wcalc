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
 

#define WCALC_FILE_VERSION      "v1"

void wcalc_open(char * fname);


/*
 */
void wcalc_save_header(wcalc_line *mymstrip, char *fname, char *id);


/*
 * Loads the data contained in 
 */
void wcalc_open(FILE *fp);

/* 
 * fp needs to be a valid file pointer to an open FILE.  if non-NULL,
 * the file name in 'fname' is also printed out.
 * 
 * prints the wcalc data to the stream 'fp'.  Can be 'stdout' for
 * debugging or a file pointer to save to a file.
 */
static void wcalc_cat(wcalc_line *mymstrip, FILE *fp, char *fname);

int main(int argc, char **argv)
{
  wcalc_subs mysubs;
  wcalc_line mymstrip;

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


  wcalc_save(&mymstrip,"test.wc","# wcalc:v1");
  
  printf("Loading wcalc from test2.wc\n");

  wcalc_load(&mymstrip,"test2.wc");
  wcalc_cat(&mymstrip,stdout,"");
  
  wcalc_open("test.wc");
  wcalc_open("test3.wc");

  return 0;

}

void wcalc_save(wcalc_line *mymstrip, char *fname, char *id)
{
  FILE *fp;

  if ( (fp = fopen(fname,"w")) == NULL){
    fprintf(stderr,"wcalc_save:  could not open \"%s\"\n",fname);
    exit(1);
  }
 
  if (id != NULL){
    fprintf(fp,"%s\n",id);
  }
  wcalc_cat(mymstrip, fp, fname);

  fclose(fp);

}


#define MAXLINELEN 80
#define FIELDSEP " \t=\n"

#define SEC_OTHER 0
#define SEC_STRIP 1
#define SEC_SUBS  2

void wcalc_load(wcalc_line *mymstrip, char *fname)
{
  FILE *fp;
  char line[MAXLINELEN];
  char *tok, *val;
  int section=SEC_OTHER;

  int i;

  int got_l=0;
  int got_w=0;

  const char fsep[]=" \t:\n";

  if ( (fp = fopen(fname,"r")) == NULL){
    fprintf(stderr,"wcalc_load:  could not open \"%s\"\n",fname);
    exit(1);
  }
 
  /* read the wcalc file version */
  fgets(line,MAXLINELEN,fp);

  /* read the model type and version line */
  fgets(line,MAXLINELEN,fp);
  
  tok = strtok(line,fsep);
  if ( (tok = strtok(NULL,fsep)) == NULL){
    fprintf(stderr,"wcalc_load:  could not read wcalc version\n");
    exit(1);
  }

  if(strcmp(tok,"wcalc") != 0){
    fprintf(stderr,"wcalc_load:  Unknown file type: \"%s\"\n",tok);
    exit(1);
  }
  tok = strtok(NULL,fsep);
  if(strcmp(tok,WCALC_FILE_VERSION) != 0){
    fprintf(stderr,"wcalc_load:  unknown file version: \"%s\"\n",tok);
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
	  if (strcmp(tok,"[wcalc]") == 0){
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
	      fprintf(stderr,"wcalc_load:  could not read value to go"
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
	    else {
	      fprintf(stderr,"wcalc_load:  unknown token \"%s\"\n",tok);
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
    fprintf(stderr,"wcalc_load:  missing data: L\n");   
    exit(1);  
  }
  if (!got_w) {  
    fprintf(stderr,"wcalc_load:  missing data: W\n");   
    exit(1);  
  }
  
  fclose(fp);
}


static void wcalc_cat(wcalc_line *mymstrip, FILE *fp, char *fname)
{
  time_t now;

  now = time(NULL);
 
  fprintf(fp,"# wcalc:%s\n",WCALC_FILE_VERSION);
  fprintf(fp,"#\n");
  if (fname != NULL){
    fprintf(fp,"# File:      %s\n",fname);
  }
  fprintf(fp,"# Modified:  %s",ctime(&now));
  fprintf(fp,"#\n");
  fprintf(fp,"\n");

  fprintf(fp,"[wcalc]\n");
  fprintf(fp,"# Length (mils)\n");
  fprintf(fp,"L = %g\n",mymstrip->l);


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
  if(strcmp(tok,"wcalc") == 0){
    printf("wcalc_open:  wcalc file, ");
  }
  else if(strcmp(tok,"coupled_wcalc") == 0){
    printf("wcalc_open:  wcalc file, ");
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




