/* $Id: air_coil.c,v 1.1 2001/09/13 21:16:49 dan Exp $ */

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

//#define DEBUG_CALC   /* debug the air_coil_calc() function */
//#define DEBUG_SYN    /* debug the air_coil_syn() function  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "air_coil.h"
#include "misc.h"


/* estimate of the enamel insulation thickness (inches) */
#define TINSUL 0.0015


static double air_coil_calc_int(air_coil *coil, double freq, int flag);

/* air_coil_calc_int flags */
#define CALC_ALL  0  /* all calculation (calculate everything) */ 
#define CALC_MIN  1  /* minimum calculation (skip some)        */ 


/*
 *function [L,Q,SRF]=indcalc(N,len,AWG,dia,freq)
 * INDCALC    Calculate inductance parameters for single-layer solenoids
 * 
 *   [L,Q,SRF]=indcalc(N,len,AWG,dia,freq)
 *   calculates:
 *     L    = Inductance (Henries)
 *     Q    = Quality Factor at f = freq
 *     SRF  = Self Resonant Frequency (in MHz)
 *   from:
 *     N    = Number of Turns
 *     len  = length of inductor (inches)
 *     AWG  = Wire size (A.W.G.)
 *     dia  = Inside Diameter of Inductor (inches)
 *     freq = Frequency for Q Analysis (MHz)
 * 
 *   If 'freq' is not specified, it defaults to 10 MHz.
 * 
 *   Part of the Filter Design Toolbox
 *   See Also:  INDSYN
 *
 *   Dan McMahill, 1/17/97
 *   Copyright (c) 1997 by Dan McMahill.
 * 
 *   Reference:
 *   Geffe, Philip R. "The Design of Single-Layer Solenoids
 *        for RF Filters," Microwave Journal, Vol 39, No 12,
 *        December 1996, pp. 70-76
 * 
 *   Note:  There are a few errors in the original Geffe
 *     article.  They have been corrected here.
 *     According to the Microwave Journal, the January
 *     of 1997 will contain the corrections
 * 
 *   Note:  Some of the corrections appeared in the Microwave
 *     Journal, but not all.  I have corrected them here.
 *
 */

double air_coil_calc(air_coil *coil, double freq)
{
  double L;
  L = air_coil_calc_int(coil,freq,CALC_ALL);
  return L;
}

