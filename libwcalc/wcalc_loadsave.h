/* $Id: wcalc_loadsave.h,v 1.15 2009/02/10 05:00:21 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004, 2006, 2008, 2009 Dan McMahill
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
#define FILE_BARS                "bars"
#define FILE_COAX                "coax"
#define FILE_COPLANAR            "coplanar"
#define FILE_COUPLED_MICROSTRIP  "coupled_microstrip"
#define FILE_COUPLED_STRIPLINE   "coupled_stripline"
#define FILE_IC_MICROSTRIP       "ic_microstrip"
#define FILE_MICROSTRIP          "microstrip"
#define FILE_PARALLEL_RC         "parallel_rc"
#define FILE_PARALLEL_RL         "parallel_rl"
#define FILE_STRIPLINE           "stripline"


enum {
  MODEL_AIR_COIL,
  MODEL_BARS,
  MODEL_COAX,
  MODEL_COPLANAR,
  MODEL_COUPLED_MICROSTRIP,
  MODEL_COUPLED_STRIPLINE,
  MODEL_IC_MICROSTRIP,
  MODEL_MICROSTRIP,
  MODEL_PARALLEL_RC,
  MODEL_PARALLEL_RL,
  MODEL_STRIPLINE
};

#define MAXLINELEN 256
#define FIELDSEP " \t=\n"

#endif /*__WCALC_LOADSAVE_H__*/
