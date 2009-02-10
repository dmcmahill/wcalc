/* $Id: parallel_rc.h,v 1.1 2009/02/10 05:00:20 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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

#ifndef __PARALLEL_RC_H__
#define __PARALLEL_RC_H__

#include "units.h"

typedef struct PARALLEL_RC
{

  /* parallel circuit */
  double Rp, Cp, Qp;

  /* series circuit */
  double Rs, Cs, Qs;

  /* both */
  double freq;

  /* enter Q instead of R */
  int use_Q;

  /* convert series to parallel (1) or parallel to series (0) */
  int series_to_parallel;

  /* user units */
  wc_units *units_Rs;
  wc_units *units_Rp;
  wc_units *units_C;
  wc_units *units_freq;

} parallel_rc;


int parallel_rc_calc(parallel_rc *, double);

void parallel_rc_free(parallel_rc *);
parallel_rc *parallel_rc_new(void);


#endif /*__PARALLEL_RC_H__*/
