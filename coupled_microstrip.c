/* $Id: coupled_microstrip.c,v 1.2 2001/09/14 01:59:04 dan Exp $ */

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

//#define DEBUG_SYN  /* debug coupled_microstrip_syn()  */
//#define DEBUG_CALC /* debug coupled_microstrip_calc() */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "coupled_microstrip.h"


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
 *  The equations for effective permittivity and characteristic impedance are
 *  from:
 *  Manfred Kirschning and Rolf Jansen, "Accurate Wide-Range Design Equations for
 *    the Frequency-Dependent Characteristic of Parallel Coupled Microstrip
 *    Lines", IEEE Transactions on Microwave Theory and Techniques, Vol MTT-32,
 *    No 1, January 1984, p83-90.
 *    corrections in MTT-33, No 3, March 1985, p. 288
 *
 *
 *
 */

double coupled_microstrip_calc(coupled_microstrip_line *line, double f)
{

  /* input physical dimensions */
  double wmil,w,lmil,l,s,smil;

  /* substrate parameters */
  double h,hmil,er,rho,tand,t,tmil,rough,roughmil;

  double u,g,deltau,V,AE,BE,EFE0,AU,EF,AO,BO,CO,DO;
  double EFO0,fn;
  double P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15;
  double FEF,FOF,EFEF,EFOF,FU,ZL0;
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

  double v,len,loss,uold,z1,z2,z3,z4,z5,d1,d2;
  double sf;

  /* even/odd mode open end correction lengths */
  double deltale, deltalo;

  double fnold;

  /* for skindepth calculation  */
  double mu,sigma,depth;


#ifdef DEBUG_CALC
  printf("coupled_microstrip_calc(): --------- Coupled_Microstrip Analysis ----------\n");
  printf("coupled_microstrip_calc(): Metal width                 = %g mil\n",line->w);
  printf("coupled_microstrip_calc(): Metal spacing               = %g mil\n",line->s);
  printf("coupled_microstrip_calc(): Metal thickness             = %g mil\n",line->subs->tmet);
  printf("coupled_microstrip_calc(): Metal relative resistivity  = %g \n",line->subs->rho);
  printf("coupled_microstrip_calc(): Metal surface roughness     = %g mil-rms\n",line->subs->rough);
  printf("coupled_microstrip_calc(): Substrate thickness         = %g mil\n",line->subs->h);
  printf("coupled_microstrip_calc(): Substrate dielectric const. = %g \n",line->subs->er);
  printf("coupled_microstrip_calc(): Substrate loss tangent      = %g \n",line->subs->tand);
  printf("coupled_microstrip_calc(): Frequency                   = %g MHz\n",f/1e6); 
  printf("coupled_microstrip_calc(): -------------- ---------------------- ----------\n");
#endif



  wmil = line->w;
  w = MIL2M(wmil);

  lmil = line->l;
  l = MIL2M(lmil);

  smil = line->s;
  s = MIL2M(smil);

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



  /*
   * Start of coupled microstrip calculations
   */

  /* Find u and correction factor for nonzero metal thickness */

  u = w/h;
  g = s/h;

  deltau = (t/(2*M_PI*h));
  //deltau = deltau*log(1 + 4*exp(1)*h/(t*coth(sqrt(6.517*u))^2))*(1 + 1/cosh(sqrt(er-1)));
  //u = u + deltau;


  /*
   * static even mode relative permittivity (f=0)
   */
  V = u*(20.0 + pow(g,2.0))/(10.0 + pow(g,2.0)) + g*exp(-g);
  AE = 1.0 
    + (1.0/49.0)*log((pow(V,4.0) + pow((V/52.0),2.0))/(pow(V,4.0) + 0.432)) 
    + (1.0/18.7)*log(1.0 + pow((V/18.1),3.0));
  BE = 0.564*pow(((er-0.9)/(er+3.0)),0.053);
  EFE0 = (er+1.0)/2.0 + ((er-1.0)/2.0)*pow((1.0 + 10.0/V),(-AE*BE));


  /*
   * static single strip, T=0, relative permittivity (f=0)
   */
  AU = 1.0 
    + (1.0/49.0)*log((pow(u,4.0) + pow((u/52.0),2.0))/(pow(u,4.0) + 0.432)) 
    + (1.0/18.7)*log(1.0 + pow((u/18.1),3.0));
  EF = (er+1.0)/2.0 + ((er-1.0)/2.0)*pow((1.0 + 10.0/u),(-AU*BE));



  /*
   * static odd mode relative permittivity (f=0)
   */
  AO = 0.7278*(EF - (er+1.0)/2.0)*(1.0 - exp(-0.179*u));
  BO = 0.747*er/(0.15 + er);
  CO = BO - (BO - 0.207) * exp(-0.414*u);
  DO = 0.593 + 0.694*exp(-0.562*u);
  EFO0 = ((er+1.0)/2.0 + AO - EF)*exp(-CO*(pow(g,DO))) + EF;


  /* normalized frequency */
  fn = 1e-6 * f * h;


  /*
   * even/odd mode relative permittivity including dispersion
   */
  P1 = 0.27488+ (0.6315 + 0.525/(pow((1.0 + 0.0157*fn),20.0)))*u - 0.065683*exp(-8.7513*u);
  P2 = 0.33622*(1.0 - exp(-0.03442*er));
  P3 = 0.0363*exp(-4.6*u)*(1 - exp(-pow((fn/38.7),4.97)));
  P4 = 1.0 + 2.751*(1.0 - exp(-pow((er/15.916),8.0)));
  P5 = 0.334*exp(-3.3*pow((er/15.0),3.0)) + 0.746;
  P6 = P5*exp(-pow((fn/18),0.368));
  P7 = 1.0 + 4.069*P6*(pow(g,0.479))*exp(-1.347*(pow(g,0.595)) - 0.17*(pow(g,2.5)));
  P8 = 0.7168*(1.0 + 1.076/(1.0 + 0.0576*(er-1.0)));
  P9 = P8 - 0.7913*(1.0 - exp(-pow((fn/20.0),1.424)))*atan(2.481*pow((er/8.0),0.946));
  P10 = 0.242*pow((er-1.0),0.55);
  P11 = 0.6366*(exp(-0.3401*fn) - 1.0)*atan(1.263*pow((u/3),1.629));
  P12 = P9 + (1.0 - P9)/(1.0 + 1.183*pow(u,1.376));
  P13 = 1.695*P10/(0.414 + 1.605*P10);
  P14 = 0.8928 + 0.1072*(1.0 - exp(-0.42*pow((fn/20),3.215)));
  P15 = fabs(1.0 - 0.8928*(1.0 + P11)*P12*exp(-P13*(pow(g,1.092)))/P14);

  FEF = P1*P2*pow(((P3*P4 + 0.1844*P7)*fn),1.5763);
  FOF = P1*P2*pow(((P3*P4 + 0.1844)*fn*P15),1.5763);
  
  /* relative permittivities: */
  EFEF = er - (er - EFE0)/(1.0 + FEF);
  EFOF = er - (er - EFO0)/(1.0 + FOF);


  /*
   * static single strip, T=0, characteristic impedance (f=0)
   *  (Hammerstad and Jensen)
   */
  FU = 6.0 + (2.0*M_PI - 6.0)*exp(-pow((30.666/u),0.7528));
  ZL0 = (60.0/sqrt(EF)) * log(FU/u + sqrt(1.0 + pow((2.0/u),2.0)));


  /*
   * static even mode characteristic impedance (f=0)
   */
  Q1 = 0.8695*(pow(u,0.194));
  Q2 = 1.0 + 0.7519*g + 0.189*(pow(g,2.31));
  Q3 = 0.1975 + pow((16.6 + pow((8.4/g),6)),-0.387) 
    + log((pow(g,10.0))/(1.0 + pow((g/3.4),10.0)))/241.0;
  Q4 = (.02*Q1/Q2)/(exp(-g)*(pow(u,Q3)) + (2.0 - exp(-g))*(pow(u,-Q3)));
  z0e0 = ZL0 * sqrt(EF/EFE0) / (1.0 - (ZL0/377)*sqrt(EF)*Q4);


  /*
   * static odd mode characteristic impedance (f=0)
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




  /*
   * relative permittivity including dispersion
   * of single microstrip of width W, Tmet=0
   *  (Kirschning and Jansen)
   */

  /* normalized frequency (GHz-cm) */
  /* save fn */
  fnold = fn;

  fn = 1e-7 * f * h;

  xP1 = 0.27488 + (0.6315 + (0.525 / (pow((1.0 + 0.157*fn),20.0))) )*u 
    - 0.065683*exp(-8.7513*u);
  xP2 = 0.33622*(1.0 - exp(-0.03442*er));
  xP3 = 0.0363*exp(-4.6*u)*(1 - exp(-pow((fn/3.87),4.97)));
  xP4 = 1.0 + 2.751*( 1.0 -  exp(-pow((er/15.916),8.0)));
  xP = xP1*xP2*pow(((0.1844 + xP3*xP4)*10*fn),1.5763);

  EFF = (EF + er*xP)/(1.0 + xP);

  /* recall fn */
  fn=fnold;



  /*
   * Characteristic Impedance of single strip, Width=W, Tmet=0
   *  (Jansen and Kirschning)
   */

  /* normalized frequency (GHz-mm)*/
  /* save fn */
  fnold = fn;
  fn = 1e-6 * f * h;

  R1 = 0.03891*(pow(er,1.4));
  R2 = 0.267*(pow(u,7.0));
  R3 = 4.766*exp(-3.228*(pow(u,0.641)));
  R4 = 0.016 + pow((0.0514*er),4.524);
  R5 = pow((fn/28.843),12.0);
  R6 = 22.20*(pow(u,1.92));
  R7 = 1.206 - 0.3144*exp(-R1)*(1.0 - exp(-R2));
  R8 = 1.0 + 1.275*(1.0 - exp(-0.004625*R3*(pow(er,1.674))));
  R9 = (5.086*R4*R5/(0.3838 + 0.386*R4))*(exp(-R6)/(1.0 + 1.2992*R5));
  R9 = R9 * (pow((er-1.0),6.0))/(1.0 + 10*pow((er-1.0),6.0));
  R10 = 0.00044*(pow(er,2.136)) + 0.0184;
  R11 = (pow((fn/19.47),6.0))/(1.0 + 0.0962*(pow((fn/19.47),6.0)));
  R12 = 1.0 / (1.0 + 0.00245*u*u);
  R13 = 0.9408*(pow(EFF,R8)) - 0.9603;
  R14 = (0.9408 - R9)*pow(EF,R8)-0.9603;
  R15 = 0.707*R10*(pow((fn/12.3),1.097));
  R16 = 1.0 + 0.0503*er*er*R11*(1.0 - exp(-(pow((u/15),6.0))));
  R17 = R7*(1.0 - 1.1241*(R12/R16)*exp(-0.026*(pow(fn,1.15656))-R15));

  /* ZLF = zero thickness single strip characteristic impedance including dispersion */
  ZLF = (60.0/sqrt(EFF)) * log(FU/u + sqrt(1.0 + pow((2.0/u),2.0)))*(pow((R13/R14),R17));

  /* Q0 = R17 from zero thickness, single microstrip */
  Q0 = R17;

  /* recall fn */
  fn=fnold;


  /*
   * even mode characteristic impedance including dispersion
   */
  Q11 = 0.893*(1.0 - 0.3/(1.0 + 0.7*(er-1.0)));
  Q12 = 2.121*((pow((fn/20.0),4.91))
	       /(1 + Q11*pow((fn/20.0),4.91)))*exp(-2.87*g)*pow(g,0.902);
  Q13 = 1.0 + 0.038*pow((er/8),5.1);
  Q14 = 1.0 + 1.203*(pow((er/15.0),4.0))/(1.0 + pow((er/15.0),4.0));
  Q15 = 1.887*exp(-1.5*(pow(g,0.84)))*(pow(g,Q14)) 
    /(1.0 + 0.41*(pow((fn/15.0),3.0))*(pow(u,(2/Q13)))/(0.125 + pow(u,(1.626/Q13))));
  Q16 = (1.0 + 9.0/(1.0 + 0.403*pow((er-1.0),2.0)))*Q15;
  Q17 = 0.394*(1.0 - exp(-1.47*pow((u/7.0),0.672)))*(1.0 - exp(-4.25*pow((fn/20.0),1.87)));
  Q18 = 0.61*(1.0 - exp(-2.13*pow((u/8),1.593)))/(1.0 + 6.544*pow(g,4.17));
  Q19 = 0.21*(pow(g,4.0))/((1.0 + 0.18*(pow(g,4.9)))*(1.0 + 0.1*u*u)*(1 + pow((fn/24),3.0)));
  Q20 = (0.09 + 1.0/(1.0 + 0.1*pow((er-1.0),2.7)))*Q19;
  Q21 = fabs(1.0 - 42.54*pow(g,0.133)*exp(-0.812*g)*pow(u,2.5)/(1.0 + 0.033*pow(u,2.5)));
  
  RE = pow((fn/28.843),12.0);
  QE = 0.016 + pow((0.0514*er*Q21),4.524);
  PE = 4.766*exp(-3.228*pow(u,0.641));
  DE = 5.086*QE*(RE/(0.3838 + 0.386*QE))
    *(exp(-22.2*pow(u,1.92))/(1 + 1.2992*RE))
    *((pow((er-1.0),6.0))/(1.0 + 10.0*pow((er-1.0),6.0)));
  CE = 1.0+1.275*(1.0 - exp(-0.004625*PE*pow(er,1.674)*pow((fn/18.365),2.745)))
    - Q12 + Q16 - Q17 + Q18 + Q20;

  z0ef = z0e0 
    * (pow((0.9408*pow(EFF,CE) - 0.9603),Q0))
    /(pow(((0.9408 - DE)*pow(EF,CE) - 0.9603),Q0));


  /*
   * odd mode characteristic impedance including dispersion
   */
  Q29 = 15.16/(1.0 + 0.196*pow((er-1.0),2.0));
  Q28 = 0.149*(pow((er-1.0),3.0))/(94.5 + 0.038*pow((er-1.0),3.0));
  Q27 = 0.4*pow(g,0.84)
    *(1.0 + 2.5*(pow((er-1.0),1.5))/(5 + pow((er-1.0),1.5)));
  Q26 = 30.0 
    - 22.2*(( pow(((er-1)/13.0),12.0)) 
	    / (1.0 + 3.0*pow(((er-1.0)/13.0),12.0))) 
    - Q29;
  Q25 = (0.3*fn*fn/(10 + fn*fn))
    *(1.0 + 2.333*(pow((er-1.0),2.0))/(5.0 + pow((er-1.0),2.0)));
  Q24 = 2.506*Q28*pow(u,0.894)
    *(pow(((1.0 + 1.3*u)*fn/99.25),4.29))/(3.575 + pow(u,0.894));
  Q23 = 1.0 + 0.005*fn*Q27/((1.0 + 0.812*pow((fn/15),1.9))/(1.0 + 0.025*u*u));
  Q22 = 0.925*(pow((fn/Q26),1.536))/(1.0 + 0.3*pow((fn/30.0),1.536));
  
  z0of = ZLF + (z0o0*pow((EFOF/EFO0),Q22) - ZLF*Q23)/(1.0 + Q24 + (pow((0.46*g),2.2))*Q25);


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
   * Loss
   */
  /* XXX need loss equations */
  loss=0.0;

  /* calculate skin depth */
   
  /* conductivity */
  sigma = 5.8e7 /rho;
   
  /* permeability of free space */
  mu = 4.0*M_PI*1e-7;
   
  /* skin depth in meters */
  depth = sqrt(1.0/(M_PI*f*mu*sigma));
   
  /*
   * Single Line End correction
   *  (Kirschning, Jansen, and Koster)
   */

  /* deltal(2u,er) (per Kirschning and Jansen notation) */
  uold = u;
  u = 2*u;
  z1 = 0.434907*((pow(EF,0.81) + 0.26)/(pow(EF,0.81) - 0.189))
    *(pow(u,0.8544) + 0.236)/(pow(u,0.8544) + 0.87);
  z2 = 1.0 + pow(u,0.371)/(2.358*er + 1.0);
  z3 = 1.0 + (0.5274*atan(0.084*(pow(u,(1.9413 / z2)))))/pow(EF,0.9236);
  z4 = 1.0 + 0.0377*atan(0.067*pow(u,1.456))*(6.0 - 5.0*exp(0.036*(1.0-er)));
  z5 = 1.0 - 0.218*exp(-7.5*u);
  
  d2 = h * z1 * z3 * z5 / z4;

  /* deltal(u,er) (per Kirschning and Jansen notation) */
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
   */
  R1 = 1.187*(1.0-exp(-0.069*pow(u,2.1)));
  R2 = 0.343*pow(u,0.6187) + (0.45*er/(1.0+er))*(pow(u,(1.357 + 1.65/(1.0 + 0.7*er))));
  R3 = 0.2974*(1.0-exp(-R2));
  R4 = (0.271 + 0.0281*er)*(pow(g,(1.167*er/(0.66+er))))
    + (1.025*er/(0.687+er))*(pow(g,(0.958*er/(0.706+er))));

  /* make it print in mils for now. */
  sf=1/25.4e-6;
  deltale =sf*( (d2 - d1 + 0.0198*h*pow(g,R1)) );
  deltalo =sf*( (d1 - h*R3)*(1.0 - exp(-R4)) + h*R3 );

  /*  [z0e,z0o,len,loss,kev,kodd]=cmlicalc(w,l,s,f,subs) */
  /* copy over the results */

  line->z0e = z0ef;
  line->z0o = z0of;
  line->z0  = sqrt(z0ef*z0of);

  line->kev = EFEF;
  line->kodd = EFOF;

  line->deltale = deltale;
  line->deltalo = deltale;
  
  /* electrical length */
  line->len     = len;

  /* loss, loss per unit length */
  line->loss      = loss;
  line->losslen   = loss/line->l;

  /* skin depth in mils */
  line->skindepth =  M2MIL(depth);

  return(line->z0);
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

  double h,er,l,lmil,wmin,wmax,abstol,reltol;
  int maxiters;
  double z0,w;
  int iters;
  int done;
  double v,len;

  double s,smin,smax,z0e,z0o,k;
  double loss,kev,kodd,keff,delta,cval,err,d;

  double AW,F1,F2,F3;

  double ai[] = {1, -0.301, 3.209, -27.282, 56.609, -37.746};
  double bi[] = {0.020, -0.623, 17.192, -68.946, 104.740, -16.148};
  double ci[] = {0.002, -0.347, 7.171, -36.910, 76.132, -51.616};

  int i;
  double ze0,ze1,ze2,dedw,deds;
  double zo0,zo1,zo2,dodw,dods;

#ifdef DEBUG_SYN
  printf("coupled_microstrip_syn(): -------- Coupled_Microstrip Synthesis ----------\n");
  printf("coupled_microstrip_syn(): Metal width                 = %g mil\n",line->w);
  printf("coupled_microstrip_syn(): Metal spacing               = %g mil\n",line->s);
  printf("coupled_microstrip_syn(): Metal thickness             = %g mil\n",line->subs->tmet);
  printf("coupled_microstrip_syn(): Metal relative resistivity  = %g \n",line->subs->rho);
  printf("coupled_microstrip_syn(): Metal surface roughness     = %g mil-rms\n",line->subs->rough);
  printf("coupled_microstrip_syn(): Substrate thickness         = %g mil\n",line->subs->h);
  printf("coupled_microstrip_syn(): Substrate dielectric const. = %g \n",line->subs->er);
  printf("coupled_microstrip_syn(): Substrate loss tangent      = %g \n",line->subs->tand);
  printf("coupled_microstrip_syn(): Frequency                   = %g MHz\n",f/1e6); 
  printf("coupled_microstrip_syn(): -------------- ---------------------- ----------\n");
  printf("coupled_microstrip_syn(): Desired Zo                  = %g ohm\n",line->z0);
  printf("coupled_microstrip_syn(): Desired electrical length   = %g degrees\n",line->len);
  printf("coupled_microstrip_syn(): -------------- ---------------------- ----------\n");
#endif

  z0 = line->z0;
  len = line->len;

  /* Substrate dielectric thickness (mils) */
  h = line->subs->h;
  /* Substrate relative permittivity */
  er = line->subs->er;

  /* desired impedances */
  z0e = line->z0e;
  z0o = line->z0o;

  /* temp value for l used while finding w and s */
  l = 1000.0;
  line->l=l;


  /* limits on the allowed range for w */
  wmin = 0.5;
  wmax = 1000;

  /* limits on the allowed range for s */
  smin = 0.5;
  smax = 1000;


  /* impedance convergence tolerance (ohms) */
  abstol = 1e-6;

  /* width relative convergence tolerance (mils) (set to 0.1 micron) */
  reltol = MICRON2MIL(0.1);

  maxiters = 50;


  /*
   * Initial guess at a solution
   */
  z0 = sqrt(z0e*z0o);
  k = (z0e - z0o)/(z0e + z0o);
  AW = exp(z0*sqrt(er+1.0)/42.4) - 1.0;
  F1 = 8.0*sqrt(AW*(7.0 + 4.0/er)/11.0 + (1.0 + 1.0/er)/0.81)/AW;

  F2 = 0;
  for (i=0; i<=5 ; i++)
    {
      F2 = F2 + ai[i] * pow(k,(i-1.0));
    }

  F3 = 0;
  for (i=0 ; i<=5 ; i++)
    {
      F3 = F3 + (bi[i] - ci[i]*(9.6 - er))*pow((0.6 - k),(i-1));
    }

  w = h*fabs(F1*F2);
  s = h*fabs(F1*F3);


  l=100;
  loss=0;
  kev=1;
  kodd=1;


  iters = 0;
  done = 0;
  delta = 1e-5;
  
  cval = 1e-12*z0e*z0o;

  while(!done)
    {
      iters++;
   
      /*[ze0,zo0,ltmp,loss,kev,kodd]=cmlicalc(w,l,s,f,subs); */
   
#ifdef DEBUG_SYN
      printf("ze = %g\tzo = %g\tw = %g\ts = %g\n",ze0,zo0,w,s);
#endif
   
      /* check for convergence */
      err = pow((ze0-z0e),2.0) + pow((zo0-z0o),2.0);
      if(err < cval)
	{
	  done = 1;
	}
      else
	{
	  /* approximate the first jacobian */
	  //[ze1,zo1,ltmp,loss,kev,kodd]=cmlicalc(w+delta,l,s,f,subs);
	  //[ze2,zo2,ltmp,loss,kev,kodd]=cmlicalc(w,l,s+delta,f,subs);
	  dedw = (ze1 - ze0)/delta;
	  dodw = (zo1 - zo0)/delta;
	  deds = (ze2 - ze0)/delta;
	  dods = (zo2 - zo0)/delta;
	  
	  /* find the determinate */
	  d = dedw*dods - deds*dodw;
	  
	  w = fabs(w - ((ze0-z0e)*dods - (zo0-z0o)*deds)/d);
	  s = fabs(s + ((ze0-z0e)*dodw - (zo0-z0o)*dedw)/d);
#ifdef DEBUG_SYN
	  printf("dedw = %g\tdodw = %g\tdeds = %g\tdods = %g\n",dedw,dodw,deds,dods);
#endif
	}
    }


  keff = sqrt(kev*kodd);
  v = LIGHTSPEED / sqrt(keff);
  l = (len/360)*(v/f);
  lmil = M2MIL(l);

#ifdef DEBUG_SYN
  printf("Took %d iterations, err = %g\n",iters,err);
  printf("ze = %g\tzo = %g\tz0e = %g\tz0o = %g\n",ze0,zo0,z0e,z0o);
#endif

  return(0);
}




void coupled_microstrip_line_free(coupled_microstrip_line * line)
{
  free(line->subs);
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

  return(newline);
}



