/* $Id: coplanar_test.c,v 1.3 2004/08/31 21:38:23 dan Exp $ */

/*
 * Copyright (c) 2006 Dan McMahill
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "coplanar.h"
#include "coplanar_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FIELDSEP " \t"
#define MAXLINELEN 256

int main(int argc, char **argv)
{
  /* inches to meters */
  double sf=0.0254;
#ifdef notdef
  double a=7;
  double b[]={3.040,3.038,3.026,3.040,3.040,3.040};
  double c[]={0,1,2,1,2,3};
  int i;
  int npts = sizeof(b)/sizeof(double);
#endif
  double freq;
  char *str, *str2;
  char rline[MAXLINELEN];
  char *tok;
  FILE *fp;

  coplanar_line *line, *line2;

  line = coplanar_line_new();
  line2 = coplanar_line_new();
  line2->w=500;
  line2->subs->er=9.3;

  str = coplanar_save_string(line);
  printf("Example of coplanar_save_string() output:\n\"%s\"\n\n",str);

  str2 = coplanar_save_string(line2);
  printf("Example of coplanar_save_string() output:\n\"%s\"\n\n",str2);

  coplanar_load_string(line2, str);
  str2 = coplanar_save_string(line2);
  printf("Example of coplanar_save_string() output:\n\"%s\"\n\n",str2);

  if(strcmp(str, str2) != 0) {
    printf("ERROR:  str != str2\n");
    printf("%s\n%s\n", str, str2);
  }

	printf("W= %8.4f, S= %8.4f, H= %8.4f, Tmet= %8.4f, Er= %8.4f, Freq= %8.4f\n",
	       line->w, 
	       line->s, 
	       line->subs->h,
	       line->subs->tmet,
	       line->subs->er,
	       line->freq);
	
	printf("Z0= %8.4f Ohms, Keff= %8.5f, Loss= %8.5f dB, DeltaL= %8.5f\n",
	       line->z0,
	       line->keff,
	       line->loss,
	       line->deltal);
  
  return 0;
}