/* $Id: wcalc_loadsave.h,v 1.2 2001/11/03 02:16:21 dan Exp $ */

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

#ifndef __WCALC_LOADSAVE_H__
#define __WCALC_LOADSAVE_H__

#define WCALC_FILE_VERSION "0.1"

void wcalc_save_header(FILE *fp, char *fname, char *model_name);
int wcalc_load(FILE *fp);

char * file_read_val(FILE *fp, const char *section, const char *key);

/* Model types.  Used to identify models in the file */
#define FILE_AIR_COIL            "air_coil"
#define FILE_COUPLED_MICROSTRIP  "coupled_microstrip"
#define FILE_IC_MICROSTRIP       "ic_microstrip"
#define FILE_MICROSTRIP          "microstrip"
#define FILE_STRIPLINE           "stripline"


enum {
  MODEL_AIR_COIL,
  MODEL_COUPLED_MICROSTRIP,
  MODEL_IC_MICROSTRIP,
  MODEL_MICROSTRIP,
  MODEL_STRIPLINE
};

#define MAXLINELEN 80
#define FIELDSEP " \t=\n"

#endif /*__WCALC_LOADSAVE_H__*/
