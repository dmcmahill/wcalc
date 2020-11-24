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

#ifndef __RODS_H__
#define __RODS_H__

#include "units.h"

typedef struct RODS
{

  /* Bar 1 diameter and length */
  double d1, l1;

  /* Bar 2 diameter and length */
  double d2, l2;

  /* Bar 2 offsets, axis-to-axis distance and length offset */
  double distance, offset;

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


int rods_calc(bars *, double);
int rods_syn(bars *, double, int);

void rods_free(bars *);
bars *rods_new(void);


/*
 * Flags for synthesis
 */

#define RODSSYN_NMIN    0    /* Synthesize for ?? */
#define RODSSYN_NFIX    1    /* Synthesize for ?? */


#endif /*__RODS_H__*/
