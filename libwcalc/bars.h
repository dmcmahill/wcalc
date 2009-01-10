/* $Id: bars.h,v 1.3 2008/11/29 20:42:03 dan Exp $ */

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

  /* operating frequency */
  double freq;

  /* user units */
  wc_units *units_xy;
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
