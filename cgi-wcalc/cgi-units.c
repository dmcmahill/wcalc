/* $Id: cgi-units.c,v 1.3 2002/06/12 11:30:02 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004 Dan McMahill
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

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* CGI specific headers */
#include "cgic.h"

/* libwcalc headers */
#include "alert.h"
#include "misc.h"
#include "units.h"

#include "cgi-units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

char * cgi_units_menu_show(const units_data *units, char *name, int ind)
{
  int i;

  assert(units != NULL);

  if (units[0].name == NULL)
    return "";


  fprintf(cgiOut,"<SELECT NAME=\"%s\">\n",name);
  i = 0;
  while (units[i].name != NULL){
    fprintf(cgiOut,"<OPTION VALUE=\"%s\"",
	    units[i].name);

    if (i == ind)
      fprintf(cgiOut," SELECTED");

    fprintf(cgiOut,">%s\n",units[i].name);
    i++;
  }  
  fprintf(cgiOut,"</SELECT>\n");

  return "";
}

static void cgi_units_submenu(const wc_units_data *units, 
			      char * name, int index)
{
  int i;

  fprintf(cgiOut, "<SELECT NAME=\"%s\">\n", name);

  /* populate the menu */
  i = 0;
  
  while( units[i].name != NULL ) {
    fprintf(cgiOut,"    <OPTION VALUE=\"%s\"", units[i].name);

    if (i == index)
      fprintf(cgiOut," SELECTED");

    fprintf(cgiOut,">%s\n", units[i].name);
    
    i++;
  }
  fprintf(cgiOut, "</SELECT>\n");

}
char * cgi_units_menu(const wc_units *units, char *name)
{
  int i;

  assert(units != NULL);

  /* Create the numerator menus */
  if (units->nnum == 0) {
    fprintf(cgiOut, "1");
  } else {
    for ( i = 0; i < units->nnum; i++) {

      cgi_units_submenu(units->num[i], name, units->numi[i]);

      if (i < (units->nnum - 1)) 
	fprintf(cgiOut, "-\n");
      
    }  
      
  }

  if (units->nden > 0) {
    fprintf(cgiOut, "/\n");
      
      for (i=0; i<units->nden; i++) {
	cgi_units_submenu(units->den[i], name, units->deni[i]);
	
	if (i < (units->nden - 1)) 
	  fprintf(cgiOut, "-\n");
      }
  }

  
  return "";
}

char ** cgi_units_inductance(void)
{
  int i;
  static char ** u=NULL;


  if (u == NULL) {
    i=0;
    while (inductance_units[i].name != NULL){
      i++;
    }
    if ( (u = malloc(i*sizeof(char *))) == NULL ) {
      fprintf(stderr,"cgi_units_inductance():  malloc() failed\n");
      exit(1);
    }
    i=0;
    while (inductance_units[i].name != NULL){
      u[i] = inductance_units[i].name;
      i++;
    }
  }

  return u;
}

