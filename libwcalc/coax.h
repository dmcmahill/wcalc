/* $Id: coax.h,v 1.13 2008/11/29 20:42:05 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2003, 2004 Dan McMahill
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

#ifndef __COAX_H__
#define __COAX_H__

#include "misc.h"
#include "units.h"

typedef struct COAX_LINE
{
  /* diameter of inner conductor (meters) */
  double a;

  /* inside diameter of outer conductor (meters) */
  double b;

  /* 
   * how much the inner conductor is offset from center (meters)
   * this parameter is usually zero.  It may be useful to examine the
   * sensitivity of impedance to manufacturing tolerance
   */
  double c;

  /* outer conductor thickness (meters) */
  double tshield;

  /* physical length of line */
  double len;

  /* resistivity of inner and outer conductors (ohm-meter)*/
  double rho_a, rho_b;

  /* relative dielectric constant */
  double er;

  /* dielectric loss tangent */
  double tand;

  /* dielectric breakdown field strength */
  double emax;

  /* incremental circuit model, ({Henries,Ohms,Farads,Siemes}/Meter) */
  double L, R, C, G;

  /* characteriztic impedance (ohms) */
  double z0;

  /* electrical length of line */
  double elen;

  /* conductor and dielectric losses */
  double alpha_c, alpha_d;

  /* cutoff frequency for TE10 mode (Hz) */
  double fc;

  /* frequency of analysis (Hz) */
  double freq;

  double delay;
  double loss;
  double losslen;

  wc_units *units_emax;
  wc_units *units_fc;
  wc_units *units_delay;
  wc_units *units_loss, *units_losslen;

  wc_units *units_abct;
  wc_units *units_len;
  wc_units *units_freq;
  wc_units *units_rho;
  wc_units *units_L, *units_R, *units_C, *units_G;

} coax_line;


int coax_calc(coax_line *line, double f);
int coax_syn(coax_line *line, double f, int flag);

void coax_free(coax_line * line);
coax_line *coax_new(void);


/*
 * Flags for synthesis
 */

#define COAXSYN_A    0    /* Synthesize A with B, C fixed */
#define COAXSYN_B    1    /* Synthesize B with A, C fixed */
#define COAXSYN_C    2    /* Synthesize C with A, B fixed */
#define COAXSYN_ER   3    /* Synthesize Er with A, B, C fixed */
#define COAXSYN_L    4    /* Synthesize L with A, B, C, Er fixed */


#endif /*__COAX_H__*/
