/* $Id: coupled_microstrip.c,v 1.22 2004/08/30 22:23:08 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004 Dan McMahill
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

/* #define DEBUG_SYN  */ /* debug coupled_microstrip_syn()  */
/* #define DEBUG_CALC */ /* debug coupled_microstrip_calc() */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "alert.h"
#include "defaults.h"
#include "mathutil.h"
#include "physconst.h"
#include "units.h"
#include "coupled_microstrip.h"
#include "coupled_microstrip_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/* Hammerstad and Jensen effective dielectric constant */
static double ee_HandJ(double u, double er);

/* Hammerstad and Jensen characteristic impedance */
static double z0_HandJ(double u);


/*function [z0e,z0o,len,loss,kev,kodd,deltale,deltalo]=cmlicalc(w,l,s,f,subs,flag)*/
/* CMLICALC   Analyze coupled microstrip transmission line from physical parameters
 *
 *  [z0e,z0o,len,loss,kev,kodd]=cmlicalc(w,l,s,f,subs)
 *  calculates:
 *    z0e   = even mode characteristic impedance (ohms)
 *    z0o   = odd mode characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    loss  = insertion loss (dB)
 *    kev   = even mode effective relative permitivity
 *    kodd  = odd mode effective relative permitivity
 *
 *  from:
 *    w     = microstrip line width (mils)
 *    l     = microstrip line length (mils)
 *    s     = spacing between lines (mils)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *          |<--W-->|<---S--->|<--W-->|
 *           _______           _______   
 *          | metal |         | metal |  
 *   ----------------------------------------------
 *  (  dielectric,er                      /|\     (
 *   )                                 H   |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *  Part of the Filter Design Toolbox
 *  See Also:  MLICALC, MLISYN, CMLISYN, TRSUBS

 *  Dan McMahill, 7/17/97
 *  Copyright (c) 1997 by Dan McMahill.

 *  Reference:
 *
 *  The equations for effective permittivity and characteristic
 *  impedance for the coupled lines are from:
 *
 *    Kirschning and Jansen (MTT):
 *    Manfred Kirschning and Rolf Jansen, "Accurate Wide-Range Design Equations for
 *      the Frequency-Dependent Characteristic of Parallel Coupled Microstrip
 *      Lines", IEEE Transactions on Microwave Theory and Techniques, Vol MTT-32,
 *      No 1, January 1984, p83-90.
 *      corrections in MTT-33, No 3, March 1985, p. 288
 *
 *  Effective permittivity and characteristic impedance for a single
 *  microstrip which is used in the Kirschning and Jansen (MTT) paper
 *  is from:
 *
 *    E. Hammerstad and O. Jensen, "Accurate Models for Microstrip Computer-
 *      Aided Design" IEEE MTT-S, International Symposium Digest, Washington
 *      D.C., May 1980, pp. 407-409
 *
 *    Kirschning and Jansen (EL):
 *    M. Kirschning and R. H. Jansen, "Accurate Model for Effective Dielectric 
 *      Constant of Microstrip with Validity up to Millimetre-Wave Frequencies"
 *      Electronics Letters, Vol 18, No. 6, March 18th, 1982, pp 272-273.
 *
 *  The loss equations are from Hammerstad and Jensen.
 *
 *
 *  I must acknowledge the transcalc project,
 *  http://transcalc.sourceforge.net
 *  They have an independent implementation of the same equations
 *  which provided something to compare to.  By comparing all of my
 *  intermediate results with all of theirs I found one bug in their
 *  code and one in mine.  A win for everyone!
 *
 */

