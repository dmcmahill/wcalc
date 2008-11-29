/* $Id: bars_test.c,v 1.4 2008/11/29 18:58:05 dan Exp $ */

/*
 * Copyright (C) 2008 Dan McMahill
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

#include "bars.h"
#include "bars_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{
  bars *b;
  char *str;
  double gap = 1.0e-6;

  b = bars_new();

  b->a = 10.0e-6;
  b->b = 1.0e-6;
  b->l1 = 1000.0e-6;

  b->d = 20.0e-6;
  b->c = 1.0e-6;
  b->l2 = 1000.0e-6;

  b->E  = b->a + gap;
  b->P  = 0.0;
  b->l3 = 0.0;

  b->freq = 1.0e9;
 
  printf("A reasonable default:\n");
  bars_calc(b, b->freq);
  printf("%g: %g pH, %g pH, %g pH, %g\n", gap, 
	 b->L1/1e-12, b->L2/1e-12, b->M/1e-12, b->k);

  str = bars_save_string(b);
  printf("Example of bars_save_string() output:\n\"%s\"\n\n", str);
  free(str);


  printf("a = %g um, b = %g um, d = %g um, c = %g um\n", b->a/1e-6, b->b/1e-6, b->d/1e-6, b->c/1e-6);
  printf("l1 = %g um, l2 = %g um, l3 = %g um\n", b->l1/1e-6, b->l2/1e-6, b->l3/1e-6);
  gap = 1.0e-6;
  while (gap <= 10.0e-6) {
    b->E = b->a + gap;

    bars_calc(b, b->freq);
    printf("%g: %g pH, %g pH, %g pH, %g\n", gap, 
	   b->L1/1e-12, b->L2/1e-12, b->M/1e-12, b->k);

    gap += 1.0e-6;
  }

  printf("STRUCTURE #2\n");
  b->d = 10.0e-6;
  printf("a = %g um, b = %g um, d = %g um, c = %g um\n", b->a/1e-6, b->b/1e-6, b->d/1e-6, b->c/1e-6);
  printf("l1 = %g um, l2 = %g um, l3 = %g um\n", b->l1/1e-6, b->l2/1e-6, b->l3/1e-6);
  gap = 1.0e-6;
  while (gap <= 10.0e-6) {
    b->E = b->a + gap;

    bars_calc(b, b->freq);
    printf("%g: %g pH, %g pH, %g pH, %g\n", gap, 
	   b->L1/1e-12, b->L2/1e-12, b->M/1e-12, b->k);

    gap += 1.0e-6;
  }

  bars_free(b);

  return 0;
}