static double air_coil_calc_int(air_coil *coil, double freq, int flag)
{
  double pitch;
  double wirediam;
  double turndiam;

  /* Nagaoka correction factor variables */
  double xk;
  double x;
  double Kn;

  /* current sheet correction due to spacing of turns */
  double a,b,s;

  /* nominal inductance */
  double L0;

  /* variables for SRF calculation */
  double y;
  double cap,SRF;

  /* variables for loss */
  double A,Q0,w;

  /* regular and minimum possible length */
  double len, lmin;

  /* temp storage */
  air_coil tmp_coil;

  pitch = coil->len / coil->Nf;

  wirediam = awg2dia(coil->AWGf);
  turndiam = coil->dia + wirediam;

  /*
   * the xk is calculated via the 'x' equation
   * from the Geffe reference.
   * x for the Nagaoka correction factor
   */
  xk = turndiam / coil->len; 

  /*
   * this 'x' is 1/x from the Geffe article, this
   * was one of the corrections
   * x for the Q and SRF equations
   */
  x = coil->len / turndiam;  

  /*
   * check to see if x is in the correct range for our
   * polynomial approximation for Q and SRF to be good 
   * the lower limit should be 0.2, but I'm pushing it a little
   */
  if ( (x>5.0) || (x <= 0.1) ){
    printf("air_coil_calc():  Maximum len/turndiam, x = %g\n",x);
    printf("air_coil_calc():  Minimum len/turndiam, x = %g\n",x);
    printf("air_coil_calc():  x is outside the range 0.1 <= x <= 5\n");
    exit(1);
  }


  /* Nagaoka correction factor */
  Kn = 1 / (1 + 0.45*xk - 0.005*xk*xk);

  /* current sheet correction due to spacing of turns */
  a = 2.3*log10(1.73*wirediam/pitch);
  b = 0.336*(1 - (2.5/coil->Nf) + (3.8/(coil->Nf * coil->Nf)));

  /* the 's' equation is corrected. */
  s = 1 - ((2*coil->len*(a+b))/(M_PI*turndiam*coil->Nf*Kn));

  /* nominal inductance */
  L0 = (pow((M_PI*turndiam*coil->Nf),2.0) * 2.54e-9)/coil->len;


  /* inductance in Henries */
  coil->L = L0 * Kn * s;


#ifdef DEBUG_CALC
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  Input values:\n");
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  N    = %g\n",coil->Nf);
  printf("air_coil_calc():  AWG  = %g\n",coil->AWGf);
  printf("air_coil_calc():  I.D. = %g inches\n",coil->dia);
  printf("air_coil_calc():  len  = %g inches\n",coil->len);
  printf("air_coil_calc():  rho  = %g\n",coil->rho);
  printf("air_coil_calc():  freq = %g MHz\n",coil->Qfreq);
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  x  = %g\n",x);
  printf("air_coil_calc():  L0 = %g nH\n",L0*1e9);
  printf("air_coil_calc():  \n");
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  found an inductance of L = %g nH\n",coil->L*1e9);
  printf("air_coil_calc():  ----------------------\n");
#endif

  /*
   * find SRF 
   */

  y = ((0.301468*x + 0.493075)*x + 0.227858) / x;
  /* shunt capacitance in pF */
  cap = y*turndiam; 

  /* self resonant freq (MHz) */
  SRF = 1.0 / (2*M_PI*sqrt(coil->L*cap)); 
  coil->SRF = SRF;

#ifdef DEBUG_CALC
  printf("air_coil_calc():  SRF = %g MHz\n",SRF);
  printf("air_coil_calc():  ----------------------\n");
#endif

  /*
   * find Q 
   */

  if ( (0.1 <= x) && (x < 1.0) ){
    A = ((42.78120*x - 139.349)*x + 181.184)*x + 16.48780; 
  }
  else if( (1.0 <= x) && (x <= 5.0) ){
    A = ((0.751186*x - 9.49018)*x + 42.5060)*x + 68.11910;
  }
  else{
    fprintf(stderr,"air_coil_calc():  x=%g which is outside the allowed range\n",x);
    exit(1);
  }


  Q0 = A * turndiam * sqrt(freq);

  w = freq / SRF;

  coil->Q     = Q0 * (1 - (w*w));
  coil->Qfreq = freq;

#ifdef DEBUG_CALC
  printf("air_coil_calc():  Q = %g MHz\n",coil->Q);
  printf("air_coil_calc():  ----------------------\n");
#endif


  if(flag == CALC_ALL){
    /*
     * figure out the ratio of length to minimum length.
     * this is useful in figuring out how much room you have
     * to play with
     */

    /* minimum length with this wire and # of turns */
    lmin = coil->Nf*(awg2dia(coil->AWGf) + TINSUL);

    len = coil->len;
    coil->fill = len/lmin;

    tmp_coil = *coil;
    tmp_coil.len  = lmin;
    coil->Lmax = air_coil_calc_int(&tmp_coil,freq,CALC_MIN);
    coil->len = len;
  }

  return(coil->L);
}

/*
 * function [N,len]=indsyn(L,dia,AWG,N);
 *
 * INDSYN     Synthesize single-layer solenoid inductors
 * 
 * 	[N,len]=indsyn(L,dia,AWG)
 * 	calculates:
 * 		N    = Number of Turns
 * 		len  = length of inductor (inches)
 * 	from:
 * 		L    = Inductance (Henries)
 * 		dia  = Inside Diameter of Inductor (inches)
 * 		AWG  = Wire size (A.W.G.)
 * 
 * 	Alternatively, the number of turns may also be
 * 	specified by:
 * 	[N,len]=indsyn(L,dia,AWG,N)
 * 
 */

