
/*
 * Copyright (C) 2004 Dan McMahill
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
