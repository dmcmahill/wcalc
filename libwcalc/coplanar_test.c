/* $Id: coplanar_test.c,v 1.2 2007/11/27 16:55:43 dan Exp $ */

/*
 * Copyright (C) 2006, 2007 Dan McMahill
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
#ifdef notdef
  /* inches to meters */
  double sf=0.0254;
  double a=7;
  double b[]={3.040,3.038,3.026,3.040,3.040,3.040};
  double c[]={0,1,2,1,2,3};
  int i;
  int npts = sizeof(b)/sizeof(double);
  double freq;
  char rline[MAXLINELEN];
  char *tok;
  FILE *fp;
#endif
  char *str, *str2;

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
