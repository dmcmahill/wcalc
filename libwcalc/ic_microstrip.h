/* $Id: ic_microstrip.h,v 1.8 2004/07/29 00:02:21 dan Exp $ */

/*
 * Copyright (C)  2001, 2002, 2004 Dan McMahill
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

#ifndef __IC_MICROSTRIP_H__
#define __IC_MICROSTRIP_H__

#include "units.h"

typedef struct IC_MICROSTRIP_SUBS
{
  /* top metal parameters */
  double tmet, rho, rough;

  /* dielectric (oxide) paramters */
  double eox, tox;

  /* substrate (silicon typically) parameters */
  double h, es, sigmas;

} ic_microstrip_subs;

typedef struct IC_MICROSTRIP_LINE
{

  /* length and width */
  double l;
  double w;

  /* characteristic impedance Ro + j Xo (ohms) */
  double Ro, Xo;

  /* electrical length (degrees) */
  double len;

  double delay;

  /* effective dielectric constant */
  double keff;

  /* loss per given length, loss per unit length, and loss per wavelegth */
  double loss, losslen, losslambda;

  /* skin depth in the metal and in the lossy dielectric */
  double met_skindepth, subs_skindepth;

  /* incremental circuit model */
  double Lmis, Rmis, Cmis, Gmis;

  ic_microstrip_subs *subs;

  /* frequency of operation */
  double freq,freq_sf;
  char *freq_units;

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
  wc_units *units_sigmas;

} ic_microstrip_line;


/*
 * Function Prototypes
 */

int ic_microstrip_calc(ic_microstrip_line *line, double f);
int ic_microstrip_syn(ic_microstrip_line *line, double f, int flag);

ic_microstrip_line *ic_microstrip_line_new(void);
void ic_microstrip_line_free(ic_microstrip_line *);
ic_microstrip_subs *ic_microstrip_subs_new(void);
void ic_microstrip_subs_free(ic_microstrip_subs *);

/*
 * Flags for synthesis
 */

#define IC_MLISYN_W    0    /* Synthesize the metal width         */
#define IC_MLISYN_H    1    /* Synthesize the substrate thickness */
#define IC_MLISYN_TOX  2    /* Synthesize the oxide thickness     */


#endif /*__IC_MICROSTRIP_H_*/
