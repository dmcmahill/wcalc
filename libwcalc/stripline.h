/*      $Id: stripline.h,v 1.3 2001/11/11 03:46:55 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002 Dan McMahill
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
 
#ifndef __STRIPLINE_H_
#define __STRIPLINE_H_


typedef struct STRIPLINE_SUBS
{
  double h,er,tmet,rho,rough,tand;
  double h_sf,tmet_sf,rho_sf,rough_sf;

  char *h_units, *tmet_units, *rho_units, *rough_units;

} stripline_subs;

typedef struct STRIPLINE_LINE
{

  /* length and width */
  double l,l_sf;
  double w,w_sf;
  char *l_units, *w_units;

  /* characteristic impedance (ohms) */
  double z0;

  /* electrical length (degrees) */
  double len;
  double delay;
  double delay_sf;
  char *delay_units;

  /* open end length correction */
  double deltal,deltal_sf;
  char *deltal_units;

  double loss,losslen,losslen_sf,skindepth,skindepth_sf;
  char *losslen_units, *skindepth_units;

  /* incremental circuit model */
  double Ls, Rs, Cs, Gs;
  double Ls_sf, Rs_sf, Cs_sf, Gs_sf;
  char *Ls_units, *Rs_units, *Cs_units, *Gs_units;

  /* the actual characteristic impedance is Ro + j Xo */
  double Ro, Xo;

  /* frequency of analysis (Hertz) */
  double freq,freq_sf;
  char *freq_units;

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
