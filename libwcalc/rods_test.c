/*
 * Copyright (C) 2020 Dan McMahill
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rods.h"
#include "rods_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{
  rods *b;
  char *str;
  double gap = 1.0e-3;

  b = rods_new();

  b->d1 = 1.0e-3;
  b->l1 = 10.0e-3;

  b->d2 = 2.0e-3;
  b->l2 = 10.0e-3;

  b->distance = 0.5*(b->d1 + b->d2) + gap;
  b->offset = 0.0;

  b->freq = 1.0e9;
 
  printf("A reasonable default:\n");
  rods_calc(b, b->freq);
  printf("%g mm: %g nH, %g nH, %g nH, %g\n", gap/1e-3,
	 b->L1/1e-9, b->L2/1e-9, b->M/1e-9, b->k);

  str = rods_save_string(b);
  printf("Example of rods_save_string() output:\n\"%s\"\n\n", str);
  free(str);


  printf("d1 = %g mm, d2 = %g mm\n", b->d1/1e-3, b->d2/1e-3);
  printf("l1 = %g mm, l2 = %g mm\n", b->l1/1e-3, b->l2/1e-3);
  gap = 0.5e-3;
  while (gap <= 10.0e-3) {
    b->distance = 0.5*(b->d1 + b->d2) + gap;

    rods_calc(b, b->freq);
    printf("%g: %g nH, %g nH, %g nH, %g\n", gap, 
	   b->L1/1e-9, b->L2/1e-9, b->M/1e-9, b->k);

    gap += 0.5e-6;
  }

  rods_free(b);

  return 0;
}
