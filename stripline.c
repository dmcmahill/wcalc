/*      $Id$ */

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

#include <math.h>
#include <stdio.h>

#include "mathutil.h"
#include "physconst.h"
#include "stripline.h"



/*function [z0,len,loss]=slicalc(w,l,f,subs,flag)*/
/* SLICALC    Analyze stripline transmission line from physical parameters
 *
 *  [z0,len,loss]=slicalc(w,l,f,subs)
 *  calculates:
 *    z0    = characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    loss  = insertion loss (dB)
 *
 *  from:
 *    w     = stripline width (mils)
 *    l     = stripline length (mils)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *  The dielectric thickness above the stripline is assumed
 *  to be the same as below the stripline
 *
 *   /////////////////ground///////////////////////
 *   ----------------------------------------------
 *  (  dielectric,er         \/           /|\     (
 *   )             -------   --            |       )
 *  (             | metal | Tmet           | H    (
 *   )             -------   --            |       )
 *  (             <---W--->  /\           \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *  Part of the Filter Design Toolbox
 *  See Also:  MLICALC, TRSUBS
 *
 *  Dan McMahill, 8/11/97
 *  Copyright (c) 1997 by Dan McMahill.
 
 *  In the case where Tmet~0 (or at least Tmet << H), an exact
 *  solution has been derived by Cohn for the characteristic
 *  impedance.  Wheeler has given a correction for Tmet.
 *
 *  Reference:
 *
 *
 *    Stanislaw Rosloniec
 *    "Algorithms For Computer-Aided Design of Linear Microwave Circuits"
 *    Archtech House, 1990
 *    ISBN 0-89006-354-0
 *    TK7876.R67 1990
 *
 */

/*
if(nargin == 4),
   flag = 1;
elseif (nargin ~=5),
   error('Wrong number of arguments to SLICALC');
end
*/

double stripline_calc(stripline_rec *line, double f)
{
  /* physical width/length of line */
  double wmil,w,lmil,l;

  /* substrate parameters */
  double h,hmil,er,rho,tand,t,tmil,rough,roughmil;

  /* calculation variables */
  double k,kp,r,kf,z0,m,deltaW,A,v,len,loss;


  /* width and length of the stripline */
  wmil = line->w;
  w = MIL2M(wmil);

  lmil = line->l;
  l = MIL2M(lmil);

  /* Substrate dielectric thickness (mils) */
  hmil = line->subs->h;
  h = MIL2M(hmil);
  /* Substrate relative permittivity */
  er = line->subs->er;
  /* Metal resistivity relative to copper */
  rho = line->subs->rho;
  /* Loss tangent of the dielectric material */
  tand = line->subs->tand;
  /* Metal thickness (mils) */
  tmil = line->subs->tmet;
  t = MIL2M(tmil);
  /* Metalization roughness */
  roughmil = line->subs->rough;
  rough = MIL2M(rough);



  /*
   * Start of stripline calculations
   */


  /*
   * Characteristic Impedance
   */

  if(t < h/1000)
    {
      /*
       * Thin strip case:
       */
#ifdef DEBUG
       printf("stripline_calc(): Thin stripline analysis\n");
#endif
       k = 1/cosh(M_PI*w/(2*h));
   
      /*
       *  compute K(k)/K'(k) where
       * K is the complete elliptic integral of the first kind,
       * K' is the complementary complete elliptic integral of the first kind
       */

      kp = sqrt(1.0-pow(k,2.0));
      r = 1.0;
      kf=(1.0+k)/(1.0+kp);
      while(kf != 1.0)
	{
	  r = r*kf;
	  k = 2.0*sqrt(k)/(1.0+k);
	  kp = 2.0*sqrt(kp)/(1.0+kp);
	  kf=(1.0+k)/(1.0+kp);
	}
   
   
      z0 = 29.976*M_PI*r/sqrt(er);
    }
  else
    {

      /*
       * Finite strip case: 
       */

      m = 6.0*(h-t)/(3.0*h-t);
      deltaW = (t/M_PI)*(1.0 - 0.5*log( pow((t/(2*h-t)),2.0) + pow((0.0796*t/(w + 1.1*t)),m) ));
      A = 4.0*(h-t)/(M_PI*(w + deltaW));
      z0 = (30/sqrt(er))*log(1.0 + A*(2.0*A + sqrt(4.0*A*A + 6.27)));
    }

  /* done with Z0 calculation */



  /*
   * Electrical Length
   */

  /* propagation velocity (meters/sec) */
  v = LIGHTSPEED / sqrt(er);
  len = 180*l*f/v;


  /* Loss */

  /* XXX - need to get loss equations for stripline */
  loss = 0;

  return(z0);
}



