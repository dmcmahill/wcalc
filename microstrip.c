/* $Id: microstrip.c,v 1.2 2001/02/17 16:56:35 dan Exp $ */

/*
 * Copyright (c) 1997, 1998, 1999, 2000, 2001 Dan McMahill
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
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "microstrip.h"

#define NOLOSS 0
#define WITHLOSS 1

static double microstrip_calc_int(microstrip_line *line, double f, int flag);

/* 
 *
 *    z0    = characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    loss  = insertion loss (dB)
 *    eeff  = effective relative permitivity
 *    depth = skin depth (mils)
 *
 *  from:
 *    w     = microstrip line width (mils)
 *    l     = microstrip line length (mils)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *
 *                |<--W-->|
 *                 _______    
 *                | metal |   
 *   ----------------------------------------------
 *  (  dielectric,er                      /|\     (
 *   )                                 H   |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *
 *  Reference:
 *
 *  Effective permittivity and characteristic impedance:
 *
 *    E. Hammerstad and O. Jensen, "Accurate Models for Microstrip Computer-
 *      Aided Design" IEEE MTT-S, International Symposium Digest, Washington
 *      D.C., May 1980, pp. 407-409
 *
 *    M. Kirschning and R. H. Jansen, "Accurate Model for Effective Dielectric 
 *      Constant of Microstrip with Validity up to Millimetre-Wave Frequencies"
 *      Electronics Letters, Vol 18, No. 6, March 18th, 1982, pp 272-273.
 *
 *    Stanislaw Rosloniec
 *    "Algorithms For Computer-Aided Design of Linear Microwave Circuits"
 *    Archtech House, 1990
 *    ISBN 0-89006-354-0
 *    TK7876.R67 1990
 *
 *  Loss equations are from:
 *    E. H. Fooks and R. A. Zakarevicius
 *    "Microwave Engineering using Microstrip Circuits"
 *    Prentice-Hall, 1990
 *    ISBN 0-13-691650-3
 *    TK7876.F66 1989
 *
 *    E. O. Hammerstad edited by F. Bekkadal
 *    "Microstrip Handbook"
 *    ELAB Report STF44 A74169, February 1975
 *    University of Trondheim, Norway
 *
 *
 *
 * flag=1 enables loss calculations
 * flag=0 disables loss calculations
 */

/* function [z0,len,loss,eeff,depth]=mlicalc(w,l,f,subs,flag)*/
double microstrip_calc(microstrip_line *line, double f)
{
  double z;
  z=microstrip_calc_int(line, f, WITHLOSS);
  return(z);
}

/*
 * flag=1 enables loss calculations
 * flag=0 disables loss calculations
 */
