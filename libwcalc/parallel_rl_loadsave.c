/* $Id: parallel_rl_loadsave.c,v 1.2 2009/02/10 12:10:10 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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

#include "parallel_rl.h"
#include "parallel_rl_loadsave.h"
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
  parallel_rl *b = 0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"parallel_rl");
    fspec_add_key(myspec, "file_version","Parallel_Rl file version",
		  'f', FILE_VERSION);

    fspec_add_key(myspec, "Ls","Series inductance (Henries)",
		  'd', &b->Ls);
    fspec_add_key(myspec, "Rs","Series resistance (Ohms)",
		  'd', &b->Rs);
    fspec_add_key(myspec, "Qs","Series quality factor",
		  'd', &b->Qs);


    fspec_add_key(myspec, "Lp","Parallel inductance (Henries)",
		  'd', &b->Lp);
    fspec_add_key(myspec, "Rp","Parallel resistance (Ohms)",
		  'd', &b->Rp);
    fspec_add_key(myspec, "Qp","Parallel quality factor",
		  'd', &b->Qp);

    fspec_add_key(myspec, "series_to_parallel","Convert from series to parallel?",
		  'i', &b->series_to_parallel);

    fspec_add_key(myspec, "use_Q","Use Q to calculate resistance?",
		  'i', &b->use_Q);

    fspec_add_key(myspec, "freq","Frequency of operation (Hz)",
		  'd', &b->freq);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec, "Desired user units and associated scale factors");
    fspec_add_key(myspec, "units_L", "Inductance units",  
		  'u', &b->units_L);
    fspec_add_key(myspec, "units_Rs", "Series resistance units",  
		  'u', &b->units_Rs);
    fspec_add_key(myspec, "units_Rp", "Parallel resistance units",  
		  'u', &b->units_Rp);
    fspec_add_key(myspec, "units_freq", "Frequency units",  
		  'u', &b->units_freq);

  }

  return myspec;
}

int parallel_rl_load(parallel_rl *b, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val = file_read_val(fp, "[parallel_rl]", "file_version")) == NULL ){
    alert("Could not determine the parallel_rl file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("parallel_rl_loadsave.c:parallel_rl_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp(val, FILE_VERSION) != 0) {
    alert("Unable to load a wcalc parallel_rl file with parallel_rl file version\n"
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


void parallel_rl_save(parallel_rl *b, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_PARALLEL_RL);
  myspec=get_fspec();
  fspec_write_file(myspec,fp,(unsigned long) b);
}

int parallel_rl_load_string(parallel_rl *b, const char *str)
{
  fspec *myspec;
  char *val;
  char *mystr;
  int rslt;

  assert(str!=NULL);

#ifdef DEBUG
  printf("parallel_rl_loadsave.c:parallel_rl_load_string():  loading \"%s\"\n",str);
#endif

  mystr = strdup(str);

  /* XXX fixme*/
  val = strtok(mystr," ");

  free(mystr);

  /* read the model version  */
  if ( val == NULL ){
    alert("Could not determine the parallel_rl file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("parallel_rl_loadsave.c:parallel_rl_load_string():  "
	 "Got file_version=\"%s\"\n",
	 val);
#endif
  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style formats.
   */

  myspec = get_fspec();
#ifdef DEBUG
  printf("parallel_rl_loadsave.c:parallel_rl_load_string():  loading \"%s\"\n",str);
#endif
  rslt = fspec_read_string(myspec, str, (unsigned long) b);
  if (rslt != 0) {
	return rslt;
  }

  return rslt;
}


char * parallel_rl_save_string(parallel_rl *b)
{
  fspec *myspec;
  char *str;

  myspec=get_fspec();
  str=fspec_write_string(myspec,(unsigned long) b);
  return str;
}
