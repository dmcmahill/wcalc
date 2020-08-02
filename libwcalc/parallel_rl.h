
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

#ifndef __PARALLEL_RL_H__
#define __PARALLEL_RL_H__

#include "units.h"

typedef struct PARALLEL_RL
{

  /* parallel circuit */
  double Rp, Lp, Qp;

  /* series circuit */
  double Rs, Ls, Qs;

  /* both */
  double freq;

  /* enter Q instead of R */
  int use_Q;

  /* convert series to parallel (1) or parallel to series (0) */
  int series_to_parallel;

  /* user units */
  wc_units *units_Rs;
  wc_units *units_Rp;
  wc_units *units_L;
  wc_units *units_freq;

} parallel_rl;


int parallel_rl_calc(parallel_rl *, double);

void parallel_rl_free(parallel_rl *);
parallel_rl *parallel_rl_new(void);


#endif /*__PARALLEL_RL_H__*/
