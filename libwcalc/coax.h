/* $Id: coax.h,v 1.4 2002/01/03 03:54:53 dan Exp $ */

/*
 * Copyright (c) 2001 Dan McMahill
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
 *        This product includes software developed Dan McMahill
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

#ifndef __COAX_H__
#define __COAX_H__

#include "misc.h"

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
  double L,R,C,G;

  /* characteriztic impedance (ohms) */
  double z0;

  /* electrical length of line */
  double elen;

  /* conductor and dielectric losses */
  double alpha_c, alpha_d;

  /* cutoff frequency for TE11 mode (Hz) */
  double fc;

  /* frequency of analysis (Hz) */
  double freq;

  double delay;
  double loss;
  double losslen;

  /* 
   * various units and scale factors which may be useful for several
   * backends.  The scale factors multiply the internal values to get
   * the external units.
   *
   * For example, we might have:
   *   L_units="nH" and L_sf = 1e-9;
   */
  double a_sf,b_sf,c_sf,tshield_sf,len_sf,rho_a_sf,rho_b_sf;
  double emax_sf;
  double L_sf,R_sf,C_sf,G_sf;
  double fc_sf,freq_sf;
  double delay_sf, loss_sf, losslen_sf;
  
  char *a_units,*b_units,*c_units,*tshield_units,*len_units,*rho_a_units,*rho_b_units;
  char *emax_units;
  char *L_units,*R_units,*C_units,*G_units;
  char *fc_units,*freq_units;
  char *delay_units, *loss_units, *losslen_units;

  composite_units_data *units_rhoa, *units_rhob;
  
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
