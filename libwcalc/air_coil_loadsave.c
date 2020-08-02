
/*
 * Copyright (C) 2001, 2002, 2004, 2012 Dan McMahill
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

#include "air_coil.h"
#include "air_coil_loadsave.h"
#include "alert.h"
#include "wcalc_loadsave.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FILE_VERSION "1.0"

/* for version 0.1 files */
static fspec * get_fspec_v0p1(void)
{
  static fspec *myspec=NULL;
  air_coil_coil *coil=0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"air_coil");
    fspec_add_key(myspec, "file_version","Air coil file version",
		  'f', "0.1");
    fspec_add_key(myspec, "Nf","Number of turns",
		  'd', &coil->Nf);
    fspec_add_key(myspec, "len","Length of coil (meters)",
		  'd', &coil->len);
    fspec_add_key(myspec, "fill","Ratio of coil length to close wound length",
		  'd', &coil->fill);
    fspec_add_key(myspec, "AWGf","Wire size (AWG)",
		  'd', &coil->AWGf);
    fspec_add_key(myspec, "rho","Wire resistivity (ohms/meter)",
		  'd', &coil->rho);
    fspec_add_key(myspec, "dia","Inside diameter of coil (meters)",
		  'd', &coil->dia);
    fspec_add_key(myspec, "L","Desired Inductance (H)",
		  'd', &coil->L);
    fspec_add_key(myspec, "freq","Frequency of operation (Hz)",
		  'd', &coil->freq);
    fspec_add_key(myspec, "use_fill","Use fill to calculate length?",
		  'i', &coil->use_fill);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec, "Desired user units and associated scale factors");
    fspec_add_key(myspec, "units_len", "Length units",  
		  'u', &coil->units_len);
    fspec_add_key(myspec, "units_dia", "Diameter units",  
		  'u', &coil->units_dia);
    fspec_add_key(myspec, "units_L", "Inductance units",  
		  'u', &coil->units_L);
    fspec_add_key(myspec, "units_SRF", "Self resonant frequency units",  
		  'u', &coil->units_SRF);
    fspec_add_key(myspec, "units_rho", "Resistivity units",  
		  'u', &coil->units_rho);
    fspec_add_key(myspec, "units_freq", "Frequency units",  
		  'u', &coil->units_freq);

  }

  return myspec;
}

static fspec * get_fspec(void)
{
  static fspec *myspec=NULL;
  air_coil_coil *coil=0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"air_coil");
    fspec_add_key(myspec, "file_version","Air coil file version",
		  'f', FILE_VERSION);
    fspec_add_key(myspec, "Nf","Number of turns",
		  'd', &coil->Nf);
    fspec_add_key(myspec, "len","Length of coil (meters)",
		  'd', &coil->len);
    fspec_add_key(myspec, "fill","Ratio of coil length to close wound length",
		  'd', &coil->fill);
    fspec_add_key(myspec, "AWGf","Wire size (AWG)",
		  'd', &coil->AWGf);
    fspec_add_key(myspec, "wire_diameter","Wire diameter (meters)",
		  'd', &coil->wire_diameter);
    fspec_add_key(myspec, "rho","Wire resistivity (ohms/meter)",
		  'd', &coil->rho);
    fspec_add_key(myspec, "dia","Inside diameter of coil (meters)",
		  'd', &coil->dia);
    fspec_add_key(myspec, "L","Desired Inductance (H)",
		  'd', &coil->L);
    fspec_add_key(myspec, "freq","Frequency of operation (Hz)",
		  'd', &coil->freq);
    fspec_add_key(myspec, "use_fill","Use fill to calculate length?",
		  'i', &coil->use_fill);
    fspec_add_key(myspec, "use_wire_diameter","Use wire_diameter to calculate wire size in AWG?",
		  'i', &coil->use_wire_diameter);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec, "Desired user units and associated scale factors");
    fspec_add_key(myspec, "units_len", "Coil length units",  
		  'u', &coil->units_len);
    fspec_add_key(myspec, "units_dia", "Coil diameter units",  
		  'u', &coil->units_dia);
    fspec_add_key(myspec, "units_wire_diameter", "Wire diameter units",  
		  'u', &coil->units_wire_diameter);
    fspec_add_key(myspec, "units_L", "Inductance units",  
		  'u', &coil->units_L);
    fspec_add_key(myspec, "units_SRF", "Self resonant frequency units",  
		  'u', &coil->units_SRF);
    fspec_add_key(myspec, "units_rho", "Resistivity units",  
		  'u', &coil->units_rho);
    fspec_add_key(myspec, "units_freq", "Frequency units",  
		  'u', &coil->units_freq);

  }

  return myspec;
}

int air_coil_load(air_coil_coil *coil, FILE *fp)
{
  fspec *myspec;
  char *val;
  int rslt;

  assert(fp!=NULL);

  /* read the model version  */
  if ( (val = file_read_val(fp, "[air_coil]", "file_version")) == NULL ){
    alert("Could not determine the air_coil file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("air_coil_loadsave.c:air_coil_load():  Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp (val, "0.1") == 0) {
    myspec = get_fspec_v0p1();
  } else if (strcmp(val, FILE_VERSION) == 0) {
    myspec = get_fspec();
  } else {
    alert("Unable to load a wcalc air_coil file with air_coil file version\n"
	  "\"%s\".  I only understand versions \"0.1\" and \"%s\"\n", 
	  val, FILE_VERSION);
    return -1;
  }

  rslt = fspec_read_file(myspec, fp,(unsigned long) coil);

  return rslt;
}


void air_coil_save(air_coil_coil *coil, FILE *fp, char *fname)
{
  fspec *myspec;

  wcalc_save_header(fp, fname, FILE_AIR_COIL);
  myspec=get_fspec();
  fspec_write_file(myspec,fp,(unsigned long) coil);
}

int air_coil_load_string(air_coil_coil *coil, const char *str)
{
  fspec *myspec;
  char *val;
  char *mystr;
  int rslt;

  assert(str!=NULL);

#ifdef DEBUG
  printf("air_coil_loadsave.c:air_coil_load_string():  loading \"%s\"\n",str);
#endif

  mystr = strdup(str);

  /* XXX fixme*/
  val = strtok (mystr," ");
  val = strtok (NULL, " ");
  free(mystr);

  /* read the model version  */
  if ( val == NULL ){
    alert("Could not determine the air_coil file_version\n");
    return -1;
  }

#ifdef DEBUG
  printf("air_coil_loadsave.c:air_coil_load_string():  "
	 "Got file_version=\"%s\"\n",
	 val);
#endif

  if (strcmp (val, "0.1") == 0) {
    myspec = get_fspec_v0p1();
  } else if (strcmp(val, FILE_VERSION) == 0) {
    myspec = get_fspec();
  } else {
    alert("Unable to load a wcalc air_coil file with air_coil file version\n"
	  "\"%s\".  I only understand versions \"0.1\" and \"%s\"\n", 
	  val, FILE_VERSION);
    return -1;
  }

#ifdef DEBUG
  printf("air_coil_loadsave.c:air_coil_load_string():  loading \"%s\"\n",str);
#endif
  rslt = fspec_read_string(myspec, str, (unsigned long) coil);
  if (rslt != 0) {
	return rslt;
  }

  return rslt;
}


char * air_coil_save_string(air_coil_coil *coil)
{
  fspec *myspec;
  char *str;

  myspec=get_fspec();
  str=fspec_write_string(myspec,(unsigned long) coil);
  return str;
}
