/*
 * Copyright (C) 2020 Dan McMahill
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

#include "rods.h"
#include "rods_loadsave.h"
#include "alert.h"
#include "wcalc_loadsave.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FILE_VERSION "1.0"

static fspec * get_fspec(void)
{
  static fspec *myspec=NULL;
  rods *b = 0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"rods");
    fspec_add_key(myspec, "file_version","Rods file version",
		  'f', FILE_VERSION);

    fspec_add_key(myspec, "d1","Rod #1 diameter (meters)",
		  'd', &b->d1);
    fspec_add_key(myspec, "l1","Rod #1 length (z-dimension, meters)",
		  'd', &b->l1);

    fspec_add_key(myspec, "d2","Rod #2 diameter (meters)",
		  'd', &b->d2);
    fspec_add_key(myspec, "l2","Rod #2 length (z-dimension, meters)",
		  'd', &b->l2);

    fspec_add_key(myspec, "distance","Rod #2 radial distance (meters)",
		  'd', &b->distance);
    fspec_add_key(myspec, "offset","Rod #2 length offset (meters)",
		  'd', &b->offset);

    fspec_add_key(myspec, "L1","Rod #1 self inductance (Henries)",
		  'd', &b->L1);
    fspec_add_key(myspec, "L2","Rod #2 self inductance (Henries)",
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
    fspec_add_key(myspec, "units_xy", "Diameter and Length units",  
		  'u', &b->units_xy);
    fspec_add_key(myspec, "units_L", "Inductance units",  
		  'u', &b->units_L);
    fspec_add_key(myspec, "units_freq", "Frequency units",  
		  'u', &b->units_freq);

  }

  return myspec;
}

int rods_load(rods *b, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val = file_read_val(fp, "[rods]", "file_version")) == NULL ){
    alert("Could not determine the rods file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("rods_loadsave.c:rods_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp(val, FILE_VERSION) != 0) {
    alert("Unable to load a wcalc rods file with rods file version\n"
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


void rods_save(rods *b, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_BARS);
  myspec=get_fspec();
  fspec_write_file(myspec,fp,(unsigned long) b);
}

int rods_load_string(rods *b, const char *str)
{
  fspec *myspec;
  char *val;
  char *mystr;
  int rslt;

  assert(str!=NULL);

#ifdef DEBUG
  printf("rods_loadsave.c:rods_load_string():  loading \"%s\"\n",str);
#endif

  mystr = strdup(str);

  /* XXX fixme*/
  val = strtok(mystr," ");

  free(mystr);

  /* read the model version  */
  if ( val == NULL ){
    alert("Could not determine the rods file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("rods_loadsave.c:rods_load_string():  "
	 "Got file_version=\"%s\"\n",
	 val);
#endif
  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style formats.
   */

  myspec = get_fspec();
#ifdef DEBUG
  printf("rods_loadsave.c:rods_load_string():  loading \"%s\"\n",str);
#endif
  rslt = fspec_read_string(myspec, str, (unsigned long) b);
  if (rslt != 0) {
	return rslt;
  }

  return rslt;
}


char * rods_save_string(rods *b)
{
  fspec *myspec;
  char *str;

  myspec=get_fspec();
  str=fspec_write_string(myspec,(unsigned long) b);
  return str;
}
