/* $Id: ic_microstrip.c,v 1.8 2002/05/09 23:49:58 dan Exp $ */

/*
 * Copyright (c) 2001, 2002 Dan McMahill
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

/* Debug the analysis routine */
/* #define DEBUG_CALC */
/* #define DEBUG_ZUSTRIP */

/* Debug the synthesis routine */
/* #define DEBUG_SYN */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "alert.h"
#include "mathutil.h"
#include "physconst.h"
#include "ic_microstrip.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define NOLOSS 0
#define WITHLOSS 1

static double Zustrip(double h, double w, double t, double er);

/* 
 *
 *    NOTE:  All units are in meters, ohms, henries, farads, siemens
 *
 *    z0    = characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    loss  = insertion loss (dB)
 *    eeff  = effective relative permitivity
 *    depth = skin depth (m)
 *
 *  from:
 *    w     = microstrip line width (m)
 *    l     = microstrip line length (m)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *
 *                |<--W-->|
 *                 _______    
 *                | metal |   
 *   ----------------------------------------------
 *  (  dielectric,eox                     /|\     (
 *   )                                Tox  |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *  (  substrate,es,sigmas                /|\     (
 *   )                                     |       )
 *  (                                      |      (
 *   )                                 H   |       )
 *  (                                      |      (
 *   )                                     |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *
 *  Reference:
 *
 *  Emre Tuncer and Dean P. Neikirk, "Highly Accurate Quasi-Static
 *  Modeling of Microstrip Lines Over Lossy Substrates", IEEE
 *  Microwave and Guided Wave Letters, vol. 2, pp. 409-411, 1992 
 *
 */

