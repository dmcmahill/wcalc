/* $Id: microstrip.h,v 1.2 2001/02/17 16:56:36 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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

#ifndef __MICROSTRIP_H_
#define __MICROSTRIP_H_


typedef struct MICROSTRIP_SUBS
{
  double h,er,tmet,rho,rough,tand;
} microstrip_subs;

typedef struct MICROSTRIP_LINE
{

  /* length and width */
  double l;
  double w;

  /* characteristic impedance (ohms) */
  double z0;

  /* electrical length (degrees) */
  double len;

  /* open end length correction */
  double deltal;

  double keff,loss,losslen,skindepth;

  /* incremental circuit model */
  double Ls, Rs, Cs, Gs;

  /* the actual characteristic impedance is Ro + j Xo */
  double Ro, Xo;

  microstrip_subs *subs;

} microstrip_line;


double microstrip_calc(microstrip_line *line, double f);
int microstrip_syn(microstrip_line *line, double f, int flag);

void microstrip_line_free(microstrip_line * line);
microstrip_line *microstrip_line_new(void);
microstrip_subs *microstrip_subs_new(void);

/*
 * Flags for synthesis
 */

#define MLISYN_W    0    /* Synthesize the metal width         */
#define MLISYN_H    1    /* Synthesize the substrate thickness */
#define MLISYN_ES   2    /* Synthesize the dielectric constant */
#define MLISYN_L    3    /* Synthesize the length */


#endif /*__MICROSTRIP_H_*/
