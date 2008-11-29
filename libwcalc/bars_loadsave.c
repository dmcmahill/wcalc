/* $Id: bars_loadsave.c,v 1.1 2008/07/02 15:03:52 dan Exp $ */

/*
 * Copyright (C) 2008 Dan McMahill
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

#include "bars.h"
#include "bars_loadsave.h"
#include "alert.h"
#include "wcalc_loadsave.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FILE_VERSION "0.1"

static fspec * get_fspec(void)
{
  static fspec *myspec=NULL;
  bars *b = 0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"bars");
    fspec_add_key(myspec, "file_version","Bars file version",
		  'f', FILE_VERSION);

    fspec_add_key(myspec, "a","Bar #1 width (x-dimension, meters)",
		  'd', &b->a);
    fspec_add_key(myspec, "b","Bar #1 height (y-dimension, meters)",
		  'd', &b->b);
    fspec_add_key(myspec, "l1","Bar #1 length (z-dimension, meters)",
		  'd', &b->l1);

    fspec_add_key(myspec, "c","Bar #2 width (x-dimension, meters)",
		  'd', &b->c);
    fspec_add_key(myspec, "d","Bar #2 height (y-dimension, meters)",
		  'd', &b->d);
    fspec_add_key(myspec, "l2","Bar #2 length (z-dimension, meters)",
		  'd', &b->l2);

    fspec_add_key(myspec, "E","Bar #2 x offset (meters)",
		  'd', &b->E);
    fspec_add_key(myspec, "P","Bar #2 y offset (meters)",
		  'd', &b->P);
    fspec_add_key(myspec, "l3","Bar #2 z offset (meters)",
		  'd', &b->l3);

    fspec_add_key(myspec, "L1","Bar #1 self inductance (Henries)",
		  'd', &b->L1);
    fspec_add_key(myspec, "L2","Bar #2 self inductance (Henries)",
		  'd', &b->L2);
    fspec_add_key(myspec, "M","Mutual inductance (Henries)",
		  'd', &b->M);
    fspec_add_key(myspec, "k","Coupling coefficient",
		  'd', &b->k);

    fspec_add_key(myspec, "freq","Frequency of operation (Hz)",
		  'd', &b->freq);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec, "Desired user units and associated scale factors");
    fspec_add_key(myspec, "units_xy", "Width/Height units",  
		  'u', &b->units_xy);
    fspec_add_key(myspec, "units_z", "Length units",  
		  'u', &b->units_z);
    fspec_add_key(myspec, "units_L", "Inductance units",  
		  'u', &b->units_L);
    fspec_add_key(myspec, "units_freq", "Frequency units",  
		  'u', &b->units_freq);

  }

  return myspec;
}

int bars_load(bars *b, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val = file_read_val(fp, "[bars]", "file_version")) == NULL ){
    alert("Could not determine the bars file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("bars_loadsave.c:bars_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp(val, FILE_VERSION) != 0) {
    alert("Unable to load a wcalc bars file with bars file version\n"
	  "\"%s\".  I only understand version \"%s\"\n", 
	  val, FILE_VERSION);
    return -1;
  }
  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */

  myspec = get_fspec();
  rslt = fspec_read_file(myspec, fp,(unsigned long) b);

  return rslt;
}


void bars_save(bars *b, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_BARS);
  myspec=get_fspec();
  fspec_write_file(myspec,fp,(unsigned long) b);
}

int bars_load_string(bars *b, const char *str)
{
  fspec *myspec;
  char *val;
  char *mystr;
  int rslt;

  assert(str!=NULL);

#ifdef DEBUG
  printf("bars_loadsave.c:bars_load_string():  loading \"%s\"\n",str);
#endif

  mystr = strdup(str);

  /* XXX fixme*/
  val = strtok(mystr," ");

  free(mystr);

  /* read the model version  */
  if ( val == NULL ){
    alert("Could not determine the bars file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("bars_loadsave.c:bars_load_string():  "
	 "Got file_version=\"%s\"\n",
	 val);
#endif
  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style formats.
   */

  myspec = get_fspec();
#ifdef DEBUG
  printf("bars_loadsave.c:bars_load_string():  loading \"%s\"\n",str);
#endif
  rslt = fspec_read_string(myspec, str, (unsigned long) b);
  if (rslt != 0) {
	return rslt;
  }

  return rslt;
}


char * bars_save_string(bars *b)
{
  fspec *myspec;
  char *str;

  myspec=get_fspec();
  str=fspec_write_string(myspec,(unsigned long) b);
  return str;
}