int ic_microstrip_calc(ic_microstrip_line *line, double f)
{
  double omega;
  
  /* XXX please fixme!  I get segfaults without this.  happens on
     NetBSD-1.4.3, NetBSD-1.5.1, solaris-2.6, solaris-2.8 with 
     gcc version egcs-2.91.60 19981201 (egcs-1.1.1 release) 
     gcc version 2.95.1 19990816 (release)
     gcc version 2.95.2 19991024 (release)
  */

#if (defined(sparc) || defined(__sparc__)) && defined(__GNUC__)
  static double mu0, e0;
#else
  double mu0, e0;
#endif

  double Cox;
  double Csemi, Gsemi, Lsemi, Z0semi;

  /* 
   * total incremental shunt admittance and series impedance for the
   * MIS line 
   */
  complex *Ytot=NULL;
  complex *Ztot=NULL;

  /* the notation here matches the Tuncer and Neikirk paper */
  double a,b,k;
  complex *Zsemi=NULL;
  complex *betas=NULL;
  complex *gammai=NULL;
  double Zi;

  /* misc temp variables */
  complex *jba=NULL, *jbb=NULL, *num=NULL, *den=NULL;
  complex *tmpc1=NULL;
  complex *tmpc2=NULL;

  /* total transmission line variables */
  double Lmis, Rmis, Cmis, Gmis;
  complex *Zo_mis=NULL;

  complex *gamma_mis=NULL;
  double alpha_mis, beta_mis;
  double lambda_mis, loss_per_lambda_mis;

  double slowwave;

  /* frequency in rad/sec */
  omega = 2.0*M_PI*f;

  /* permeability and permitivitty of free space (H/m and F/m) */
  mu0 = 4.0*M_PI*1.0e-7;
  e0  = 1.0/(mu0*LIGHTSPEED*LIGHTSPEED);

#ifdef DEBUG_CALC
  printf("ic_microstrip_calc(): -------------- IC Microstrip Analysis ----------\n");
  printf("ic_microstrip_calc(): Metal width                 = %g um\n",line->w*1e6);
  printf("ic_microstrip_calc(): Oxide thickness             = %g um\n",line->subs->tox*1e6);
  printf("ic_microstrip_calc(): Oxide dielectric const.     = %g \n",line->subs->eox);
  printf("ic_microstrip_calc(): Substrate thickness         = %g um\n",line->subs->h*1e6);
  printf("ic_microstrip_calc(): Substrate dielectric const. = %g \n",line->subs->es);
  printf("ic_microstrip_calc(): Substrate conductivity      = %g 1/(ohm-cm)\n",line->subs->sigmas*0.01);
  printf("ic_microstrip_calc(): Substrate resistivity       = %g ohm-cm\n",100.0/(line->subs->sigmas));
  printf("ic_microstrip_calc(): Metal thickness             = %g um\n",line->subs->tmet*1e6);
  printf("ic_microstrip_calc(): Metal resistivity           = %g ohm-cm\n",line->subs->rho/100.0);
  printf("ic_microstrip_calc(): Metal roughness             = %g um-rms\n",line->subs->rough*1e6);
  printf("ic_microstrip_calc(): Frequency                   = %g GHz\n",f/1e9); 
  printf("ic_microstrip_calc(): -------------- ---------------------- ----------\n");
#endif

  /* 
   * Find the oxide capacitance assuming tox << w
   */
  Cox = ((line->subs->eox)*e0/(line->subs->tox))*line->w;
#ifdef DEBUG_CALC
  printf("Finding oxide capacitance\n");
  printf("Cox = %g fF/um\n",Cox*1e9);
#endif

  /*
   * Find substrate capacitance using Wheelers microstrip equations
   */
#ifdef DEBUG_CALC
  printf("Finding substrate capacitance\n");
#endif
  Lsemi = sqrt(mu0*e0)*Zustrip(line->subs->h,line->w,line->subs->tmet,1.0);
  Z0semi = Zustrip(line->subs->h,line->w,line->subs->tmet,line->subs->es);
#ifdef DEBUG_CALC
  printf("Calling Z0semi = Zustrip(h=%g,w=%g,tmet=%g,es=%g)\n",line->subs->h,line->w,line->subs->tmet,line->subs->es);
  printf("Z0semi = %g ohms\n",Z0semi);
#endif
  Csemi = Lsemi/(Z0semi*Z0semi);
#ifdef DEBUG_CALC
  printf("Csemi = %g fF/um\n",Csemi*1e15*1e-6);
  printf("Lsemi = %g pH/um\n",Lsemi*1e12*1e-6);
#endif

  /* substrate conductance */
#ifdef DEBUG_CALC
  printf("Finding substrate conductance\n");
#endif
  Gsemi = (line->subs->sigmas/(line->subs->es*e0))*Csemi;
#ifdef DEBUG_CALC
  printf("Gsemi = %g s/um\n",Gsemi*1e-6);
#endif



  /* admittance per unit length of the MIS transmission line */
#ifdef DEBUG_CALC
  printf("ic_microstrip_calc():  Finding Ytot\n");
  printf("                       -omega*omega*Csemi*Cox = %g\n",-omega*omega*Csemi*Cox);
  printf("                       omega*Cox*Gsemi = %g\n",omega*Cox*Gsemi);
  printf("                       --------------------\n");
  printf("                       Gsemi = %g\n",Gsemi);
  printf("                       omega*(Csemi + Cox) = %g\n",omega*(Csemi + Cox));
#endif
  num = c_complex_p(-omega*omega*Csemi*Cox,omega*Cox*Gsemi,num);
  den = c_complex_p(Gsemi,omega*(Csemi + Cox),den);
  Ytot = c_div_p(num,den,Ytot);
	       

#ifdef DEBUG_CALC
  printf("Ytot (1/(ohm-cm)) = %g + j%g\n",
	  0.01*Ytot->re,0.01*Ytot->im);
#endif

  /*
   * Now we need to find series impedance per unit length of the MIS
   * transmission line 
   */

  k = 3*line->subs->h + line->w/2;
  a = line->subs->h*line->w/(2*k-line->w);
  b = a + line->subs->h;
  
#ifdef DEBUG_CALC
  printf("k = %g\n",k);
  printf("a = %g\n",a);
  printf("b = %g\n",b);
#endif
  /*     
   * calculate the factor:
   *
   *       --------------------------
   *     /       j omega mu0
   *    / ------------------------
   *   /   sigmas + j omega es e0
   * \/
   *
   */
  num = c_complex_p(0.0,omega*mu0,num);
  den = c_complex_p(line->subs->sigmas,omega*line->subs->es*e0,den);
  Zsemi = c_div_p(num,den,Zsemi);
  Zsemi = c_sqrt_p(Zsemi,Zsemi);
		       

  /* 
   * divide by j w (yes, j*w, not j*omega) to get 
   *
   *
   *                   -------------------------
   *     1           /       j omega mu0
   *   -----        / ------------------------
   *    j w        /   sigmas + j omega es e0
   *             \/
   *
   */
  den = c_complex_p(0.0,line->w,den);
  Zsemi = c_div_p(Zsemi,den,Zsemi);

#ifdef DEBUG_CALC
  printf("ic_microstrip_calc():  Zsemi = %g + %gi\n",REAL_P(Zsemi),IMAG_P(Zsemi));
#endif
  /*
   *               ----------------------------------------
   *             /              /                      \
   * betas =    /  j omega mu0 | j omega es e0 + sigmas |
   *          \/                \                      /
   *
   */
  betas = c_complex_p(-omega*omega*mu0*(line->subs->es)*e0,omega*mu0*(line->subs->sigmas),betas);
  betas = c_sqrt_p(betas,betas);

  /* j betas a */
  jba = c_complex_p(0.0,1.0,jba);
  jba = c_rmul_p(a,jba,jba);
  jba = c_mul_p(jba,betas,jba);

  /* j betas b */
  jbb = c_complex_p(0.0,1.0,jbb);
  jbb = c_rmul_p(b,jbb,jbb);
  jbb = c_mul_p(jbb,betas,jbb);

#ifdef DEBUG_CALC
  printf("ic_microstrip_calc():  betas = %g + %gi\n",REAL_P(betas),IMAG_P(betas));
  printf("ic_microstrip_calc():  jba   = %g + %gi\n",REAL_P(jba),IMAG_P(jbb));
  printf("ic_microstrip_calc():  jbb   = %g + %gi\n",REAL_P(jbb),IMAG_P(jbb));
#endif

  /*
   * Now we need the multiplication factor which depends on the Hankel
   * functions 
   *
   *  H0,2(jbb) H0,1(jba)  -  H0,2(jba) H0,1(jbb)      num
   * ---------------------------------------------  = -----
   *  H0,2(jbb) H1,1(jba)  -  H1,2(jba) H0,1(jbb)      den
   *
   */
  tmpc1 = c_hankel0_2_p(jbb,tmpc1);
#ifdef DEBUG_CALC
  printf("ic_microstrip_calc():  H0,2(jbb) = %g + %gi\n",REAL_P(tmpc1),IMAG_P(tmpc1));
  printf("ic_microstrip_calc():  jbb   = %g + %gi\n",REAL_P(jbb),IMAG_P(jbb));
#endif
  tmpc2 = c_hankel0_1_p(jba,tmpc2);
  num   = c_mul_p(tmpc1,tmpc2,num);
  tmpc1 = c_hankel0_2_p(jba,tmpc1);
  tmpc2 = c_hankel0_1_p(jbb,tmpc2);
  tmpc1 = c_mul_p(tmpc1,tmpc2,tmpc1);
  num   = c_sub_p(num,tmpc1,num);


  tmpc1 = c_hankel0_2_p(jbb,tmpc1);
  tmpc2 = c_hankel1_1_p(jba,tmpc2);
  den   = c_mul_p(tmpc1,tmpc2,den);
  tmpc1 = c_hankel1_2_p(jba,tmpc1);
  tmpc2 = c_hankel0_1_p(jbb,tmpc2);
  tmpc1 = c_mul_p(tmpc1,tmpc2,tmpc1);
  den   = c_sub_p(den,tmpc1,den);


  /* multiply into our expression for Zsemi.  This is the final answer */
#ifdef DEBUG_CALC
  printf("Zsemi (partial)= %g + i%g\n",REAL_P(Zsemi),IMAG_P(Zsemi));
  printf("num = %g + i%g\n",REAL_P(num),IMAG_P(num));
  printf("den = %g + i%g\n",REAL_P(den),IMAG_P(den));
#endif
  tmpc1 = c_div_p(num,den,tmpc1);
  Zsemi = c_mul_p(Zsemi, tmpc1, Zsemi);

  /* find the oxide impedance and propagation constant */
  Zi = sqrt(mu0/((line->subs->eox)*e0))/line->w;
  gammai = c_complex_p(0.0,omega*sqrt(mu0*(line->subs->eox)*e0),gammai);

  /* combine to find the total MIS incremental series impedance */

  /*
   *            Zsemi + Zi     tanh(gammai tox)
   * Ztot = Zi ---------------------------------
   *            Zi    + Zsemi  tanh(gammai tox)
   */
  num = c_rmul_p(line->subs->tox,gammai,num);
  num = c_tanh_p(num,num);
  num = c_rmul_p(Zi, num, num);
  num = c_add_p(Zsemi,num,num);

  den = c_rmul_p(line->subs->tox,gammai,den);
  den = c_tanh_p(den,den);
  den = c_mul_p(Zsemi, den, den);
  tmpc1 = c_complex_p(Zi,0.0,tmpc1);
  den = c_add_p(tmpc1,den,den);
  
  Ztot = c_div_p(num,den,Ztot);
  Ztot = c_rmul_p(Zi,Ztot,Ztot);


#ifdef DEBUG_CALC
  printf("Zi    = %g ohms/um\n",Zi*1e-6);
  printf("Zsemi = %g + i%g\n",REAL_P(Zsemi),IMAG_P(Zsemi));
#endif

  /* whew! */

  /* 
   * now we have the incremental series impedance and shunt admittance
   * so we can proceed with finding things like the MIS characteristic
   * impedance, velocity, and loss.
   */

  /* elements in an incremental circuit model */
  Rmis = REAL_P(Ztot);
  Lmis = IMAG_P(Ztot)/omega;
  Gmis = REAL_P(Ytot);
  Cmis = IMAG_P(Ytot)/omega;


#ifdef DEBUG_CALC
  printf("Lmis = %g pH/um\n",Lmis*1e6);
  printf("Cmis = %g fF/um\n",Cmis*1e9);
  printf("Rmis = %g ohm/um\n",Rmis*1e-6);
  printf("Gmis = %g s/um\n",Gmis*1e-6);

  printf("Zmis = %g + i%g ohm/cm\n",REAL_P(Ztot)*1e-2,IMAG_P(Ztot)*1e-2);
  printf("Zmis = %g + i%g ohm/um\n",REAL_P(Ztot)*1e-6,IMAG_P(Ztot)*1e-6);
#endif

  /* characteristic impedance */
  Zo_mis = c_div_p(Ztot,Ytot,Zo_mis);
  Zo_mis = c_sqrt_p(Zo_mis,Zo_mis);

  /* propagation constant */
  gamma_mis = c_mul_p(Ztot,Ytot,gamma_mis);
  gamma_mis = c_sqrt_p(gamma_mis,gamma_mis);
  alpha_mis = REAL_P(gamma_mis);
  beta_mis  = IMAG_P(gamma_mis);
#ifdef DEBUG_CALC
  printf("gamma_mis = %g + i%g\n",alpha_mis,beta_mis);
#endif

  lambda_mis = 2.0*M_PI/beta_mis;
  slowwave = beta_mis/(omega*sqrt(mu0*e0));

#ifdef DEBUG_CALC
  printf("omega = %g rad/sec\n",omega);
#endif

  /* loss in dB per wavelentgh of transmission line */
  loss_per_lambda_mis = alpha_mis*lambda_mis*8.68;

  /* loss total for the line */
  
#ifdef DEBUG_CALC
  printf("Zo (ohms) = %g + j%g\n",REAL_P(Zo_mis),IMAG_P(Zo_mis));
  printf("MIS wavelength           = %g mm\n",1e3*lambda_mis);
  printf("Free space wavelength    = %g mm\n",1e3*LIGHTSPEED/f);
  printf("Slow wave factor         = %g\n",slowwave);
  printf("Loss per wavelength (dB) = %g\n ",loss_per_lambda_mis);
#endif

  /* copy over output */
  line->Lmis = Lmis;
  line->Rmis = Rmis;
  line->Cmis = Cmis;
  line->Gmis = Gmis;
  line->Ro   = REAL_P(Zo_mis);
  line->Xo   = IMAG_P(Zo_mis);

  /* XXX I probably should avoid 'keff' unless I can show 'mueff' = 1
   */
  line->keff = slowwave;


  /* XXX fix these */
  line->losslen=0;
  line->met_skindepth=0;
  line->subs_skindepth=0;

  /* electrical length */
  line->len  = 360*line->l/lambda_mis;


  free(Ytot);
  free(Ztot);
  free(Zsemi);
  free(betas);
  free(gammai);
  free(jba);
  free(jbb);
  free(num);
  free(den);
  free(tmpc1);
  free(tmpc2);
  free(Zo_mis);
  free(gamma_mis);

  return 0;
}