static double microstrip_calc_int(microstrip_line *line, double f, int flag)
{
  double wmil,w,lmil,l;

  double h,hmil,er,rho,tand,t,tmil,rough,roughmil;

  double u,deltau;

  double A,B,E0;

  double fn;
  double P1,P2,P3,P4,P,EF;

  double F,z0;


  double R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17;

  double c,v;

  double z1,z2,z3,z4,z5,deltal,len;


  double eeff;

  double ld,mu,delta,depth;

  double lc,Res,loss;

  double sigma;


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

  /*   subs(6) = Metalization roughness */
  roughmil = line->subs->rough;
  rough = MIL2M(roughmil);

#ifdef DEBUG
  printf("starting microstrip_calc_int() with %g MHz and ",f/1.0e6);
  if(flag == NOLOSS)
    printf("NOLOSS\n");
  else
    printf("WITHLOSS\n");

  printf("L = %g mils\n",lmil);
  printf("W = %g mils\n",wmil);
  printf("Tmet = %g mils\n",M2MIL(t));

  printf("\n");

#endif

  /*
   * Start of microstrip calculations
   */


  /* Find u and correction factor for nonzero metal thickness */
  u = w/h;
  deltau = (t/(2*M_PI*h));

  if(t>0.0)
    {
      deltau = deltau
	*log(1.0 + 4.0*exp(1.0)*h/(t*pow(coth(sqrt(6.517*u)),2.0)))
	*(1.0 + 1.0/cosh(sqrt(er-1.0)));
    }
  u = u + deltau;

  

  /*
   * some test stuff to compare with Kobayashi
   * u = 2;
   * er=16;
   * hl0 = 1;
   * f=10e9;
   * l0 = 3e8/f;
   * h=hl0*l0;
   */

  /*
   * relative permittivity at f=0
   *  (Hammerstad and Jensen)
   */
   A = 1.0 + (1.0/49.0)
     *log((pow(u,4.0) + pow((u/52.0),2.0))/(pow(u,4.0) + 0.432)) 
     + (1.0/18.7)*log(1.0 + pow((u/18.1),3.0));
   B = 0.564*pow(((er-0.9)/(er+3.0)),0.053);
   E0 = (er+1.0)/2.0 + ((er-1.0)/2.0)*pow((1.0 + 10.0/u),(-A*B));


   /*
    * relative permittivity including dispersion
    *  (Kirschning and Jansen)
    */

    /* normalized frequency (GHz-cm)*/
   fn = 1e-7 * f * h;

   P1 = 0.27488 + 
     (0.6315 + (0.525 / (pow((1.0 + 0.157*fn),20.0))) )*u 
     - 0.065683*exp(-8.7513*u);
   P2 = 0.33622*(1.0 - exp(-0.03442*er));
   P3 = 0.0363*exp(-4.6*u)*(1.0 - exp(-pow((fn/3.87),4.97)));
   P4 = 1.0 + 2.751*( 1.0 -  exp(-pow((er/15.916),8.0)));
   P = P1*P2*pow(((0.1844 + P3*P4)*10.0*fn),1.5763);
   
   EF = (E0 + er*P)/(1.0 + P);


   /*
    * Characteristic Impedance
    *  (Hammerstad and Jensen)
    */
   F = 6.0 + (2.0*M_PI - 6.0)*exp(-pow((30.666/u),0.7528));
   z0 = (60.0/sqrt(EF)) * log(F/u + sqrt(1.0 + pow((2/u),2.0)));



   /*
    * Characteristic Impedance
    *  (Jansen and Kirschning)
    */

   /* normalized frequency (GHz-mm) */
   fn = 1.0e-6 * f * h;


   R1 = 0.03891*pow(er,1.4);
   R2 = 0.267*pow(u,7.0);
   R3 = 4.766*exp(-3.228*pow(u,0.641));
   R4 = 0.016 + pow((0.0514*er),4.524);
   R5 = pow((fn/28.843),12.0);
   R6 = 22.20*pow(u,1.92);
   R7 = 1.206 - 0.3144*exp(-R1)*(1.0 - exp(-R2));
   R8 = 1.0 + 1.275*(1.0 - exp(-0.004625*R3*pow(er,1.674)));
   R9 = (5.086*R4*R5/(0.3838 + 0.386*R4))*(exp(-R6)/(1.0 + 1.2992*R5));
   R9 = R9 * pow((er-1.0),6.0)/(1.0 + 10.0*pow((er-1),6.0));
   R10 = 0.00044*pow(er,2.136) + 0.0184;
   R11 = pow((fn/19.47),6.0)/(1.0 + 0.0962*pow((fn/19.47),6.0));
   R12 = 1.0 / (1.0 + 0.00245*u*u);
   R13 = 0.9408*pow(EF,R8) - 0.9603;
   R14 = (0.9408 - R9)*pow(E0,R8)-0.9603;
   R15 = 0.707*R10*pow((fn/12.3),1.097);
   R16 = 1.0 + 0.0503*er*er*R11*(1.0 - exp(-pow((u/15),6.0)));
   R17 = R7*(1.0 - 1.1241*(R12/R16)*exp(-0.026*pow(fn,1.15656)-R15));

   z0 = (60.0/sqrt(E0)) 
     * log(F/u + sqrt(1.0 + pow((2.0/u),2.0)))*pow((R13/R14),R17);

   /*
    * propagation velocity (meters/sec)
    */
   c = 2.997925e8;
   v = c / sqrt(EF);



   /*
    * End correction
    *  (Kirschning, Jansen, and Koster)
    */
   /* DAN should decide what to do about this */
   z1 = 0.434907*((pow(EF,0.81) + 0.26)/(pow(EF,0.81) - 0.189))
     *(pow(u,0.8544) + 0.236)/(pow(u,0.8544) + 0.87);
   z2 = 1.0 + (pow(u,0.371))/(2.358*er + 1.0);
   z3 = 1.0 + (0.5274*atan(0.084*(pow(u,(1.9413/z2)))))/(pow(EF,0.9236));
   z4 = 1.0 
     + 0.0377*atan(0.067*(pow(u,1.456)))*(6.0 - 5.0*exp(0.036*(1.0-er)));
   z5 = 1.0 - 0.218*exp(-7.5*u);

   deltal = h * z1 * z3 * z5 / z4;

   deltal=0.0;


   if(flag == WITHLOSS)
     {
       /* length in wavelengths */
       if(f > 0.0)
	 len= (l-deltal)/(v/f);
       else
	 len = 0.0;
       
       /* convert to degrees */
       len = 360.0*len;



       /* effective relative permittivity */
       eeff=EF;


       line->keff = eeff;
       line->len = len;

       /* calculate loss */
   

       /*
	* Dielectric Losses
	*/
   
       /* loss in nepers/meter */
       ld=(M_PI*f/v)*(er/EF)*((EF-1.0)/(er-1.0))*tand;
   
       /* loss in dB/meter */
       ld = 20.0*log10(exp(1.0)) * ld;
   
       /* loss in dB */
       ld = ld * l;
   
       /*
	* Conduction Losses
	*/
   
   
       /* calculate skin depth */
   
       /* conductivity */
       sigma = 5.8e7 /rho;
   
       /* permeability of free space */
       mu = 4.0*M_PI*1e-7;
   
       /* skin depth in meters */
       delta = sqrt(1.0/(M_PI*f*mu*sigma));
       depth = delta;
 
       /* skin depth in mils */
       delta = M2MIL(delta);
 
       
       /* warn the user if the loss calc is suspect. */
       if(t < 3.0*depth)
	 {
	   printf("Warning:  The metal thickness is less than\n");
	   printf("three skin depths.  Use the loss results with\n");
	   printf("caution.\n");
	 }

       /*
	* if the skinDepth is greater than Tmet, assume current
	* flows uniformly through  the conductor.  Then loss
	* is just calculated from the dc resistance of the
	* trace.  This is somewhat
	* suspect, but I dont have time right now to come up
	* with a better result.
	*/
       if(depth <= t)
	 {
   
	   /* store the substrate parameters */
	   /* XXX */
	   /* subsl = subs; */

	   line->subs->er = 1.0;
	   z2=microstrip_calc_int(line,f,NOLOSS);


	   line->subs->h = hmil + delta;
	   line->subs->tmet = tmil - delta;
	   line->w = wmil-delta;
	   z1=microstrip_calc_int(line,f,NOLOSS);
	   
	   line->subs->er = er;
	   line->subs->h = hmil;
	   line->subs->tmet = tmil;
	   line->w = wmil;

	   /* conduction losses, nepers per meter */
	   lc = (M_PI*f/c)*(z1 - z2)/z0;
	 }

	   /* "dc" case  */
       else if(t > 0.0)
	 {
	   /* resistance per meter = 1/(Area*conductivity) */
	   Res = 1/(w*t*sigma);  
  
	   /* conduction losses, nepers per meter */
	   lc = Res/(2.0*z0);

	   /*
	    * change delta to be equal to the metal thickness for
	    * use in surface roughness correction
	    */
	   delta = t;

	   /* no conduction loss case */
	 }
       else
	 {
	   lc=0.0;
	 }

 
       /* loss in dB/meter */
       lc = 20.0*log10(exp(1.0)) * lc;
   
       /* loss in dB */
       lc = lc * l;
   
       /* factor due to surface roughness
	* note that the equation in Fooks and Zakarevicius is slightly 
	* errored.   
	* the correct equation is penciled in my copy and was 
	* found in Hammerstad and Bekkadal
	*/
       lc = lc * (1.0 + (2.0/M_PI)*atan(1.4*pow((roughmil/delta),2.0)));
   
       /*
	* Total Loss
	*/
   
       loss = ld + lc;
   
       /*
	* Skin depth 
	* in mils
	*/
       depth = M2MIL(depth);
     }
   else
     {
       loss = 0.0;
       depth = 0.0;
     }

   line->loss = loss;
   line->losslen = loss/line->l;
   line->skindepth = depth;

   return (z0);
}