void air_coil_syn(air_coil *coil, double f, int flag)
{
  double N,N1,N2;
  double lenPerTurn;

  double error;

  double len,len1,len2,Lsyn1,Lsyn2;

  double L;

  L = coil->L;

#ifdef DEBUG_SYN
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  Input values:\n");
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  N    = %g\n",coil->Nf);
  printf("air_coil_syn():  AWG  = %g\n",coil->AWGf);
  printf("air_coil_syn():  I.D. = %g inches\n",coil->dia);
  printf("air_coil_syn():  len  = %g inches\n",coil->len);
  printf("air_coil_syn():  rho  = %g\n",coil->rho);
  printf("air_coil_syn():  freq = %g MHz\n",coil->Qfreq);
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  desired inductance  = %g nH\n",L*1e9);
  printf("air_coil_syn():  \n");
  printf("air_coil_syn():  ----------------------\n");
#endif

  /* initial guess for N */
  if (flag == AIRCOILSYN_NMIN){
    N = L*awg2dia(coil->AWGf)/(M_PI*M_PI*coil->dia*coil->dia*2.54e-9);
    N2 = N + 1;
  }

  /*
   * initial guess at a length per turn
   * The extra factor is more or less whats
   * due to the enamel insulation.
   */
  lenPerTurn= awg2dia(coil->AWGf) + TINSUL;


  if (flag == AIRCOILSYN_NMIN){
    /* we've been asked to synthesize using the minimum possible N */
#ifdef DEBUG_SYN
  printf("air_coil_syn():  Starting iteration to find Nmin\n");
#endif

    /* Iterate to find the minimum number of turns */
    error=1;
    while(error > 0.2){
      N1 = N2;
      N2 = N;
      len1 = N1*lenPerTurn;
      len2 = N2*lenPerTurn;

      coil->Nf  = N1;
      coil->len = len1;
      Lsyn1     = air_coil_calc_int(coil,f,CALC_MIN);

      coil->Nf  = N2;
      coil->len = len2;
      Lsyn2     = air_coil_calc_int(coil,f,CALC_MIN);
  
      N = N2 + (L - Lsyn2)*(N2-N1)/(Lsyn2-Lsyn1);
      error = fabs(N-N2);
#ifdef DEBUG_SYN
  printf("air_coil_syn():  Ntrial = %g  Error = %g  Lsyn1=%g Lsyn2=%g\n",N,error,Lsyn1,Lsyn2);
#endif
    }

    N = ceil(N);
    coil->Nf = N;
#ifdef DEBUG_SYN
  printf("air_coil_syn():  Nmin = %g\n",N);
  printf("air_coil_syn():  ----------------------\n");
#endif
  }
  else{
    /* we're supposed to use the user supplied N */
    N = coil->Nf;
  }



  /* Iterate to find the length */
  len2 = N*lenPerTurn;
  len  = 1.2*len2;

#ifdef DEBUG_SYN
  printf("air_coil_syn():  Starting iteration to find length\n");
  printf("air_coil_syn():  len = %g    len2 = %g\n",len,len2);
#endif

  error=1.0;
  while(error > 1e-8){
    len1 = len2;
    len2 = len;
    
    coil->len = len1;
    Lsyn1     = air_coil_calc_int(coil,f,CALC_MIN);
    
    coil->len = len2;
    Lsyn2     = air_coil_calc_int(coil,f,CALC_MIN);
    
  
    len = len2 + (L - Lsyn2)*(len2-len1)/(Lsyn2-Lsyn1);
    error = fabs(len-len2)/len;

#ifdef DEBUG_SYN
    printf("air_coil_syn():  len=%g\tlen1=%g\tlen2=%g\n",len,len1,len2);
    printf("                 L1=%g\tL2=%g\terror=%g\n",Lsyn1,Lsyn2,error);
#endif
  
    coil->len = len;

    /* fill in the rest of the data */
    air_coil_calc_int(coil,f,CALC_ALL);

  }

  if (flag == AIRCOILSYN_NFIX){
    if (len < N*lenPerTurn){
      fprintf(stderr,"air_coil_syn():  WARNING:  the specified value of N=%g is\n",N);
      fprintf(stderr,"air_coil_syn():  too low.  You CAN NOT fit the desired\n");
      fprintf(stderr,"air_coil_syn():  number of turns into the required length\n");
      exit(1);
    }
  }

}


void air_coil_free(air_coil *coil)
{
  free(coil);
}


air_coil *air_coil_new()
{
  air_coil *newcoil;

  newcoil = (air_coil *) malloc(sizeof(air_coil));
  if(newcoil == NULL)
    {
      fprintf(stderr,"air_coil_new: malloc() failed\n");
      exit(1);
    }

  return(newcoil);
}