/*
 *  Synthesize ic_microstrip transmission line from electrical parameters
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
 *    subs  = substrate parameters.
 *
 *                |<--W-->|
 *                 _______    
 *                | metal |   
 *   ----------------------------------------------
 *  (  dielectric,eox                     /|\     (
 *   )                                Tox  |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *  (  substrate,es,sigmas                /|\     (
 *   )                                     |       )
 *  (                                      |      (
 *   )                                 H   |       )
 *  (                                      |      (
 *   )                                     |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 */


int ic_microstrip_syn(ic_microstrip_line *line, double freq, int flag)
{
  int rslt;

  double l;
  double Ro, Xo;
  double v,len;
  double eeff;

  /* the parameters which define the structure */
  double w;
  double tmet;
  double tox,eox;
  double h,es,sigmas;

  /* permeability and permitivity of free space */
  double mu0, e0;


  /* the optimization variables, current, min/max, and previous values */
  double var=0;
  double varmax=0;
  double varmin=0;
  double varold=0;
  
  /* errors due to the above values for the optimization variable */
  double err, errmax, errmin, errold;

  /* derivative */
  double deriv;

  /* the sign of the slope of the function being optimized */
  double sign;

  /* pointer to which parameter of the line is being optimized */
  double *optpar=NULL;

  /* number of iterations so far, and max number allowed */
  int iters;
  int maxiters=50;
  
  /* convergence parameters */
  double abstol=0.1e-6;
  double reltol=0.01e-6;



  /* flag to end optimization */
  int done=0;


  /* permeability and permitivitty of free space (H/m and F/m) */
  mu0 = 4*M_PI*1.0e-7;
  e0  = 1.0/(mu0*LIGHTSPEED*LIGHTSPEED);

  /*
   * figure out what parameter we're synthesizing and set up the
   * various optimization parameters.
   *
   * Basically what we need to know are
   *    1)  min/max values for the parameter
   *    2)  how to access the parameter
   *    3)  an initial guess for the parameter
   */

  /* XXX initialize and set limits in a ratio way */
  switch(flag){
  case IC_MLISYN_W:
    optpar = &(line->w);
    varmax = 100.0*line->subs->h;
    varmin = 1.0e-4*line->subs->h;
    var    = 0.2*line->subs->h;
    break;

  case IC_MLISYN_H:
    optpar = &(line->subs->h);
    varmax = 1.0e4*line->w;
    varmin = 0.01*line->w;
    var    = 5.0*line->w;
    break;

  case IC_MLISYN_TOX:
    optpar = &(line->subs->tox);
    varmax = 1000.0e-6;
    varmin = 0.01e-6;
    var    = 1.0e-6;
    break;

  default:
    fprintf(stderr,"ic_microstrip_synth():  illegal flag=%d\n",flag);
    exit(1);
    break;
  }

  /*
   * read values from the input line structure
   */

  Ro = line->Ro;
  Xo = line->Xo;


  /* Metal width, length, and thicknexx */
  w = line->w;
  len = line->l;
  tmet = line->subs->tmet;

  /* oxide thickness and relative permitivity */
  tox = line->subs->tox;
  eox = line->subs->eox;

  /* Substrate thickness, relative permitivity, and conductivity */
  h = line->subs->h;
  es = line->subs->es;
  sigmas = line->subs->sigmas;


  /* temp value for l used while finding w */
  l = 1000.0;
  line->l=l;


#ifdef DEBUG_SYN
  printf("\n");
  printf("ic_microstrip_syn(): -------------- IC Microstrip Synthesis ----------\n");
  printf("ic_microstrip_syn(): Metal width                 = %g um\n",line->w*1e6);
  printf("ic_microstrip_syn(): Oxide thickness             = %g um\n",line->subs->tox*1e6);
  printf("ic_microstrip_syn(): Oxide dielectric const.     = %g \n",line->subs->eox);
  printf("ic_microstrip_syn(): Substrate thickness         = %g um\n",line->subs->h*1e6);
  printf("ic_microstrip_syn(): Substrate dielectric const. = %g \n",line->subs->es);
  printf("ic_microstrip_syn(): Substrate conductivity      = %g 1/(ohm-cm)\n",line->subs->sigmas*0.01);
  printf("ic_microstrip_syn(): Substrate resistivity       = %g ohm-cm\n",100.0/(line->subs->sigmas));
  printf("ic_microstrip_syn(): Frequency                   = %g GHz\n",freq/1e9); 
  printf("ic_microstrip_syn(): -------------- ---------------------- ----------\n");
  printf("ic_microstrip_syn(): Desired Ro                  = %g ohm\n",Ro);
  printf("ic_microstrip_syn(): -------------- ---------------------- ----------\n");
  printf("ic_microstrip_syn(): Starting optimization value = %g\n",var);
  printf("ic_microstrip_syn(): Optimization flag           = %d\n",flag);
  printf("ic_microstrip_syn(): -------------- ---------------------- ----------\n");
#endif



  /* Initialize the various error values */
    *optpar = varmin;
    rslt = ic_microstrip_calc(line,freq);
    if (rslt)
      return rslt;
    errmin = line->Ro - Ro;

    *optpar = varmax;
    rslt = ic_microstrip_calc(line,freq);
    if (rslt)
      return rslt;
    errmax = line->Ro - Ro;

    *optpar = var;
    rslt = ic_microstrip_calc(line,freq);
    if (rslt)
      return rslt;
    err = line->Ro - Ro;

    varold = 0.99*var;
    *optpar = varold;
    rslt = ic_microstrip_calc(line,freq);
    if (rslt)
      return rslt;
    errold = line->Ro - Ro;

  /* see if we've actually been able to bracket the solution */
  if (errmax*errmin > 0){
#ifdef DEBUG_SYN
    printf("Could not bracket solution\n");
#endif
    alert("Could not bracket the solution.\n"
	  "Synthesis failed.\n");
    return -1;
  }
#ifdef DEBUG_SYN
  printf("Solution has been bracketed\n");
#endif
  
  /* figure out the slope of the error vs variable */
  if (errmax > 0)
    sign =  1.0;
  else
    sign = -1.0;

  iters = 0;
  done = 0;

  /* the actual iterations */
  while (!done){

    /* update the interation count */
    iters = iters + 1;
    
    /* calculate an estimate of the derivative */
    deriv = (err-errold)/(var-varold);

    /* copy over the current estimate to the previous one */
    varold = var;
    errold = err;

    /* try a quasi-newton iteration */
    var = var - err/deriv;
  
    
    /*
     * see if the new guess is within our bracketed range.  If so,
     * accept the new estimate.  If not, toss it out and do a
     * bisection step to reduce the bracket.
     */

    if ( (var>varmax) || (var<varmin) ){
#ifdef DEBUG_SYN
      printf("ic_microstrip_syn():  Taking a bisection step\n");
#endif
      var = (varmin + varmax)/2.0;
    }

    /* update the error value */
    *optpar = var;
    rslt = ic_microstrip_calc(line,freq);
    err = line->Ro - Ro;
    if (rslt)
      return rslt;

    /* update our bracket of the solution. */

    if (sign*err > 0)
      varmax = var;
    else
      varmin = var;

 
    /* check to see if we've converged */
    if (fabs(err) < abstol){
      done = 1;
#ifdef DEBUG_SYN
      printf("ic_microstrip_syn():  abstol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if ( fabs((var-varold)/var) < reltol){
      done = 1;
#ifdef DEBUG_SYN
      printf("ic_microstrip_syn():  reltol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if (iters >= maxiters){
      alert("Synthesis failed to converge in\n"
	    "%d iterations\n", maxiters);
      return -1;
    }
    

#ifdef DEBUG_SYN
      printf("ic_microstrip_syn(): iteration #%d:  var = %g\terr = %g\n",iters,var,err);
#endif
      /* done with iteration */
  }

  
  /* fill in the length/electrical length information */

  /* velocity on line */
  eeff = line->keff;
  v = LIGHTSPEED / sqrt(eeff);

  /* find the required physical length */
  line->l = (len/360.0)*(v/freq);

  /* recalculate using real length to find loss  */
  ic_microstrip_calc(line,freq);

#ifdef DEBUG_SYN  
  printf("ic_microstrip_syn(): -------------- -  Synthesis Results -  ----------\n");
  printf("ic_microstrip_syn(): Metal width                 = %g um\n",line->w*1e6);
  printf("ic_microstrip_syn(): Oxide thickness             = %g um\n",line->subs->tox*1e6);
  printf("ic_microstrip_syn(): Oxide dielectric const.     = %g \n",line->subs->eox);
  printf("ic_microstrip_syn(): Substrate thickness         = %g um\n",line->subs->h*1e6);
  printf("ic_microstrip_syn(): Substrate dielectric const. = %g \n",line->subs->es);
  printf("ic_microstrip_syn(): Substrate conductivity      = %g 1/(ohm-cm)\n",line->subs->sigmas*0.01);
  printf("ic_microstrip_syn(): Substrate resistivity       = %g ohm-cm\n",100.0/(line->subs->sigmas));
  printf("ic_microstrip_syn(): Frequency                   = %g GHz\n",freq/1e9); 
  printf("ic_microstrip_syn(): -------------- ---------------------- ----------\n");
  printf("ic_microstrip_syn(): Desired Ro                  = %g ohm\n",Ro);
  printf("ic_microstrip_syn(): Achieved Ro                 = %g ohm\n",line->Ro);
  printf("ic_microstrip_syn(): -------------- ---------------------- ----------\n");
#endif

  return 0;
}


ic_microstrip_line *ic_microstrip_line_new(void)
{
  ic_microstrip_line *newline;

  newline = (ic_microstrip_line *) malloc(sizeof(ic_microstrip_line));
  if(newline == NULL) {
    fprintf(stderr,"ic_microstrip_line_new: malloc() failed\n");
    exit(1);
  }

  newline->subs = ic_microstrip_subs_new();

  /* initialize the values to something */
  newline->l    = 1000.0e-6;
  newline->w    = 150.0e-6;
  newline->freq = 2.4e9;

  newline->subs->tox   = 1.0e-6;
  newline->subs->eox   = 4.0;
  newline->subs->h     = 250e-6;
  newline->subs->es    = 11.8;
  newline->subs->sigmas= 1e3;
  newline->subs->tmet  = 1.4e-6;
  newline->subs->rho   = 1.0;
  newline->subs->rough = 0.0;

  newline->l_sf = 1.0;
  newline->w_sf = 1.0;
  newline->freq_sf = 1.0;
  newline->subs->tox_sf = 1.0;
  newline->subs->h_sf = 1.0;
  newline->subs->sigmas_sf = 1.0;
  newline->subs->tmet_sf = 1.0;
  newline->subs->rho_sf = 1.0;
  newline->subs->rough_sf = 1.0;


  newline->l_units = "m";
  newline->w_units = "m";
  newline->freq_units = "Hz";
  newline->subs->tox_units = "m";
  newline->subs->h_units = "m";
  newline->subs->sigmas_units = "1 / ohm-m";
  newline->subs->tmet_units = "m";
  newline->subs->rho_units = "ohm-m";
  newline->subs->rough_units = "m";

#if defined(DEBUG_CALC) || defined(DEBUG_SYN)
  printf("ic_microstrip_line_new():  calling ic_microstrip_calc(%p,%g)\n",newline,newline->freq);
#endif

  /* and do a calculation to finish the initialization */
  ic_microstrip_calc(newline,newline->freq);
  
#if defined(DEBUG_CALC) || defined(DEBUG_SYN)
  printf("ic_microstrip_line_new():  all done\n");
#endif

  return(newline);
}

ic_microstrip_subs *ic_microstrip_subs_new(void)
{
  ic_microstrip_subs *newsubs;

  newsubs = (ic_microstrip_subs *) malloc(sizeof(ic_microstrip_subs));
  if(newsubs == NULL) {
      fprintf(stderr,"ic_microstrip_subs_new: malloc() failed\n");
      exit(1);
    }

  return(newsubs);
}

void ic_microstrip_line_free(ic_microstrip_line *line)
{
  ic_microstrip_subs_free(line->subs);
  free(line);
}

void ic_microstrip_subs_free(ic_microstrip_subs *subs)
{
  free(subs);
}

/*
 * Calculate microstrip characteristic impedance using the
 * Wheeler formulation. [wheeler 1977].
 *
 * h  = dielectric height
 * w  = conductor width
 * t  = conductor thickness
 * er = dielectric relative permitivitty
 *
 */

static double Zustrip(double h, double w, double t, double er)
{

  double deltaw, b, z0;
  double wp;
  double e;

  e = exp(1.0);
  deltaw = (t/M_PI)*
    ((1.0 + (1.0/er))/2.0)*
    log(4*e/sqrt(pow(t/h,2.0) +	 pow((1/M_PI)/(1.1 + w/t),2.0)));

  wp = w + deltaw;

  b = ((14 + 8/er)/11)*(4*h/wp);

  z0 = 42.4/sqrt(1.0 + er);
  z0 = z0*log(1.0 + 
	      (4*h/wp)*(b + sqrt(pow(b,2.0) + 
				 0.5*(1.0 + 1.0/er)*pow(M_PI,2.0))));


#ifdef DEBUG_ZUSTRIP
  printf("Zustrip(h=%g,w=%g,t=%g,er=%g) = %g Ohms\n",h,w,t,er,z0);
#endif

  return z0;
}


