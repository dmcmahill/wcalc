/* $Id: coax_loadsave.c,v 1.6 2001/11/25 16:33:28 dan Exp $ */

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

#define FILE_VERSION "0.1"

static fspec * get_fspec(void)
{
  static fspec *myspec=NULL;
  coax_line *line=0;

  if (myspec == NULL) {
    /* Build up the list which describes the file format */

    myspec=fspec_add_sect(NULL,"coax");
    fspec_add_key(myspec,"file_version","Coax file version",'f',FILE_VERSION);
    fspec_add_comment(myspec,"Physical parameters");
    fspec_add_key(myspec,"a","Radius of inner conductor (meters)",'d',&line->a);
    fspec_add_key(myspec,"b","Inner radius of outer conductor (meters)",'d',&line->b);
    fspec_add_key(myspec,"c","Offset of inner conductor from center (meters)",'d',&line->c);
    fspec_add_key(myspec,"tshield","Thickness of outer conductor (meters)",'d',&line->tshield);
    fspec_add_key(myspec,"rho_a","Resistivity of inner conductor (ohms/meter)",'d',&line->rho_a);
    fspec_add_key(myspec,"rho_b","Resistivity of outer conductor (ohms/meter)",'d',&line->rho_b);
    fspec_add_key(myspec,"er","Dielectric relative permitivity",'d',&line->er);
    fspec_add_key(myspec,"tand","Dielectric loss tangent",'d',&line->tand);
    fspec_add_key(myspec,"len","Physical length of line (meters)",'d',&line->len);

    fspec_add_comment(myspec,"Electrical parameters");
    fspec_add_key(myspec,"z0","Characteristic impedance of line (Ohms)",'d',&line->z0);
    fspec_add_key(myspec,"elen","Electrical length of line (degrees)",'d',&line->elen);
    fspec_add_key(myspec,"freq","Frequency of operation (Hz)",'d',&line->freq);

    /*
     * The desired user units
     */
    fspec_add_comment(myspec,"Desired user units and associated scale factors");

    fspec_add_key(myspec,"freq_sf","Frequency scale factor (Hz/unit)",'d',&line->freq_sf);
    fspec_add_key(myspec,"freq_units","Frequency units",'s',&line->freq_units);
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

