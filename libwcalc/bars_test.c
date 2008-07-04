/* $Id: bars_test.c,v 1.1 2008/07/02 15:03:52 dan Exp $ */

/*
 * Copyright (c) 2008 Dan McMahill
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
  double gap;

  b = bars_new();

  b->a = 10.0e-6;
  b->b = 1.0e-6;
  b->l1 = 1000.0e-6;

  b->d = 20.0e-6;
  b->c = 1.0e-6;
  b->l2 = 1000.0e-6;

  b->E  = b->a + 2.0e-6;
  b->P  = 0.0;
  b->l3 = 0.0;

  b->freq = 1.0e9;

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
  gap = 1.0e-6;
  while (gap <= 10.0e-6) {
    b->E = b->a + gap;

    bars_calc(b, b->freq);
    printf("%g: %g pH, %g pH, %g pH, %g\n", gap, 
	   b->L1/1e-12, b->L2/1e-12, b->M/1e-12, b->k);

    gap += 1.0e-6;
  }

  str = bars_save_string(b);
  printf("Example of bars_save_string() output:\n\"%s\"\n\n", str);
  free(str);

  bars_free(b);

  return 0;
}
