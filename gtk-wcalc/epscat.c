/* $Id: epscat.c,v 1.4 2002/06/12 11:30:11 dan Exp $ */

/*
 * Copyright (C) 2001, 2002 Dan McMahill
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

#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"
#include "epscat.h"

#define MAXLINELEN 80
#define FIELDSEP " \t=\n"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static int eps_isps(FILE *fp)
{
  char line[MAXLINELEN+1];
  
  /* 
   * make sure its postscript 
   */
  if ( fgets(line,MAXLINELEN,fp) == NULL ) {
    return -1;
  }

  if ( strncmp("%!PS",line,4) != 0) {
    return -1;
  }
  
  /* put the line back */
  fseek(fp,-strlen(line),SEEK_CUR);

  return 0;
}

static int eps_bbox(FILE *fp,int bbox[])
{
  char line[MAXLINELEN+1];
  char *tok;
  char *p;
  int i;

  /* 
   * make sure its postscript 
   */
  if ( eps_isps(fp) != 0 ) {
    alert("epscat.c:eps_bbox():  NOT Postscript\n");
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
    return -1;
  }
  tok = strtok(line," \t");
  for (i=0 ; i<4 ; i++) {
    tok = strtok(NULL," \t");
    if (tok == NULL) {
      return -1;
    }
    bbox[i] = atoi(tok);
  }

  return 0;
}

/*
 * open the EPS file in 'fname', wrap it in our EPS wrapper
 * and print it to 'ofp'
 */

int eps_cat(char *fname, FILE *ofp)
{
  FILE *fp;
  char line[MAXLINELEN+1];
  char *p;
  int c;
  int bbox[4];

  if ( (fp = fopen(fname,"r")) == NULL){
    alert("Could not open EPS file:\"%s\"\n",fname);
    return -1;
  }
  
  /* 
   * make sure its postscript 
   */
  if ( eps_isps(fp) != 0 ) {
    alert("\"%s\" is NOT a Postscript file!\n",fname);
    fclose(fp);
    return -1;
  }

  /* 
   * extract the bounding box
   */
  if ( eps_bbox(fp,bbox) != 0 ) {
    alert("Could not find BoundingBox in \"%s\"\n",fname);
    fclose(fp);
    return -1;
  }


  /* 
   * Print our EPS wrapper header
   */
  fprintf(ofp,"%% We are currently where the top center of the figure should be.\n");
  fprintf(ofp,"%% First figure out where the text will be when we continue\n");
  fprintf(ofp,"%% after the figure\n");
  fprintf(ofp,"%%\n");
  fprintf(ofp,"currentpoint %d %d sub sub\n",bbox[3],bbox[1]);
  fprintf(ofp,"gsave\n");
  fprintf(ofp,"currentpoint pop %d %d add 2 div sub\n",bbox[0],bbox[2]);

  fprintf(ofp,"currentpoint exch pop %d sub \n",bbox[3]);
  fprintf(ofp,"translate\n");
  fprintf(ofp,"%%\n");
  fprintf(ofp,"%%include the EPS file\n");
  fprintf(ofp,"%%\n");
  fprintf(ofp,"BEGINEPSFILE\n");
  fprintf(ofp,"\n");

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
	fputs(line,ofp);
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
    fputs(line,ofp);
  }

  /* 
   * Print our EPS wrapper footer
   */
  fprintf(ofp,"\n");
  fprintf(ofp,"ENDEPSFILE\n");
  fprintf(ofp,"grestore\n");
  fprintf(ofp,"moveto\n");
  
  
  fclose(fp);
  return 0;
}
