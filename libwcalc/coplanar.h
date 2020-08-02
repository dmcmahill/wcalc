
/*
 * Copyright (C) 2006 Dan McMahill
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
 
#ifndef __COPLANAR_H__
#define __COPLANAR_H__


#include "units.h"

typedef struct COPLANAR_SUBS
{
  double h, er, tmet, rho, rough, tand;

} coplanar_subs;

typedef struct COPLANAR_LINE
{

  /* length and width */
  double l;
  double w;

  /* spacing */
  double s;

  /* characteristic impedance (ohms) */
  double z0;

  /* effective relative dielectric constant */
  double keff;

  /* electrical length (degrees) */
  double len;

  /* delay (seconds) */
  double delay;

  /* open end length correction */
  double deltal;

  double loss, losslen, skindepth;
  double lc, ld;

  /* incremental circuit model */
  double Ls, Rs, Cs, Gs;

  /* the actual characteristic impedance is Ro + j Xo */
  double Ro, Xo;

  /* frequency of analysis (Hertz) */
  double freq;

  /* with or without the bottom side ground plane */
  int with_ground;

  wc_units *units_lwht;
  wc_units *units_L, *units_R, *units_C, *units_G;
  wc_units *units_len;
  wc_units *units_freq;
  wc_units *units_loss;
  wc_units *units_losslen;
  wc_units *units_rho;
  wc_units *units_rough;
  wc_units *units_delay;
  wc_units *units_depth;
  wc_units *units_deltal;

  coplanar_subs *subs;

} coplanar_line;


int coplanar_calc(coplanar_line *line, double f);
int coplanar_syn(coplanar_line *line, double f, int flag);

void coplanar_line_free(coplanar_line * line);
coplanar_line *coplanar_line_new(void);
coplanar_subs *coplanar_subs_new(void);

/*
 * Flags for synthesis
 */

#define CPWSYN_W    0    /* Synthesize the metal width         */
#define CPWSYN_S    1    /* Synthesize the spacing to the top side ground */
#define CPWSYN_H    2    /* Synthesize the substrate thickness */
#define CPWSYN_ER   3    /* Synthesize the dielectric constant */



#endif /*__COPLANAR_H_*/