/*
 *  Synthesize microstrip transmission line from electrical parameters
 *
 *  calculates:
 *    w     = microstrip line width (mils)
 *    l     = microstrip line length (mils)
 *    loss  = insertion loss (dB)
 *    eeff  = effective relative permitivity
 *
 *  from:
 *    z0    = characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *                |<--W-->|
 *                 _______    
 *                | metal |   
 *   ----------------------------------------------
 *  (  dielectric,er                      /|\     (
 *   )                                 H   |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 */


int microstrip_syn(microstrip_line *line, double f)
{

  double h,er,l,lmil,wmin,wmax,abstol,reltol;
  int maxiters;
  double A,z0,w_h,B,w,wold,zold;
  int iters;
  int done;
  double zo,dzdw,c,v,len;
  double eeff;
  

  double initialw;

  z0 = line->z0;
  len = line->len;

  /* Substrate dielectric thickness (mils) */
  h = line->subs->h;

  /* Substrate relative permittivity */
  er = line->subs->er;

  /* temp value for l used while finding w */
  l = 1000.0;
  line->l=l;


  /* limits on the allowed range for w */
  wmin = 0.5;
  wmax = 1000;


  /* impedance convergence tolerance (ohms) */
  abstol = 0.1e-6;

  /* width relative convergence tolerance (mils) (set to 0.1 micron) */
  reltol = MICRON2MIL(0.01);

  maxiters = 500;

  /*
   * take an initial guess at w and take a trial step to initialize the
   * iteration
   */

  /*
   *  Synthesis formula from Hammerstad and Bekkadal 
   *  page -2-, (I.1.3), (I.1.4), (I.1.4), (I.1.4) 
   */
  A = ((er - 1.0)/(er + 1.0))*(0.226 + 0.121/er) 
    + (M_PI/376.63)*sqrt(2.0*(er + 1.0))*z0;
  w_h = 4.0/(0.5*exp(A) - exp(-A));

  if(w_h > 2.0)
    {
      B = M_PI*376.63/(2.0*z0*sqrt(er));
      w_h = (2.0/M_PI)
	*(B - 1.0 - log(2.0*B - 1.0) + ((er-1.0)/(2.0*er))
	  *(log(B-1.0) + 0.293 - 0.517/er));
    }

  w = w_h * h;

#ifdef DEBUG
  printf("\ninitial guess for w is %g mils, %g mm\n",w,MIL2MM(w));
#endif

  initialw = w;

  if(w >= wmax)
  {
    w = 0.95*wmax;
  }
  else if(w < wmin)
  {
    w = wmin;
  }

  wold = 1.01*w;

  line->w=wold;
  /* dont need loss calculations here */
  zold = microstrip_calc_int(line,f,NOLOSS);
  
  /*
   * check to see if we're too high or too low and bracket the
   * value for w.
   */

  if(zold < z0)
    wmax = wold;
  else
    wmin = wold;
 

  iters = 0;
  done = 0;

  while(!done)
    {
      iters = iters + 1;
   

#ifdef DEBUG
      printf("iteration #%d:  w = %g\n",iters,line->w);
#endif

      /* check zo for this value of w */
      line->w=w;

      /* dont need loss calculations here */
      zo = microstrip_calc_int(line,f,NOLOSS);

      /* keep track of the bounds on w. */
      if(zo < z0)
	wmax = w;
      else
	wmin = w;

   
      /* check for convergence */
      if(fabs(zo - z0) < abstol)
	{
	  done = 1;

#ifdef DEBUG
printf("Z0 Converged\n");
#endif
}
      else if(fabs(w-wold) < reltol)
	{
	  done = 1;
#ifdef DEBUG
printf("W Converged\n");
#endif
}
      else if(iters >= maxiters)
	{
	  fprintf(stderr,"MLISYN failed to converge in %d iterations\n",
		  maxiters);
	  exit(1);
	}

      /* calculate approximation to the derivative */
      dzdw = (zo-zold)/(w-wold);
      wold = w;
      zold = zo;
      
      /* take a newton iteration */
      w = w - (zo-z0)/dzdw;
      
      /*
       * if the newton iteration takes us out of the known range for w,
       * take a bisection step
       */

      if((w > wmax) | (w < wmin))
	{
#ifdef DEBUG
printf("Taking bisection step.\n");
#endif
w = (wmin + wmax)/2;
	}
    }

#ifdef DEBUG
  printf("Took %d iterations\n",iters);
#endif

  /* speed of light */
  c = LIGHTSPEED;

  /* velocity on line */
  microstrip_calc(line,f);
  eeff = line->keff;

  v = c / sqrt(eeff);


  l = (len/360)*(v/f);
  lmil = M2MIL(l);


  line->w=w;
  line->l=lmil;

  /* recalculate using real length to find loss  */
  microstrip_calc(line,f);
  
#ifdef DEBUG
  printf("synthesis for Z0=%g ohms and len=%g deg\n",line->z0,line->len);
  printf("produced:\n");
  printf("\twidth = %g\n\tlength = %g\n",w,l);
#endif

  return(0);
}


microstrip_line *microstrip_line_new()
{
  microstrip_line *newline;

  newline = (microstrip_line *) malloc(sizeof(microstrip_line));
  if(newline == NULL)
    {
      fprintf(stderr,"microstrip_line_new: malloc() failed\n");
      exit(1);
    }

  newline->subs = microstrip_subs_new();

  return(newline);
}

microstrip_subs *microstrip_subs_new(void)
{
  microstrip_subs *newsubs;

  newsubs = (microstrip_subs *) malloc(sizeof(microstrip_subs));
  if(newsubs == NULL)
    {
      fprintf(stderr,"microstrip_subs_new: malloc() failed\n");
      exit(1);
    }

  return(newsubs);
}
