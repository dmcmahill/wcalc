/*      $Id: stripline.h,v 1.9 2004/09/08 22:40:23 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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
 
#ifndef __STRIPLINE_H_
#define __STRIPLINE_H_


#include "units.h"

typedef struct STRIPLINE_SUBS
{
  double h, er, tmet, rho, rough, tand;

} stripline_subs;

typedef struct STRIPLINE_LINE
{

  /* length and width */
  double l;
  double w;

  /* characteristic impedance (ohms) */
  double z0;

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

  stripline_subs *subs;

} stripline_line;


int stripline_calc(stripline_line *line, double f);
int stripline_syn(stripline_line *line, double f, int flag);

void stripline_line_free(stripline_line * line);
stripline_line *stripline_line_new(void);
stripline_subs *stripline_subs_new(void);

/*
 * Flags for synthesis
 */

#define SLISYN_W    0    /* Synthesize the metal width         */
#define SLISYN_H    1    /* Synthesize the substrate thickness */
#define SLISYN_ER   2    /* Synthesize the dielectric constant */
#define SLISYN_L    3    /* Synthesize the length */



#endif /*__STRIPLINE_H_*/
