/* $Id: air_coil.c,v 1.1 2001/10/05 00:37:29 dan Exp $ */

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
#include "alert.h"
#include "misc.h"


/* estimate of the enamel insulation thickness (inches) */
#define TINSUL 0.0015


static int air_coil_calc_int(air_coil_coil *coil, double freq, int flag);

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

int air_coil_calc(air_coil_coil *coil, double freq)
{
  int rslt;

  rslt=air_coil_calc_int(coil,freq,CALC_ALL);
#ifdef DEBUG_CALC
  printf("air_coil_calc():  returning %d\n",rslt);
#endif
  return rslt;
}

static int air_coil_calc_int(air_coil_coil *coil, double freq, int flag)
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
  air_coil_coil tmp_coil;

  if(coil->Nf < 1.0){
    alert("You have specified < 1 turn.  This\n"
	  "is not accurately modeled by this\n"
	  "model.\n");
    return -1;
  }
  /* 
   * are we using the given length directly or calculating it based on
   * fill?
   */
  lmin = coil->Nf*(awg2dia(coil->AWGf) + TINSUL);
  if (coil->use_fill){
    if(coil->fill < 1.0){
      alert("You have specified a fill < 1.  This\n"
	    "is not allowed as fill is the ratio of\n"
	    "coil length to minimum possible length\n"
	    "consistent with the number of turns\n"
	    "specified.\n");
      return -1;
    }
    coil->len = INCH2M(lmin)*coil->fill;
  }
  else{
    coil->fill = coil->len/INCH2M(lmin);
  }

  if(coil->fill < 1.0){
      alert("WARNING:  the specified value of N=%g is\n"
	    "too high.  You CAN NOT fit the desired\n"
	    "number of turns into the given length.\n"
	    "The maximum number of turns that can\n"
	    "fit in the given length is %g\n",
	    coil->Nf,
	    floor(coil->Nf*coil->fill));
      return -1;
  }
  pitch = M2INCH(coil->len) / coil->Nf;

  wirediam = awg2dia(coil->AWGf);
  turndiam = M2INCH(coil->dia) + wirediam;

  /*
   * the xk is calculated via the 'x' equation
   * from the Geffe reference.
   * x for the Nagaoka correction factor
   */
  xk = turndiam / M2INCH(coil->len); 

  /*
   * this 'x' is 1/x from the Geffe article, this
   * was one of the corrections
   * x for the Q and SRF equations
   */
  x = M2INCH(coil->len) / turndiam;  

  /*
   * check to see if x is in the correct range for our
   * polynomial approximation for Q and SRF to be good 
   * the lower limit should be 0.2, but I'm pushing it a little
   */
  if ( (x>5.0) || (x <= 0.1) ){
    alert("The length/diameter ratio, x, = %g\n"
	  "which is outside the range 0.1 <= x <= 5\n"
	  "over which the analysis is accurate\n",x);
    return -1;
  }


  /* Nagaoka correction factor */
  Kn = 1 / (1 + 0.45*xk - 0.005*xk*xk);

  /* current sheet correction due to spacing of turns */
  a = 2.3*log10(1.73*wirediam/pitch);
  b = 0.336*(1 - (2.5/coil->Nf) + (3.8/(coil->Nf * coil->Nf)));

  /* the 's' equation is corrected. */
  s = 1 - ((2*coil->len*(a+b))/(M_PI*turndiam*coil->Nf*Kn));

  /* nominal inductance */
  L0 = (pow((M_PI*turndiam*coil->Nf),2.0) * 2.54e-9)/M2INCH(coil->len);


  /* inductance in Henries */
  coil->L = L0 * Kn * s;


#ifdef DEBUG_CALC
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  Input values:\n");
  printf("air_coil_calc():  ----------------------\n");
  printf("air_coil_calc():  N    = %g\n",coil->Nf);
  printf("air_coil_calc():  AWG  = %g\n",coil->AWGf);
  printf("air_coil_calc():  I.D. = %g inches\n",M2INCH(coil->dia));
  printf("air_coil_calc():  len  = %g inches\n",M2INCH(coil->len));
  printf("air_coil_calc():  rho  = %g\n",coil->rho);
  printf("air_coil_calc():  freq = %g MHz\n",coil->freq*1e-6);
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

  /* SRF in Hz */
  coil->SRF = SRF*1e6;

#ifdef DEBUG_CALC
  printf("air_coil_calc():  SRF = %g MHz\n",coil->SRF*1e-6);
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
    alert("The length/diameter ratio, x, = %g\n"
	  "which is outside the range 0.1 <= x <= 5\n"
	  "over which the analysis is accurate\n",x);
    return -1;
  }


  Q0 = A * turndiam * sqrt(freq*1e-6);

  w = (freq*1e-6) / SRF;

  coil->Q     = Q0 * (1 - (w*w));
  coil->freq = freq;

