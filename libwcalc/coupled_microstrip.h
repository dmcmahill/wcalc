/* $Id: coupled_microstrip.h,v 1.5 2003/01/19 05:07:40 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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

#ifndef __COUPLED_MICROSTRIP_H_
#define __COUPLED_MICROSTRIP_H_

#include "microstrip.h"
#include "units.h"


typedef struct COUPLED_MICROSTRIP_LINE
{

  /* length, width, and spacing */
  double l;
  double w;
  double s;

  /* characteristic impedance (ohms) and coupling factor */
  double z0, k;

  /* even and odd mode impedance */
  double z0e;
  double z0o;

  /* electrical length (degrees) */
  double len;


  /* open circuit end line correction */
  double deltale, deltalo;

  /* even and odd mode effective relative permitivitty */
  double kev, kodd;

  double loss_ev, loss_odd, losslen_ev, losslen_odd, skindepth;

  /* analysis frequency */
  double freq;

  microstrip_subs *subs;

  /* user units */
  wc_units *units_lwst;
  wc_units *units_len;

  wc_units *units_freq;
  wc_units *units_loss;
  wc_units *units_losslen;
  wc_units *units_rho;
  wc_units *units_rough;
  wc_units *units_delay;
  wc_units *units_depth;
  wc_units *units_deltal;

} coupled_microstrip_line;


double coupled_microstrip_calc(coupled_microstrip_line *line, double f);
int coupled_microstrip_syn(coupled_microstrip_line *line, double f, int flag);

void coupled_microstrip_line_free(coupled_microstrip_line * line);
coupled_microstrip_line *coupled_microstrip_line_new(void);

/*
 * Flags for synthesis
 */

/* these aren't used yet */
#define CMLISYN_W    0    /* Synthesize the metal width         */
#define CMLISYN_H    1    /* Synthesize the substrate thickness */
#define CMLISYN_ES   2    /* Synthesize the dielectric constant */
#define CMLISYN_L    3    /* Synthesize the length */


#endif /*__COUPLED_MICROSTRIP_H_*/