int coupled_microstrip_calc(coupled_microstrip_line *line, double f)
{

  /* input physical dimensions */
  double w, l, s;

  /* substrate parameters */
  double h, er, rho, tand, t, rough;

  double u,g,deltau,deltau1,deltaur,T,V,EFE0,EF,AO,BO,CO,DO;
  double EFO0,fn;
  double P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15;
  double FEF,FOF,EFEF,EFOF,ZL0;
  double Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9,Q10,Q11,Q12,Q13,Q14;
  double Q15,Q16,Q17,Q18,Q19,Q20,Q21,Q22,Q23,Q24,Q25,Q26;
  double Q27,Q28,Q29;
  double z0e0,z0o0;
  double xP1,xP2,xP3,xP4,xP,EFF;
  double R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17;
  double ZLF;
  double RE,QE,PE,DE,CE;

  /* even/odd mode impedance at the frequency of interest */
  double z0ef,z0of;

  double v, len, uold, z1, z2, z3, z4, z5, d1, d2;

  /* even/odd mode open end correction lengths */
  double deltale, deltalo;

  double fnold;

  /* for skindepth calculation  */
  double mu,sigma,depth;


#ifdef DEBUG_CALC
  printf("coupled_microstrip_calc(): --------- Coupled_Microstrip Analysis ----------\n");
  printf("coupled_microstrip_calc(): Metal width                 = %g %s\n",
	 line->w/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_calc(): Metal spacing               = %g %s\n",
	 line->s/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_calc(): Metal thickness             = %g %s\n",
	 line->subs->tmet/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_calc(): Metal resistivity           = %g %s\n",
	 line->subs->rho/line->units_rho->sf, line->units_rho->name);
  printf("coupled_microstrip_calc(): Metal surface roughness     = %g %s-rms\n",
	 line->subs->rough/line->units_rough->sf, line->units_rough->name);
  printf("coupled_microstrip_calc(): Substrate thickness         = %g %s\n",
	 line->subs->h/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_calc(): Substrate dielectric const. = %g \n",
	 line->subs->er);
  printf("coupled_microstrip_calc(): Substrate loss tangent      = %g \n",
	 line->subs->tand);
  printf("coupled_microstrip_calc(): Frequency                   = %g %s\n",
	 line->freq/line->units_freq->sf, line->units_freq->name);
  printf("coupled_microstrip_calc(): -------------- ---------------------- ----------\n");
#endif



  w = line->w;
  l = line->l;
  s = line->s;

  /* Substrate dielectric thickness */
  h = line->subs->h;

  /* Substrate relative permittivity */
  er = line->subs->er;

  /* Metal resistivity */
  rho = line->subs->rho;

  /* Loss tangent of the dielectric material */
  tand = line->subs->tand;

  /* Metal thickness */
  t = line->subs->tmet;

  /*   subs(6) = Metalization roughness */
  rough = line->subs->rough;



  /*
   * Start of coupled microstrip calculations
   */

  /* Find u and correction factor for nonzero metal thickness */

  u = w/h;
  g = s/h;
  
  /*
   * verify that the geometry is in a range where the accuracy
   * is good.  This is given by (1) in Kirschning and Jansen (MTT)
   */
  if( (u  < 0.1) || (u  > 10.0) ) {
    alert(_("Warning:  u=w/h=%g is outside the range for highly accurate results\n"),u);
  }
  if( (g  < 0.1) || (g  > 10.0) ) {
    alert(_("Warning:  g=s/h=%g is outside the range for highly accurate results\n"),g);
  }
  if( (er < 1.0) || (er > 18.0) ) {
    alert(_("Warning:  er=%g is outside the range for highly accurate results\n"),er);
  }
  
  if(t>0.0) {
    /* find normalized metal thickness */
    T = t/h;
    
    /* (6) from Hammerstad and Jensen */
    deltau1 = (T/M_PI)*log(1.0 + 4.0*exp(1.0)/(T*pow(coth(sqrt(6.517*u)),2.0)));
    
    /* (7) from Hammerstad and Jensen */
    deltaur = 0.5*(1.0 + 1.0/cosh(sqrt(er-1.0)))*deltau1;
    
    deltau = deltaur;
    
#ifdef DEBUG_CALC
    printf("coupled_microstrip.c: coupled_microstrip_calc():  deltau1 = %g \n",deltau1);
    printf("                                                  deltaur = %g \n",deltaur);
    printf("                                                  t/h     = %g \n",T);
#endif
  }
  else {
    deltau = 0.0;
    deltau1 = 0.0;
    deltaur = 0.0;
  }

  /*
   * static even mode relative permittivity (f=0)
   * (3) from Kirschning and Jansen (MTT).  Accurate to 0.7 % over
   * "accurate" range
   */
  V = u*(20.0 + pow(g,2.0))/(10.0 + pow(g,2.0)) + g*exp(-g);
  /*
   * note:  The equations listed in (3) in Kirschning and Jansen (MTT)
   * are the same as in Hammerstad and Jensen but with u in H&J
   * replaced with V from K&J.
   */
  EFE0 = ee_HandJ(V,er);


  /*
   * static single strip, T=0, relative permittivity (f=0), width=w
   * This is from Hammerstad and Jensen.  
   */
  EF = ee_HandJ(u,er);



  /*
   * static odd mode relative permittivity (f=0)
   * This is (4) from Kirschning and Jansen (MTT).  Accurate to 0.5%.
   */
  AO = 0.7287*(EF - (er+1.0)/2.0)*(1.0 - exp(-0.179*u));
  BO = 0.747*er/(0.15 + er);
  CO = BO - (BO - 0.207) * exp(-0.414*u);
  DO = 0.593 + 0.694*exp(-0.562*u);

  /*
   * Note, this includes the published correction
   */
  EFO0 = ((er+1.0)/2.0 + AO - EF)*exp(-CO*(pow(g,DO))) + EF;

  /* normalized frequency  (2) from Kirschning and Jansen (MTT) */
  fn = 1e-6 * f * h;

  /*
   * check for range of validity for the dispersion equations.  p. 85
   * of Kirschning and Jansen (MTT) says fn <= 25 gives 1.4% accuracy.
   */
  if( fn > 25.0 ) {
    alert(_("Warning:  Frequency is higher (by %g %%) than the range\n"
	    "over which the dispersion equations are accurate."),100.0*(fn-25.0)/25.0);
  }
  
  /*
   * even/odd mode relative permittivity including dispersion
   */

  /* even mode dispersion.  (6) from Kirschning and Jansen (MTT) */
  P1 = 0.27488+ (0.6315 + 0.525/(pow((1.0 + 0.0157*fn),20.0)))*u - 0.065683*exp(-8.7513*u);
  P2 = 0.33622*(1.0 - exp(-0.03442*er));
  P3 = 0.0363*exp(-4.6*u)*(1.0 - exp(-pow((fn/38.7),4.97)));
  P4 = 1.0 + 2.751*(1.0 - exp(-pow((er/15.916),8.0)));
  P5 = 0.334*exp(-3.3*pow((er/15.0),3.0)) + 0.746;
  P6 = P5*exp(-pow((fn/18.0),0.368));
  P7 = 1.0 + 4.069*P6*(pow(g,0.479))*exp(-1.347*(pow(g,0.595)) - 0.17*(pow(g,2.5)));
  FEF = P1*P2*pow(((P3*P4 + 0.1844*P7)*fn),1.5763);

  /* odd mode dispersion.  (7) from Kirschning and Jansen (MTT) */
  P8 = 0.7168*(1.0 + 1.076/(1.0 + 0.0576*(er-1.0)));
  P9 = P8 - 0.7913*(1.0 - exp(-pow((fn/20.0),1.424)))*atan(2.481*pow((er/8.0),0.946));
  P10 = 0.242*pow((er-1.0),0.55);
  P11 = 0.6366*(exp(-0.3401*fn) - 1.0)*atan(1.263*pow((u/3.0),1.629));
  P12 = P9 + (1.0 - P9)/(1.0 + 1.183*pow(u,1.376));
  P13 = 1.695*P10/(0.414 + 1.605*P10);
  P14 = 0.8928 + 0.1072*(1.0 - exp(-0.42*pow((fn/20.0),3.215)));
  P15 = fabs(1.0 - 0.8928*(1.0 + P11)*P12*exp(-P13*(pow(g,1.092)))/P14);
  FOF = P1*P2*pow(((P3*P4 + 0.1844)*fn*P15),1.5763);

 #ifdef DEBUG_CALC
  printf("P1 = %g\n",P1);
  printf("P2 = %g\n",P2);
  printf("P3 = %g\n",P3);
  printf("P4 = %g\n",P4);
  printf("P5 = %g\n",P5);
  printf("P6 = %g\n",P6);
  printf("P7 = %g\n",P7);
  printf("P8 = %g\n",P8);
  printf("P9 = %g\n",P9);
  printf("P10 = %g\n",P10);
  printf("P11 = %g\n",P11);
  printf("P12 = %g\n",P12);
  printf("P13 = %g\n",P13);
  printf("P14 = %g\n",P14);
  printf("P15 = %g\n",P15);
 #endif

  /*
   * relative permittivities including dispersion via generalization
   * of Getsinger's dispersion relation.  This is (5) in Kirschning
   * and Jansen (MTT).
   */
  EFEF = er - (er - EFE0)/(1.0 + FEF);
  EFOF = er - (er - EFO0)/(1.0 + FOF);

#ifdef DEBUG_CALC
  printf("EFE0=%g, FEF=%g, EFEF=%g\n",EFE0,FEF,EFEF);
  printf("EFO0=%g, FOF=%g, EFOF=%g\n",EFO0,FOF,EFOF);
#endif 

  /*
   * static single strip, T=0, characteristic impedance (f=0)
   *  (Hammerstad and Jensen)
   */
  ZL0 = z0_HandJ(u)/sqrt(EF);

  /*
   * static even mode characteristic impedance (f=0)
   * (8) from Kirschning and Jansen (MTT)
   * 0.6% accurate
   */
  Q1 = 0.8695*(pow(u,0.194));
  Q2 = 1.0 + 0.7519*g + 0.189*(pow(g,2.31));
  Q3 = 0.1975 + pow((16.6 + pow((8.4/g),6.0)),-0.387) 
    + log((pow(g,10.0))/(1.0 + pow((g/3.4),10.0)))/241.0;
  Q4 = (2.0*Q1/Q2)/(exp(-g)*(pow(u,Q3)) + (2.0 - exp(-g))*(pow(u,-Q3)));
  z0e0 = ZL0 * sqrt(EF/EFE0) / (1.0 - (ZL0/377.0)*sqrt(EF)*Q4);


  /*
   * static odd mode characteristic impedance (f=0)
   * (9) from Kirschning and Jansen (MTT)
   * 0.6% accurate
   */
  Q5 = 1.794 + 1.14*log(1.0 + 0.638/(g + 0.517*(pow(g,2.43))));
  Q6 = 0.2305 
    + log((pow(g,10.0))/(1.0 + pow((g/5.8),10.0)))/281.3 
    + log(1.0 + 0.598*(pow(g,1.154)))/5.1;
  Q7 = (10.0 + 190.0*g*g)/(1.0 + 82.3*pow(g,3.0));
  Q8 = exp(-6.5 - 0.95*log(g) - pow((g/0.15),5.0));
  Q9 = log(Q7)*(Q8 + 1.0/16.5);
  Q10 = (Q2*Q4 - Q5*exp(log(u)*Q6*(pow(u,-Q9))))/Q2;
  z0o0 = ZL0 * sqrt(EF/EFO0) / (1.0 - (ZL0/377.0)*sqrt(EF)*Q10);

#ifdef DEBUG_CALC
  printf("Q1 = %g\n",Q1);
  printf("Q2 = %g\n",Q2);
  printf("Q3 = %g\n",Q3);
  printf("Q4 = %g\n",Q4);
  printf("Q5 = %g\n",Q5);
  printf("Q6 = %g\n",Q6);
  printf("Q7 = %g\n",Q7);
  printf("Q8 = %g\n",Q8);
  printf("Q9 = %g\n",Q9);
  printf("Q10 = %g\n",Q10);
  printf("ZL0 = %g\n",ZL0);
  printf("static even mode z0 = %g\n",z0e0);
  printf("static odd mode z0 = %g\n",z0o0);
#endif

  /*
   * relative permittivity including dispersion
   * of single microstrip of width W, Tmet=0
   * Kirschning and Jansen (EL)
   */

  /* normalized frequency (GHz-cm) */
  /* save fn */
  fnold = fn;

  fn = 1.0e-7 * f * h;

  /* (2) from Kirschning and Jansen (EL) */
  xP1 = 0.27488 + (0.6315 + (0.525 / (pow((1.0 + 0.157*fn),20.0))) )*u 
    - 0.065683*exp(-8.7513*u);
  xP2 = 0.33622*(1.0 - exp(-0.03442*er));
  xP3 = 0.0363*exp(-4.6*u)*(1.0 - exp(-pow((fn/3.87),4.97)));
  xP4 = 1.0 + 2.751*( 1.0 -  exp(-pow((er/15.916),8.0)));
  xP = xP1*xP2*pow(((0.1844 + xP3*xP4)*10.0*fn),1.5763);

  /* (1) from Kirschning and Jansen (EL) */
  EFF = (EF + er*xP)/(1.0 + xP);
  /* recall fn */
  fn=fnold;



  /*
   * Characteristic Impedance of single strip, Width=W, Tmet=0
   * Jansen and Kirschning
   */

  /* normalized frequency (GHz-mm)*/
  /* save fn */
  fnold = fn;
  fn = 1e-6 * f * h;
  
  /* (1) from Jansen and Kirschning */
  R1 = 0.03891*(pow(er,1.4));
  R2 = 0.267*(pow(u,7.0));
  R3 = 4.766*exp(-3.228*(pow(u,0.641)));
  R4 = 0.016 + pow((0.0514*er),4.524);
  R5 = pow((fn/28.843),12.0);
  R6 = 22.20*(pow(u,1.92));

  /* (2) from Jansen and Kirschning */
  R7 = 1.206 - 0.3144*exp(-R1)*(1.0 - exp(-R2));
  R8 = 1.0 + 1.275*(1.0 - 
		    exp(-0.004625*R3*
			pow(er,1.674)*
			pow(fn/18.365,2.745)));
  R9 = (5.086*R4*R5/(0.3838 + 0.386*R4))*(exp(-R6)/(1.0 + 1.2992*R5));
  R9 = R9 * (pow((er-1.0),6.0))/(1.0 + 10.0*pow((er-1.0),6.0));

  /* (3) from Jansen and Kirschning */
  R10 = 0.00044*(pow(er,2.136)) + 0.0184;
  R11 = (pow((fn/19.47),6.0))/(1.0 + 0.0962*(pow((fn/19.47),6.0)));
  R12 = 1.0 / (1.0 + 0.00245*u*u);

  /* (4) from Jansen and Kirschning */
  /*
   * EF is the 0 frequency (static) relative dielectric constant for a
   * single microstrip.
   * EFF is the frequency dependent relative dielectric constant for a
   * single microstrip given in Kirschning and Jansen (EL)
   */
  R13 = 0.9408*(pow(EFF,R8)) - 0.9603;
  R14 = (0.9408 - R9)*pow(EF,R8)-0.9603;
  R15 = 0.707*R10*(pow((fn/12.3),1.097));
  R16 = 1.0 + 0.0503*er*er*R11*(1.0 - exp(-(pow((u/15),6.0))));
  R17 = R7*(1.0 - 1.1241*(R12/R16)*exp(-0.026*(pow(fn,1.15656))-R15));

  /* ZLF = zero thickness single strip characteristic impedance including dispersion */
  ZLF = ZL0 * pow((R13/R14),R17);

  /* Q0 = R17 from zero thickness, single microstrip */
  Q0 = R17;

  /* recall fn */
  fn=fnold;


  /*
   * even mode characteristic impedance including dispersion
   * this is (10) from Kirschning and Jansen (MTT)
   */
  Q11 = 0.893*(1.0 - 0.3/(1.0 + 0.7*(er-1.0)));
  Q12 = 2.121*((pow((fn/20.0),4.91))
	       /(1 + Q11*pow((fn/20.0),4.91)))*exp(-2.87*g)*pow(g,0.902);
  Q13 = 1.0 + 0.038*pow((er/8.0),5.1);
  Q14 = 1.0 + 1.203*(pow((er/15.0),4.0))/(1.0 + pow((er/15.0),4.0));
  Q15 = 1.887*exp(-1.5*(pow(g,0.84)))*(pow(g,Q14)) 
    /(1.0 + 0.41*(pow((fn/15.0),3.0))*(pow(u,(2.0/Q13)))/(0.125 + pow(u,(1.626/Q13))));
  Q16 = (1.0 + 9.0/(1.0 + 0.403*pow((er-1.0),2.0)))*Q15;
  Q17 = 0.394*(1.0 - exp(-1.47*pow((u/7.0),0.672)))*(1.0 - exp(-4.25*pow((fn/20.0),1.87)));
  Q18 = 0.61*(1.0 - exp(-2.13*pow((u/8),1.593)))/(1.0 + 6.544*pow(g,4.17));
  Q19 = 0.21*(pow(g,4.0))/((1.0 + 0.18*(pow(g,4.9)))*(1.0 + 0.1*u*u)*(1 + pow((fn/24.0),3.0)));
  Q20 = (0.09 + 1.0/(1.0 + 0.1*pow((er-1.0),2.7)))*Q19;
  Q21 = fabs(1.0 - 42.54*pow(g,0.133)*exp(-0.812*g)*pow(u,2.5)/(1.0 + 0.033*pow(u,2.5)));
  
  RE = pow((fn/28.843),12.0);
  QE = 0.016 + pow((0.0514*er*Q21),4.524);
  PE = 4.766*exp(-3.228*pow(u,0.641));
  DE = 5.086*QE*(RE/(0.3838 + 0.386*QE))
    *(exp(-22.2*pow(u,1.92))/(1.0 + 1.2992*RE))
    *((pow((er-1.0),6.0))/(1.0 + 10.0*pow((er-1.0),6.0)));
  CE = 1.0+1.275*(1.0 - exp(-0.004625*PE*pow(er,1.674)*pow((fn/18.365),2.745)))
    - Q12 + Q16 - Q17 + Q18 + Q20;

  /*
   * EFF is the single microstrip effective dielectric constant from 
   * Kirschning and Jansen (EL).
   * Note:  This line contains one of the published corrections.
   * The second EFF from the original paper is replaced by EF.
   */
  z0ef = z0e0 
    * (pow((0.9408*pow(EFF,CE) - 0.9603),Q0))
    /(pow(((0.9408 - DE)*pow(EF,CE) - 0.9603),Q0));


  /*
   * odd mode characteristic impedance including dispersion
   * This is (11) from Kirschning and Jansen (MTT)
   */
  Q29 = 15.16/(1.0 + 0.196*pow((er-1.0),2.0));
  Q28 = 0.149*(pow((er-1.0),3.0))/(94.5 + 0.038*pow((er-1.0),3.0));
  Q27 = 0.4*pow(g,0.84)
    *(1.0 + 2.5*(pow((er-1.0),1.5))/(5.0 + pow((er-1.0),1.5)));
  Q26 = 30.0 
    - 22.2*(( pow(((er-1.0)/13.0),12.0)) 
	    / (1.0 + 3.0*pow(((er-1.0)/13.0),12.0))) 
    - Q29;
  Q25 = (0.3*fn*fn/(10.0 + fn*fn))
    *(1.0 + 2.333*(pow((er-1.0),2.0))/(5.0 + pow((er-1.0),2.0)));
  Q24 = 2.506*Q28*pow(u,0.894)
    *(pow(((1.0 + 1.3*u)*fn/99.25),4.29))/(3.575 + pow(u,0.894));
  Q23 = 1.0 + 0.005*fn*Q27/((1.0 + 0.812*pow((fn/15.0),1.9))*
			    (1.0 + 0.025*u*u));
  Q22 = 0.925*(pow((fn/Q26),1.536))/(1.0 + 0.3*pow((fn/30.0),1.536));
  
  /*
   * in this final expression, ZLF is the frequency dependent single
   * microstrip characteristic impedance from Jansen and Kirschning.
   */
  z0of = ZLF + (z0o0*pow((EFOF/EFO0),Q22) - ZLF*Q23)/(1.0 + Q24 + (pow((0.46*g),2.2))*Q25);

#ifdef DEBUG_CALC
  printf("1.0 + 0.812*pow((fn/15.0),1.9) = %g\n",
	 1.0 + 0.812*pow((fn/15.0),1.9));
  printf("1.0 + 0.025*u*u = %g\n",1.0 + 0.025*u*u);
  
  printf("fn = %g, u = %g\n",fn,u);
  printf("Q11 = %g\n",Q11);
  printf("Q12 = %g\n",Q12);
  printf("Q13 = %g\n",Q13);
  printf("Q14 = %g\n",Q14);
  printf("Q15 = %g\n",Q15);
  printf("Q16 = %g\n",Q16);
  printf("Q17 = %g\n",Q17);
  printf("Q18 = %g\n",Q18);
  printf("Q19 = %g\n",Q19);
  printf("Q20 = %g\n",Q20);
  printf("Q21 = %g\n",Q21);
  printf("Q22 = %g\n",Q22);
  printf("Q23 = %g\n",Q23);
  printf("Q24 = %g\n",Q24);
  printf("Q25 = %g\n",Q25);
  printf("Q26 = %g\n",Q26);
  printf("Q27 = %g\n",Q27);
  printf("Q28 = %g\n",Q28);
  printf("Q29 = %g\n",Q29);
  printf("Z0 single line with dispersion = %g\n",ZLF);
#endif

  /* accuracy check from page 87 in Kirschning and Jansen (MTT) */
  if( fn > 20.0 ) {
    alert(_("Warning:  Normalized frequency is higher than the\n"
	    "maximum for < 2.5 %% error in even/odd mode impedance."));
  }

  /*
   * electrical length 
   */

  /* propagation velocity (meters/sec) */
  v = LIGHTSPEED / sqrt(sqrt(EFEF*EFOF));

  /* length in wavelengths */
  len= l/(v/f);

  /* convert to degrees */
  len = 360.0*len;

  /*
   * Dielectric Losses
   */

  /* loss in nepers/meter */
  if (er > 1.0) {
    line->losslen_ev=(M_PI*f*sqrt(EFE0)/LIGHTSPEED)*(er/EFE0)*((EFE0-1.0)/(er-1.0))*tand;
    line->losslen_odd=(M_PI*f*sqrt(EFO0)/LIGHTSPEED)*(er/EFO0)*((EFO0-1.0)/(er-1.0))*tand;
  }
  else {
    /* XXX verify this one */
    line->losslen_ev = 0.0;
    line->losslen_odd = 0.0;
  }
  
  /* loss in dB/meter */
  line->losslen_ev  = 20.0*log10(exp(1.0)) * line->losslen_ev;
  line->losslen_odd = 20.0*log10(exp(1.0)) * line->losslen_odd;
  

  /* loss in dB */
  line->loss_ev = l * line->losslen_ev;
  line->loss_odd = l * line->losslen_odd;
  
#ifdef DEBUG_CALC
  printf("coupled_microstrip_calc():  Even mode dielectric loss = %g dB/m \n",
	 line->losslen_ev);
  printf("coupled_microstrip_calc():  Odd mode dielectric loss = %g dB/m \n",
	 line->losslen_odd);
#endif

  /* calculate skin depth */

  /* conductivity */
  sigma = 1.0 / rho;
   
  /* permeability of free space Henries/meter*/
  mu = 4.0*M_PI*1e-7;
   
  /* skin depth in meters */
  depth = sqrt(1.0/(M_PI*f*mu*sigma));
   

  /* warn the user if the loss calc is suspect. */
  if(t < 3.0*depth)
    {
      alert("Warning:  The metal thickness is less than\n"
	    "three skin depths.  Use the loss results with\n"
	    "caution.\n");
    }
  
  /*
   * if the skinDepth is greater than Tmet, assume current
   * flows uniformly through  the conductor.  Then loss
   * is just calculated from the dc resistance of the
   * trace.  This is somewhat
   * suspect, but I dont have time right now to come up
   * with a better result.
   */
#ifdef notdef   
  if(depth <= t)
    {
      
      /*
       * Find the current distribution factor.  This is (39) from
       * Hammerstad and Jensen.
       */
      Ko = exp(-1.2*pow( (z0e0 + z0o0)/(2.0*FREESPACEZ0), 0.7));

#ifdef DEBUG_CALC
#endif
      printf("coupled_microstrip_calc():  Ko = %g\n",Ko);

      /* skin resistance */
      Rs = 1.0 / (sigma * depth);

      /* conduction losses, nepers per meter */
      lc = (M_PI*z0e0*h*f/(Rs*LIGHTSPEED))*(u/Ko);
      /* dB/meter */
      lc = 20.0*log10(exp(1.0)) * lc;

      printf("coupled_microstrip_calc():  Even mode conduction loss = %g dB/m",
	     lc);

      /* conduction losses, nepers per meter */
      lc = (M_PI*z0o0*h*f/(Rs*LIGHTSPEED))*(u/Ko);
      /* dB/meter */
      lc = 20.0*log10(exp(1.0)) * lc;

      printf("coupled_microstrip_calc():  Even mode conduction loss = %g dB/m",
	     lc);
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
   * found in Hammerstad and Bekkadal as well as Hammerstad and Jensen 
   */
   /* XXX this was roughmil/delta double check it */
  lc = lc * (1.0 + (2.0/M_PI)*atan(1.4*pow((rough/delta),2.0)));
  
#endif

  /*
   * Single Line End correction
   *  (Kirschning, Jansen, and Koster)
   */

  /* deltal(2u,er) (per Kirschning and Jansen (MTT) notation) */
  uold = u;
  u = 2*u;
  z1 = 0.434907*((pow(EF,0.81) + 0.26)/(pow(EF,0.81) - 0.189))
    *(pow(u,0.8544) + 0.236)/(pow(u,0.8544) + 0.87);
  z2 = 1.0 + pow(u,0.371)/(2.358*er + 1.0);
  z3 = 1.0 + (0.5274*atan(0.084*(pow(u,(1.9413 / z2)))))/pow(EF,0.9236);
  z4 = 1.0 + 0.0377*atan(0.067*pow(u,1.456))*(6.0 - 5.0*exp(0.036*(1.0-er)));
  z5 = 1.0 - 0.218*exp(-7.5*u);
  
  d2 = h * z1 * z3 * z5 / z4;

  /* deltal(u,er) (per Kirschning and Jansen (MTT) notation) */
  u = uold;
  z1 = 0.434907*((pow(EF,0.81) + 0.26)/(pow(EF,0.81) - 0.189))
    *(pow(u,0.8544) + 0.236)/(pow(u,0.8544) + 0.87);
  z2 = 1 + pow(u,0.371)/(2.358*er + 1);
  z3 = 1 + (0.5274*atan(0.084*(pow(u,(1.9413 / z2)))))/pow(EF,0.9236);
  z4 = 1 + 0.0377*atan(0.067*pow(u,1.456))*(6.0 - 5.0*exp(0.036*(1.0-er)));
  z5 = 1 - 0.218*exp(-7.5*u);

  d1 = h * z1 * z3 * z5 / z4;


  /*
   * Even and Odd Mode End corrections
   * (12) and (13) from Kirschning and Jansen (MTT)
   */
  R1 = 1.187*(1.0-exp(-0.069*pow(u,2.1)));
  R2 = 0.343*pow(u,0.6187) + (0.45*er/(1.0+er))*(pow(u,(1.357 + 1.65/(1.0 + 0.7*er))));
  R3 = 0.2974*(1.0-exp(-R2));
  R4 = (0.271 + 0.0281*er)*(pow(g,(1.167*er/(0.66+er))))
    + (1.025*er/(0.687+er))*(pow(g,(0.958*er/(0.706+er))));

  deltale = (d2 - d1 + 0.0198*h*pow(g,R1))*exp(-0.328*pow(g,2.244)) +d1 ;
  deltalo = (d1 - h*R3)*(1.0 - exp(-R4)) + h*R3 ;

  /*  [z0e,z0o,len,loss,kev,kodd]=cmlicalc(w,l,s,f,subs) */
  /* copy over the results */

  line->z0e = z0ef;
  line->z0o = z0of;
  line->z0  = sqrt(z0ef*z0of);

  line->kev = EFEF;
  line->kodd = EFOF;

  /* coupling coefficient */
  line->k = (z0ef-z0of)/(z0ef+z0of);

  line->deltale = deltale;
  line->deltalo = deltalo;
  
  /* electrical length */
  line->len     = len;

  /* skin depth in m */
  line->skindepth =  depth;

  return 0;
}

/*function [w,l,s,loss,kev,kodd]=cmlisyn(z0e,z0o,len,f,subs) */
/* CMLISYN    Synthesize coupled microstrip transmission line from electrical parameters
 *
 *  [w,l,s,loss,kev,kodd]=cmlisyn(z0e,z0o,len,f,subs)
 *  calculates:
 *    w     = microstrip line width (mils)
 *    l     = microstrip line length (mils)
 *    s     = spacing between lines (mils)
 *    loss  = insertion loss (dB)
 *    kev   = even mode effective relative permitivity
 *    kodd  = odd mode effective relative permitivity
 *
 *  from:
 *    z0e   = even mode characteristic impedance (ohms)
 *    z0o   = odd mode characteristic impedance (ohms)
 *    len   = electrical length (degrees)
 *    f     = frequency (Hz)
 *    subs  = substrate parameters.  See TRSUBS for details.
 *
 *          |<--W-->|<---S--->|<--W-->|
 *           _______           _______   
 *          | metal |         | metal |  
 *   ----------------------------------------------
 *  (  dielectric,er                      /|\     (
 *   )                                 H   |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *  Part of the Filter Design Toolbox
 *  See Also:  CMLICALC, MLISYN, MLICALC, SLISYN, SLICALC, TRSUBS
 *
 *  Dan McMahill, 7/17/97
 *  Copyright (c) 1997 by Dan McMahill.
 *
 */

int coupled_microstrip_syn(coupled_microstrip_line *line, double f)
{

  double h, er, l, wmin, wmax, abstol, reltol;
  int maxiters;
  double z0, w;
  int iters;
  int done;
  double len;

  double s, smin, smax, z0e, z0o, k;
  double loss, kev, kodd, delta, cval, err, d;

  double AW,F1,F2,F3;

  double ai[] = {1, -0.301, 3.209, -27.282, 56.609, -37.746};
  double bi[] = {0.020, -0.623, 17.192, -68.946, 104.740, -16.148};
  double ci[] = {0.002, -0.347, 7.171, -36.910, 76.132, -51.616};

  int i;
  double dw, ds;
  double ze0=0,ze1,ze2,dedw,deds;
  double zo0=0,zo1,zo2,dodw,dods;

#ifdef DEBUG_SYN
  printf("coupled_microstrip_syn(): -------- Coupled_Microstrip Synthesis ----------\n");
  printf("coupled_microstrip_syn(): Metal width                 = %g %s\n",
	 line->w/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_syn(): Metal spacing               = %g %s\n",
	 line->s/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_syn(): Metal thickness             = %g %s\n",
	 line->subs->tmet/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_syn(): Metal relative resistivity  = %g %s\n",
	 line->subs->rho/line->units_rho->sf, line->units_rho->name);
  printf("coupled_microstrip_syn(): Metal surface roughness     = %g %s-rms\n",
	 line->subs->rough/line->units_rough->sf, line->units_rough->name);
  printf("coupled_microstrip_syn(): Substrate thickness         = %g %s\n",
	 line->subs->h/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_microstrip_syn(): Substrate dielectric const. = %g \n",
	 line->subs->er);
  printf("coupled_microstrip_syn(): Substrate loss tangent      = %g \n",
	 line->subs->tand);
  printf("coupled_microstrip_syn(): Frequency                   = %g %s\n",
	 line->freq/line->units_freq->sf, line->units_freq->name);
  printf("coupled_microstrip_syn(): -------------- ---------------------- ----------\n");
  printf("coupled_microstrip_syn(): Desired Zo                  = %g ohm\n", line->z0);
  printf("coupled_microstrip_syn(): Desired k                   = %g \n", line->k);
  printf("coupled_microstrip_syn(): Desired Even Mode Zo        = %g ohm\n", line->z0e);
  printf("coupled_microstrip_syn(): Desired Odd Mode Zo         = %g ohm\n", line->z0o);
  printf("coupled_microstrip_syn(): Desired electrical length   = %g degrees\n",line->len);
  printf("coupled_microstrip_syn(): -------------- ---------------------- ----------\n");
#endif

  len = line->len;

  /* Substrate dielectric thickness (m) */
  h = line->subs->h;

  /* Substrate relative permittivity */
  er = line->subs->er;

  /* impedance and coupling */
  z0 = line->z0;
  k = line->k;

  /* even/odd mode impedances */
  z0e = line->z0e;
  z0o = line->z0o;

  if( line->use_z0k ) {
    /* use z0 and k to calculate z0e and z0o */
    z0o = z0*sqrt((1.0 - k) / (1.0 + k));
    z0e = z0*sqrt((1.0 + k) / (1.0 - k));
  } else {
    /* use z0e and z0o to calculate z0 and k */
    z0 = sqrt(z0e*z0o);
    k = (z0e - z0o)/(z0e + z0o);
  }

  /* temp value for l used while finding w and s */
  l = 1000.0;
  line->l=l;


  /* limits on the allowed range for w */
  wmin = MIL2M(0.5);
  wmax = MIL2M(1000);

  /* limits on the allowed range for s */
  smin = MIL2M(0.5);
  smax = MIL2M(1000);


  /* impedance convergence tolerance (ohms) */
  abstol = 1e-6;

  /* width relative convergence tolerance (mils) (set to 0.1 micron) */
  reltol = MICRON2MIL(0.1);

  maxiters = 50;


  /*
   * Initial guess at a solution
   */
  AW = exp(z0*sqrt(er+1.0)/42.4) - 1.0;
  F1 = 8.0*sqrt(AW*(7.0 + 4.0/er)/11.0 + (1.0 + 1.0/er)/0.81)/AW;

  F2 = 0;
  for (i=0; i<=5 ; i++)
    {
      F2 = F2 + ai[i] * pow(k,i);
    }

  F3 = 0;
  for (i=0 ; i<=5 ; i++)
    {
      F3 = F3 + (bi[i] - ci[i]*(9.6 - er))*pow((0.6 - k),(double)(i));
    }

  w = h*fabs(F1*F2);
  s = h*fabs(F1*F3);


#ifdef DEBUG_SYN
  printf("coupled_microstrip_syn():  AW=%g, F1=%g, F2=%g, F3=%g\n",
	 AW, F1, F2, F3);
  
  printf("coupled_microstrip_syn():  Initial estimate:\n"
	     "                w = %g %s, s = %g %s\n", 
	     w/line->units_lwst->sf, line->units_lwst->name,
	     s/line->units_lwst->sf, line->units_lwst->name);
#endif

  l=100;
  loss=0;
  kev=1;
  kodd=1;


  iters = 0;
  done = 0;
  if( w < s )
    delta = 1e-3*w;
  else
    delta = 1e-3*s;
  
  delta = MIL2M(1e-5);

  cval = 1e-12*z0e*z0o;

  /* 
   * We should never need anything anywhere near maxiters iterations.
   * This limit is just to prevent going to lala land if something
   * breaks. 
   */
  while( (!done) && (iters < maxiters) )
    {
      iters++;
      line->w = w;
      line->s = s;
      coupled_microstrip_calc(line, line->freq);
      
      ze0 = line->z0e;
      zo0 = line->z0o;

#ifdef DEBUG_SYN
      printf("Iteration #%d ze = %g\tzo = %g\tw = %g %s\ts = %g %s\n", 
	     iters, ze0, zo0, 
	     w/line->units_lwst->sf, line->units_lwst->name,
	     s/line->units_lwst->sf, line->units_lwst->name);
#endif

      /* check for convergence */
      err = pow((ze0-z0e),2.0) + pow((zo0-z0o),2.0);
      if(err < cval) {
	done = 1;
      } else {
	/* approximate the first jacobian */
	line->w = w + delta;
	line->s = s;
	coupled_microstrip_calc(line, line->freq);
	ze1 = line->z0e;
	zo1 = line->z0o;

	line->w = w;
	line->s = s + delta;
	coupled_microstrip_calc(line, line->freq);
	ze2 = line->z0e;
	zo2 = line->z0o;

	dedw = (ze1 - ze0)/delta;
	dodw = (zo1 - zo0)/delta;
	deds = (ze2 - ze0)/delta;
	dods = (zo2 - zo0)/delta;
	
	/* find the determinate */
	d = dedw*dods - deds*dodw;
	
	/* estimate the new solution */
	dw = -1.0 *  ((ze0-z0e)*dods - (zo0-z0o)*deds)/d;
	w = fabs(w + dw);

	ds =         ((ze0-z0e)*dodw - (zo0-z0o)*dedw)/d;
	s = fabs(s + ds);

#ifdef DEBUG_SYN
	printf("coupled_microstrip_syn():  delta = %g, determinate = %g\n", delta, d);
	printf("coupled_microstrip_syn():  ze0 = %16.8g,  ze1 = %16.8g,  ze2 = %16.8g\n",
	       ze0, ze1, ze2);
	printf("coupled_microstrip_syn():  zo0 = %16.8g,  zo1 = %16.8g,  zo2 = %16.8g\n",
	       zo0, zo1, zo2);
	printf("coupled_microstrip_syn(): dedw = %16.8g, dodw = %16.8g\n",
	       dedw, dodw);
	printf("coupled_microstrip_syn(): ze1-ze0 = %16.8g, ze2-ze0 = %16.8g\n",
	       ze1-ze0, ze2-ze0);
	printf("coupled_microstrip_syn(): deds = %16.8g, dods = %16.8g\n",
	       deds, dods);
	printf("coupled_microstrip_syn(): zo1-zo0 = %16.8g, zo2-zo0 = %16.8g\n",
	       zo1-zo0, zo2-zo0);
	printf("coupled_microstrip_syn(): dw = %g %s, ds = %g %s\n",
	       dw/line->units_lwst->sf, line->units_lwst->name,
	       ds/line->units_lwst->sf, line->units_lwst->name);
	printf("-----------------------------------------------------\n");
#endif
      }
    }
  
  line->w = w;
  line->s = s;
  coupled_microstrip_calc(line, line->freq);


#ifdef DEBUG_SYN
  printf("Took %d iterations, err = %g\n", iters, err);
  printf("ze = %g\tzo = %g\tz0e = %g\tz0o = %g\n", ze0, zo0, z0e, z0o);
#endif

  return(0);
}

/*
 * Effective dielectric constant from Hammerstad and Jensen
 */
static double ee_HandJ(double u, double er)
{
  double A,B,E0;

  /* (4) from Hammerstad and Jensen */
  A = 1.0 + (1.0/49.0)
    *log((pow(u,4.0) + pow((u/52.0),2.0))/(pow(u,4.0) + 0.432)) 
    + (1.0/18.7)*log(1.0 + pow((u/18.1),3.0));
  
  /* (5) from Hammerstad and Jensen */
  B = 0.564*pow(((er-0.9)/(er+3.0)),0.053);

  
  /* 
   * zero frequency effective permitivity.  (3) from Hammerstad and
   * Jensen.  This is ee(ur,er) thats used by (9) in Hammerstad and
   * Jensen.
   */
  E0 = (er+1.0)/2.0 + ((er-1.0)/2.0)*pow((1.0 + 10.0/u),(-A*B));

#ifdef DEBUG_CALC
  printf("ee_HandJ():  A = %g, B = %g, E0 = %g\n", A, B, E0);
#endif

  return E0;
}


/* 
 * Characteristic impedance from (1) and (2) in Hammerstad and Jensen 
 */
static double z0_HandJ(double u)
{
  double F,z01;
  
  /* (2) from Hammerstad and Jensen.  'u' is the normalized width */
  F = 6.0 + (2.0*M_PI - 6.0)*exp(-pow((30.666/u),0.7528));

  /* (1) from Hammerstad and Jensen */
  /* XXX decide on which to use here */
  z01 = (FREESPACEZ0/(2*M_PI)) * log(F/u + sqrt(1.0 + pow((2/u),2.0)));
  z01 = (377.0/(2*M_PI)) * log(F/u + sqrt(1.0 + pow((2/u),2.0)));

#ifdef DEBUG_CALC
  printf("microstrip.c: z0_HandJ(%g) = %g Ohms. FREESPACEZ0=%g Ohms\n",
         u,z01,FREESPACEZ0);
#endif

  return z01;
}

void coupled_microstrip_line_free(coupled_microstrip_line * line)
{
  free(line->subs);
  wc_units_free(line->units_lwst);
  wc_units_free(line->units_len);
  wc_units_free(line->units_freq);
  wc_units_free(line->units_loss);
  wc_units_free(line->units_losslen);
  wc_units_free(line->units_rho);
  wc_units_free(line->units_rough);
  wc_units_free(line->units_delay);
  wc_units_free(line->units_depth);
  wc_units_free(line->units_deltal);
  free(line);
}


coupled_microstrip_line *coupled_microstrip_line_new()
{
  coupled_microstrip_line *newline;

  newline = (coupled_microstrip_line *) malloc(sizeof(coupled_microstrip_line));
  if(newline == NULL)
    {
      fprintf(stderr,"coupled_microstrip_line_new: malloc() failed\n");
      exit(1);
    }

  newline->subs = microstrip_subs_new();

  /* create the units */
  newline->units_lwst    = wc_units_new(WC_UNITS_LENGTH);
  newline->units_len     = wc_units_new(WC_UNITS_LENGTH);
  newline->units_freq    = wc_units_new(WC_UNITS_FREQUENCY);
  newline->units_loss    = wc_units_new(WC_UNITS_DB);
  newline->units_losslen = wc_units_new(WC_UNITS_DB_PER_LEN);
  newline->units_rho     = wc_units_new(WC_UNITS_RESISTIVITY);
  newline->units_rough   = wc_units_new(WC_UNITS_LENGTH);
  newline->units_delay   = wc_units_new(WC_UNITS_TIME);
  newline->units_depth   = wc_units_new(WC_UNITS_LENGTH);
  newline->units_deltal  = wc_units_new(WC_UNITS_LENGTH);

  /* load in the defaults */
  coupled_microstrip_load_string(newline, default_coupled_microstrip);

  /* and do a calculation to finish the initialization */
  coupled_microstrip_calc(newline, newline->freq);

  return(newline);
}