#ifdef DEBUG_CALC
  printf("air_coil_calc():  Q = %g \n",coil->Q);
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

    len = M2INCH(coil->len);
    coil->fill = len/lmin;

    tmp_coil = *coil;
    tmp_coil.len  = INCH2M(lmin);
    tmp_coil.use_fill = 0;
    if (air_coil_calc_int(&tmp_coil,freq,CALC_MIN) != 0)
      return -1;
    coil->Lmax = tmp_coil.L;

  }

  return 0;
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

int air_coil_syn(air_coil_coil *coil, double f, int flag)
{
  double N=0;
  double N1=0;
  double N2=0;
  double lenPerTurn;

  double error;

  double len,len1,len2,Lsyn1,Lsyn2;

  double L;
  int use_fill;


  /* store the use_fill setting and switch to length for synthesis */
  use_fill = coil->use_fill;
  coil->use_fill=0;

  L = coil->L;

#ifdef DEBUG_SYN
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  Input values:\n");
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  N    = %g\n",coil->Nf);
  printf("air_coil_syn():  AWG  = %g\n",coil->AWGf);
  printf("air_coil_syn():  I.D. = %g inches\n",M2INCH(coil->dia));
  printf("air_coil_syn():  len  = %g inches\n",M2INCH(coil->len));
  printf("air_coil_syn():  rho  = %g\n",coil->rho);
  printf("air_coil_syn():  freq = %g MHz\n",coil->freq*1e-6);
  printf("air_coil_syn():  ----------------------\n");
  printf("air_coil_syn():  desired inductance  = %g nH\n",L*1e9);
  printf("air_coil_syn():  \n");
  printf("air_coil_syn():  ----------------------\n");
#endif

  /* initial guess for N */
  if (flag == AIRCOILSYN_NMIN){
    N = L*awg2dia(coil->AWGf)/(M_PI*M_PI*M2INCH(coil->dia)*M2INCH(coil->dia)*2.54e-9);
    N2 = N + 1;
#ifdef DEBUG_SYN
    printf("air_coil_syn():  Initial guess for N = %g\n",N);
#endif
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
      coil->len = INCH2M(len1);
      if (air_coil_calc_int(coil,f,CALC_MIN) != 0)
	return -1;
      Lsyn1     = coil->L;

      coil->Nf  = N2;
      coil->len = INCH2M(len2);
      if (air_coil_calc_int(coil,f,CALC_MIN) != 0)
	return -1;
      Lsyn2     = coil->L;
  
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
  } /* flag == AIRCOILSYN_NMIN */
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
    
    coil->len = INCH2M(len1);
    if (air_coil_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn1     = coil->L;
    
    coil->len = INCH2M(len2);
    if (air_coil_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn2     = coil->L;
    
  
    len = len2 + (L - Lsyn2)*(len2-len1)/(Lsyn2-Lsyn1);
    error = fabs(len-len2)/len;

#ifdef DEBUG_SYN
    printf("air_coil_syn():  len=%g\tlen1=%g\tlen2=%g\n",len,len1,len2);
    printf("                 L1=%g\tL2=%g\terror=%g\n",Lsyn1,Lsyn2,error);
#endif
  
    coil->len = INCH2M(len);

    /* fill in the rest of the data */
    if (air_coil_calc_int(coil,f,CALC_ALL) != 0)
      return -1;

  }

  if (flag == AIRCOILSYN_NFIX){
    if (len < N*lenPerTurn){
      alert("WARNING:  the specified value of N=%g is\n"
	    "too low.  You CAN NOT fit the desired\n"
	    "number of turns into the required length\n",N);
      return -1;
    }
  }
  
  /* restore the use_fill setting */
  coil->use_fill = use_fill;

  return 0;
}


void air_coil_free(air_coil_coil *coil)
{
  free(coil);
}


air_coil_coil *air_coil_new()
{
  air_coil_coil *newcoil;

  newcoil = (air_coil_coil *) malloc(sizeof(air_coil_coil));
  if(newcoil == NULL)
    {
      fprintf(stderr,"air_coil.c:air_coil_new(): malloc() failed\n");
      exit(1);
    }

  newcoil->Nf = 7.0;
  newcoil->len = INCH2M(0.2);
  newcoil->AWGf = 22.0;
  newcoil->rho = 1.0;
  newcoil->dia = INCH2M(0.14);
  newcoil->freq = 10e6;

  newcoil->use_fill=0;

  newcoil->len_sf = 25.4e-3;
  newcoil->dia_sf = 25.4e-3;
  newcoil->L_sf = 1.0e-9;
  newcoil->SRF_sf = 1.0e6;
  newcoil->freq_sf = 1.0e6;

  newcoil->len_units="inches";
  newcoil->dia_units="inches";
  newcoil->L_units="nH";
  newcoil->SRF_units="MHz";
  newcoil->freq_units="MHz";

  /* get the rest of the entries in sync */
  air_coil_calc(newcoil,newcoil->freq);

  return(newcoil);
}


