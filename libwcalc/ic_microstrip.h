/* $Id: ic_microstrip.h,v 1.1 2001/10/05 00:37:31 dan Exp $ */

/*
 * Copyright (c)  2001 Dan McMahill
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

#ifndef __IC_MICROSTRIP_H_
#define __IC_MICROSTRIP_H_


typedef struct IC_MICROSTRIP_SUBS
{
  /* top metal parameters */
  double tmet,rho,rough;

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
  double freq;

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
