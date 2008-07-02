/* $Id: bars.h,v 1.5 2004/07/26 01:09:16 dan Exp $ */

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

#ifndef __BARS_H__
#define __BARS_H__

#include "units.h"

typedef struct BARS
{

  /* Bar 1 x-y-z dimensions */
  double a, b, l1;

  /* Bar 2 x-y-z dimensions */
  double c, d, l2;

  /* Bar 2 x-y-z offsets */
  double E, P, l3;

  /* Self inductances */
  double L1, L2;

  /* Mutual inductance */
  double M;

  /* coupling coefficient */
  double k;

  /* user units */
  wc_units *units_xy, *units_z;
  wc_units *units_L;
  wc_units *units_freq;

} bars;


int bars_calc(bars *, double);
int bars_syn(bars *, double, int);

void bars_free(bars *);
bars *bars_new(void);


/*
 * Flags for synthesis
 */

#define BARSSYN_NMIN    0    /* Synthesize for ?? */
#define BARSSYN_NFIX    1    /* Synthesize for ?? */


#endif /*__BARS_H__*/