/* function [w,l,loss]=slisyn(z0,len,f,subs) */
/* SLISYN     Synthesize microstrip transmission line from electrical parameters
 *
 *  [w,l,loss]=slisyn(z0,len,f,subs)
 *  calculates:
 *    w     = microstrip line width (mils)
 *    l     = microstrip line length (mils)
 *    loss  = insertion loss (dB)
 *
 *  from:
 *    z0    = characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *  The dielectric thickness above the stripline is assumed
 *  to be the same as below the stripline
 *
 *   /////////////////ground///////////////////////
 *   ----------------------------------------------
 *  (  dielectric,er         \/           /|\     (
 *   )             -------   --            |       )
 *  (             | metal | Tmet           | H    (
 *   )             -------   --            |       )
 *  (             <---W--->  /\           \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *  Part of the Filter Design Toolbox
 *  See Also:  SLICALC, MLICALC
 *
 *  Dan McMahill, 8/11/97
 *  Copyright (c) 1997 by Dan McMahill.
 *
 *
 *  Reference:
 *
 *
 *    Stanislaw Rosloniec
 *    "Algorithms For Computer-Aided Design of Linear Microwave Circuits"
 *    Archtech House, 1990
 *    ISBN 0-89006-354-0
 *    TK7876.R67 1990
 *
 */

int stripline_syn(stripline_rec *line, double f)
{
  /* physical width/length of line */
  double wmil,w,lmil,l;

  double z0,len;

  /* substrate parameters */
  double h,hmil,er,rho,tand,t,tmil,rough,roughmil;

  /* synthesis variables */
  double q,N,D,No,Do,k,m,B,w0,deltaW,v;
  int i;


  /* desired impedance (ohms) and electrical length (degrees) */
  z0 = line->z0;
  len = line->len;

  /* Substrate dielectric thickness (mils) */
  hmil = line->subs->h;
  h = MIL2M(hmil);
  /* Substrate relative permittivity */
  er = line->subs->er;
  /* Metal resistivity relative to copper */
  rho = line->subs->rho;
  /* Loss tangent of the dielectric material */
  tand = line->subs->tand;
  /* Metal thickness (mils) */
  tmil = line->subs->tmet;
  t = MIL2M(tmil);
  /* Metalization roughness */
  roughmil = line->subs->rough;
  rough = MIL2M(roughmil);

  /*
   * Start of stripline calculations
   */



  if(t < h/1000)
    {
      /*
       * Thin strip case:
       * disp('Doing thin strip synthesis');
       */
      q = exp(-29.976*M_PI*M_PI/(z0*sqrt(er)));
      N=0.0;
      D=0.5;
      i=0;
      No=1.0;
      Do=1.0;

      while((N-No) || (D-Do))
	{
	  i++;
	  No=N;
	  Do=D;
	  N = N + pow(q,(i*(i-1)));
	  D = D + pow(q,(i*i));
	}
#ifdef DEBUG
      printf("Took %d iters\n",i);
#endif

      k = sqrt(q)*(pow((N/D),2.0));
      w = (2.0*h/M_PI)*log(1.0/k + sqrt(pow((1.0/k),2.0) - 1.0));
    }
  else
    {
      /*
       * Finite strip case: 
       */
#ifdef DEBUG
      printf("stripline_syn(): doing finite thickness strip synthesis\n");
#endif
      m = 6.0*(h-t)/(3.0*h-t);
      B = exp(z0*sqrt(er)/30.0);
      w0 = (8.0*(h-t)/M_PI)*sqrt(B + 0.568)/(B-1);

      deltaW = (t/M_PI)*(1.0 - 0.5*log( pow((t/(2*h-t)),2.0) + pow((0.0796*t/(w0 - 0.26*t)),m) ));
      w = w0 - deltaW;
    }

  /* convert to mils */
  wmil = M2MIL(w);


  /*
   * Electrical Length
   */

  /* propagation velocity (meters/sec) */
  v = LIGHTSPEED / sqrt(er);
  l = (len/360.0)*(v/f);
  lmil = M2MIL(l);

  /*
   * Run analysis to get loss
   */

  /* XXX - need to add this call */
  /*[ztmp,ltmp,loss]=slicalc(w,l,f,subs);*/

  return(0);
}
