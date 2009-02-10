/* $Id: make_defaults.c,v 1.5 2008/11/29 20:42:15 dan Exp $ */

/*
 * Copyright (C) 2004, 2006, 2008, 2009 Dan McMahill
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

/*
 * This program reads in the saved defaults files and spits out
 * a "savestring" version.  This can then be used to inialize 
 * any new elements.  This especially helps with the cgi frontend.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "air_coil.h"
#include "air_coil_loadsave.h"
#include "bars.h"
#include "bars_loadsave.h"
#include "coax.h"
#include "coax_loadsave.h"
#include "coplanar.h"
#include "coplanar_loadsave.h"
#include "coupled_microstrip.h"
#include "coupled_microstrip_loadsave.h"
#include "coupled_stripline.h"
#include "coupled_stripline_loadsave.h"
#include "ic_microstrip.h"
#include "ic_microstrip_loadsave.h"
#include "microstrip.h"
#include "microstrip_loadsave.h"
#include "parallel_rc.h"
#include "parallel_rc_loadsave.h"
#include "stripline.h"
#include "stripline_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define FIELDSEP " \t"
#define MAXLINELEN 256

int main(int argc, char **argv)
{
  char *str;
  FILE *fp;
  char *dir="../gtk-wcalc";
  char fname[FILENAME_MAX];

  air_coil_coil *coil;
  bars *b;
  coax_line *coax;
  coplanar_line *cpw_line;
  coupled_microstrip_line *cms_line;
  coupled_stripline_line *cst_line;
  ic_microstrip_line *ic_ms_line;
  microstrip_line *ms_line;
  parallel_rc *prc;
  stripline_line *st_line;

  printf("/* $Id: make_defaults.c,v 1.5 2008/11/29 20:42:15 dan Exp $ */\n\n");
  printf("/* GENERATED FILE.  DO NOT EDIT. */\n");
  printf("/* use \"make_defaults\" to generate */\n\n");


  sprintf(fname, "%s/%s", dir, "air_coil.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    coil = air_coil_new();
    air_coil_load(coil, fp);
    str = air_coil_save_string(coil);
    printf("const char *default_air_coil=\"%s\";\n", str);
    free(str);
    air_coil_free(coil);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "bars.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    b = bars_new();
    bars_load(b, fp);
    str = bars_save_string(b);
    printf("const char *default_bars=\"%s\";\n", str);
    free(str);
    bars_free(b);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "coax.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    coax = coax_new();
    coax_load(coax, fp);
    str = coax_save_string(coax);
    printf("const char *default_coax=\"%s\";\n", str);
    free(str);
    coax_free(coax);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "coplanar.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    cpw_line = coplanar_line_new();
    coplanar_load(cpw_line, fp);
    str = coplanar_save_string(cpw_line);
    printf("const char *default_coplanar=\"%s\";\n", str);
    free(str);
    coplanar_line_free(cpw_line);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "coupled_microstrip.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    cms_line = coupled_microstrip_line_new();
    coupled_microstrip_load(cms_line, fp);
    str = coupled_microstrip_save_string(cms_line);
    printf("const char *default_coupled_microstrip=\"%s\";\n", str);
    free(str);
    coupled_microstrip_line_free(cms_line);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "coupled_stripline.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    cst_line = coupled_stripline_line_new();
    coupled_stripline_load(cst_line, fp);
    str = coupled_stripline_save_string(cst_line);
    printf("const char *default_coupled_stripline=\"%s\";\n", str);
    free(str);
    coupled_stripline_line_free(cst_line);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "ic_microstrip.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    ic_ms_line = ic_microstrip_line_new();
    ic_microstrip_load(ic_ms_line, fp);
    str = ic_microstrip_save_string(ic_ms_line);
    printf("const char *default_ic_microstrip=\"%s\";\n", str);
    free(str);
    ic_microstrip_line_free(ic_ms_line);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "microstrip.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    ms_line = microstrip_line_new();
    microstrip_load(ms_line, fp);
    str = microstrip_save_string(ms_line);
    printf("const char *default_microstrip=\"%s\";\n", str);
    free(str);
    microstrip_line_free(ms_line);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "parallel_rc.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    prc = parallel_rc_new();
    parallel_rc_load(prc, fp);
    str = parallel_rc_save_string(prc);
    printf("const char *default_parallel_rc=\"%s\";\n", str);
    free(str);
    parallel_rc_free(prc);
    fclose(fp);
  }

  sprintf(fname, "%s/%s", dir, "stripline.wc");
  if( (fp = fopen(fname, "r")) != NULL ) {
    st_line = stripline_line_new();
    stripline_load(st_line, fp);
    str = stripline_save_string(st_line);
    printf("const char *default_stripline=\"%s\";\n", str);
    free(str);
    stripline_line_free(st_line);
    fclose(fp);
  }

  return 0;
}
