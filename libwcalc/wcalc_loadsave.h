/* $Id: wcalc_loadsave.h,v 1.11 2006/01/09 21:09:25 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004, 2006 Dan McMahill
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

#ifndef __WCALC_LOADSAVE_H__
#define __WCALC_LOADSAVE_H__

#define WCALC_FILE_VERSION "0.1"

/*
 * typedef's
 */

typedef struct FSPEC {
  /* 
   * SECTION:  section (like "[air_coil]")
   * KEY:      key (like "width")
   * FIXED:    fixed key (like "file_version")
   * COMMENT:  fixed comment
   */
  enum {SPEC_SECTION, SPEC_KEY, SPEC_FIXED, SPEC_COMMENT} spec_type;

  /* the section name ("air_coil") or keyname ("width") */
  char *key;

  /*
   * a comment like "Width of the line (meters)" 
   * This will be saved in the output file.
   */
  char *comment;

  /* 
   * type of the variable associated with the key.
   *  'd' = double
   *  'i' = int
   *  's' = char *   (i.e. a string)
   *  'u' = char *, but it is a units string (i.e. wc_units_to_savestr()
   *        output)
   */
  char type;

  /* 
   * offset into the struct where this value lives.  For example:
   * &air_coil->fill - air_coil;
   */
  unsigned long ofs;

  /* this is a doubly linked list */
  struct FSPEC *next, *prev;
} fspec;


/*
 * prototypes
 */

/*
 * Write a wcalc header to the opened file pointer, fp.
 * The file name is written out to the file in the comment
 * section.
 */
void wcalc_save_header(FILE *fp, char *fname, char *model_name);

/*
 * look in the opened file pointer to determine the model type
 * contained in the file.
 * Returns one of the MODEL_* enumeration values below
 */
int wcalc_load(FILE *fp);

char * file_read_val(FILE *fp, const char *section, const char *key);

fspec * fspec_add_key(fspec *list,
		      char *name,
		      char *comment,
		      char type,
		      void *ofs
		      );
fspec * fspec_add_sect(fspec *list,char *name);
fspec * fspec_add_comment(fspec *list,char *comment);
int fspec_write_file(fspec *list,FILE *fp,unsigned long base);
int fspec_read_file(fspec *list,FILE *fp,unsigned long base);
char *fspec_write_string(fspec *list,unsigned long base);
int fspec_read_string(fspec *list, const char *str,unsigned long base);


/* Model types.  Used to identify models in the file */
#define FILE_AIR_COIL            "air_coil"
#define FILE_COAX                "coax"
#define FILE_COPLANAR            "coplanar"
#define FILE_COUPLED_MICROSTRIP  "coupled_microstrip"
#define FILE_COUPLED_STRIPLINE   "coupled_stripline"
#define FILE_IC_MICROSTRIP       "ic_microstrip"
#define FILE_MICROSTRIP          "microstrip"
#define FILE_STRIPLINE           "stripline"


enum {
  MODEL_AIR_COIL,
  MODEL_COAX,
  MODEL_COPLANAR,
  MODEL_COUPLED_MICROSTRIP,
  MODEL_COUPLED_STRIPLINE,
  MODEL_IC_MICROSTRIP,
  MODEL_MICROSTRIP,
  MODEL_STRIPLINE
};

#define MAXLINELEN 256
#define FIELDSEP " \t=\n"

#endif /*__WCALC_LOADSAVE_H__*/
