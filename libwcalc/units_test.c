/* $Id: units_test.c,v 1.1 2004/07/18 16:09:39 dan Exp $ */

/*
 * Copyright (c) 2004 Dan McMahill
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{
  wc_units *my_units;
  char *str;
  char *savestr = "2-3";
  int rc;
  double sf;

  my_units = wc_units_new(WC_UNITS_RESISTIVITY);

  str = wc_units_to_str( my_units );
  printf("wc_units_to_str() returned \"%s\"\n", str);
  free( str );

  sf = wc_units_to_sf( my_units );
  printf("wc_units_to_sf() returned %g\n", sf);

  str = wc_units_to_savestr( my_units );
  printf("wc_units_to_savestr() returned \"%s\"\n", str);
  free( str );

  rc = wc_savestr_to_units( savestr, my_units);
  printf("wc_savestr_to_units with \"%s\" returned %d\n", savestr, rc);

  str = wc_units_to_str( my_units );
  printf("wc_units_to_str() returned \"%s\"\n", str);
  free( str );

  sf = wc_units_to_sf( my_units );
  printf("wc_units_to_sf() returned %g\n", sf);

  str = wc_units_to_savestr( my_units );
  printf("wc_units_to_savestr() returned \"%s\"\n", str);
  free( str );
  
  wc_units_free( my_units );

  return 0;
}
