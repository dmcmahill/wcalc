/* $Id: alert.c,v 1.2 2001/09/28 00:25:32 dan Exp $ */

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

#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif


#define MAXLINELEN 80
#define FIELDSEP " \t=\n"

int eps_isps(FILE *fp)
{
  char line[MAXLINELEN+1];
  
  /* 
   * make sure its postscript 
   */
  if ( fgets(line,MAXLINELEN,fp) == NULL ) {
    printf("Could not read first line");
    return -1;
  }

  if ( strncmp("%!PS",line,4) != 0) {
    printf("NOT Postscript\n");
    return -1;
  }
  
  /* put the line back */
  fseek(fp,-strlen(line),SEEK_CUR);

  return 0;
}

int eps_bbox(char *fname,int bbox[])
{
  FILE *fp;
  char line[MAXLINELEN+1];
  char *tok;
  char *p;
  int i;

  if ( (fp = fopen(fname,"r")) == NULL){
    fprintf(stderr,"eps_bbox:  could not open \"%s\"\n",fname);
    exit(1);
  }
  
  /* 
   * make sure its postscript 
   */
  if ( eps_isps(fp) != 0 ) {
    printf("NOT Postscript\n");
    return -1;
  }

  /* 
   * extract BoundingBox
   */
  p = fgets(line,MAXLINELEN,fp);
  while ( (strncmp("%%BoundingBox:",line,14) != 0) && (p != NULL) ) {
    p = fgets(line,MAXLINELEN,fp);
  }
  if ( p == NULL ) {
    printf("No BoundingBox found (are you sure this is EPS?)\n");
    fclose(fp);
    return -1;
  }
  tok = strtok(line," \t");
  for (i=0 ; i<4 ; i++) {
    tok = strtok(NULL," \t");
    if (tok == NULL) {
      printf("Error reading BoundingBox data\n");
      fclose(fp);
      return -1;
    }
    bbox[i] = atoi(tok);
  }

  fclose(fp);

  return 0;
}

int eps_cat(char *fname)
{
  FILE *fp;
  char line[MAXLINELEN+1];
  char *p;
  int c;

  if ( (fp = fopen(fname,"r")) == NULL){
    fprintf(stderr,"eps_cat:  could not open \"%s\"\n",fname);
    exit(1);
  }
  
  /* 
   * make sure its postscript 
   */
  if ( eps_isps(fp) != 0 ) {
    printf("NOT Postscript\n");
    return -1;
  }


  /* 
   * spit out the file eating any
   * PageBoundingBox lines
   */

  p = line;
  while( (c = getc(fp)) != EOF ) {
    *p = (char) c;
    if ( (p == line+MAXLINELEN) || (*p == '\n') ){
      p++;
      *p = '\0';
      if ( strncmp("%%PageBoundingBox:",line,18) != 0){
	fputs(line,stdout);
      }
      p = line;
    }
    else {
      p++;
    }
  }
  
  /* flush anything left in our buffer */
  if ( (p > line) &&
       (strncmp("%%PageBoundingBox:",line,18) != 0) ) {
    *p = '\0';
    fputs(line,stdout);
  }
  
  return 0;
}


int main(int argc, char **argv)
{
  int bbox[4];

  
  if (argc == 1)
    eps_bbox("microstrip.eps",bbox);
  else
    eps_bbox(argv[1],bbox);

  printf("BoundingBox = (%d, %d) (%d, %d)\n",
	 bbox[0],
	 bbox[1],
	 bbox[2],
	 bbox[3]);

  if (argc == 1)
    eps_cat("microstrip.eps");
  else
    eps_cat(argv[1]);

  return 0;
}
