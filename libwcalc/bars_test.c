/* $Id: coax_test.c,v 1.7 2005/01/06 22:52:35 dan Exp $ */

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

  b->a = 10.0;
  b->b = 1.0;
  b->l1 = 1000.0;

  b->c = 20.0;
  b->d = 1.0;
  b->l2 = 1000.0;

  b->E  = 0.5*(b->a + b->c) + 2.0;
  b->P  = 1.0;
  b->l3 = 0.0;

  b->freq = 1.0e9;

  gap = 1.0;
  while (gap <= 10.0 ; gap += 1.0) {
    b->E = 0.5*(b->a + b->c) + gap;

    bars_calc(b, b->freq);
    printf("%g: %g pH, %g pH, %g pH, %g\n", gap, b->L1, b->L2, b->M, b->k);
  }

  str = bars_save_string(line);
  printf("Example of bars_save_string() output:\n\"%s\"\n\n", str);
  free(str);

  bars_free(line);

  return 0;
}
