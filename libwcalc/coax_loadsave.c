/* $Id: coax_loadsave.c,v 1.16 2004/08/31 21:38:18 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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

#include "coax.h"
#include "coax_loadsave.h"
#include "alert.h"
#include "wcalc_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FILE_VERSION "0.1"

static fspec * get_fspec(void)
{
  static fspec *myspec=NULL;
  coax_line *line=0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"coax");
    fspec_add_key(myspec, "file_version","Coax file version",
		  'f', FILE_VERSION);
    fspec_add_comment(myspec, "Physical parameters");
    fspec_add_key(myspec, "a","Radius of inner conductor (meters)",
		  'd', &line->a);
    fspec_add_key(myspec, "b","Inner radius of outer conductor (meters)",
		  'd', &line->b);
    fspec_add_key(myspec, "c","Offset of inner conductor from center (meters)",
		  'd', &line->c);
    fspec_add_key(myspec, "tshield","Thickness of outer conductor (meters)",
		  'd', &line->tshield);
    fspec_add_key(myspec, "rho_a","Resistivity of inner conductor (ohms/meter)",
		  'd', &line->rho_a);
    fspec_add_key(myspec, "rho_b","Resistivity of outer conductor (ohms/meter)",
		  'd', &line->rho_b);
    fspec_add_key(myspec, "er","Dielectric relative permitivity",
		  'd', &line->er);
    fspec_add_key(myspec, "tand","Dielectric loss tangent",
		  'd', &line->tand);
    fspec_add_key(myspec, "len","Physical length of line (meters)",
		  'd', &line->len);

    fspec_add_comment(myspec, "Electrical parameters");
    fspec_add_key(myspec, "z0","Characteristic impedance of line (Ohms)",
		  'd', &line->z0);
    fspec_add_key(myspec, "elen","Electrical length of line (degrees)",
		  'd', &line->elen);
    fspec_add_key(myspec, "freq","Frequency of operation (Hz)",
		  'd', &line->freq);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec, "User units");


    fspec_add_key(myspec, "wc_units_emax", "Max. electric field units",
		  'u', &line->units_emax);
    fspec_add_key(myspec, "wc_units_fc", "TE10 cutoff frequency units",
		  'u', &line->units_fc);
    fspec_add_key(myspec, "wc_units_delay", "Delay units",
		  'u', &line->units_delay);
    fspec_add_key(myspec, "wc_units_loss", "Loss units",
		  'u', &line->units_loss);
    fspec_add_key(myspec, "wc_units_losslen", "Loss/length units",
		  'u', &line->units_losslen);

    fspec_add_key(myspec, "wc_units_abct", "A,B,C, Tshield units",
		  'u', &line->units_abct);
    fspec_add_key(myspec, "wc_units_len", "Line physical length units",
		  'u', &line->units_len);
    fspec_add_key(myspec, "wc_units_freq", "Frequency units",
		  'u', &line->units_freq);
    fspec_add_key(myspec, "wc_units_rho", "Resistivity units",
		  'u', &line->units_rho);
    fspec_add_key(myspec, "wc_units_L", "Incremental inductance units",
		  'u', &line->units_L);
    fspec_add_key(myspec, "wc_units_R", "Incremental resistance units",
		  'u', &line->units_R);
    fspec_add_key(myspec, "wc_units_C", "Incremental capacitance units",
		  'u', &line->units_C);
    fspec_add_key(myspec, "wc_units_G", "Incremental conductance units",
		  'u', &line->units_G);
  }

  return myspec;
}

int coax_load(coax_line *line, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val=file_read_val(fp,"[coax]","file_version")) == NULL ){
    alert("Could not determine the coax file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("coax_loadsave.c:coax_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp(val, FILE_VERSION) != 0) {
    alert("Unable to load a wcalc coax file with coax file version\n"
	  "\"%s\".  I only understand version \"%s\"\n", 
	  val, FILE_VERSION);
    return -1;
  }

  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style file formats.
   */

  myspec=get_fspec();
  rslt=fspec_read_file(myspec,fp,(unsigned long) line);

  return rslt;
}


void coax_save(coax_line *line, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_COAX);
  myspec=get_fspec();
  fspec_write_file(myspec,fp,(unsigned long) line);
}

int coax_load_string(coax_line *line, const char *str)
{
  fspec *myspec;
  /* char *val; */
  int rslt;

  assert(str!=NULL);

#ifdef DEBUG
  printf("coax_loadsave.c:coax_load_string():  loading \"%s\"\n",str);
#endif
#ifdef notdef
  /* XXX fixme*/
  val = strtok(str," ");

  /* read the model version  */
  if ( val == NULL ){
    alert("Could not determine the coax file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("coax_loadsave.c:coax_load_string():  Got file_version=\"%s\"\n",
	 val);
#endif
#endif
  /*
   * If the file format changes, this is where we would call legacy
   * routines to read old style formats.
   */

  myspec=get_fspec();
#ifdef DEBUG
  printf("coax_loadsave.c:coax_load_string():  loading \"%s\"\n",str);
#endif
  rslt=fspec_read_string(myspec,str,(unsigned long) line);
  if (rslt != 0) {
	return rslt;
  }

  return rslt;
}


char * coax_save_string(coax_line *line)
{
  fspec *myspec;
  char *str;

  myspec=get_fspec();
  str=fspec_write_string(myspec,(unsigned long) line);
  return str;
}

