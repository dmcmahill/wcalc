/* $Id$ */

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


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "ic_microstrip.h"

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

double ic_microstrip_calc(ic_microstrip_line *line, double f)
{
  double z0;

  double omega;
  double w;

  double rho,tmet,rough;
  double tox,eox;
  double h,es,sigmas;

  double mu0, e0;

  double Cox;

  double Csemi, Gsemi, Lsemi, Z0semi;

  /* 
   * total incremental shunt admittance and series impedance for the
   * MIS line 
   */
  complex Ytot;
  complex Ztot;

  /* the notation here matches the Tuncer and Neikirk paper */
  double a,b,k;
  complex Zsemi;
  complex betas;
  complex gammai;
  double Zi;

  /* misc temp variables */
  complex jba, jbb, num, den;


  /* total transmission line variables */
  double Lmis, Rmis, Cmis, Gmis;
  complex Zo_mis;

  complex gamma_mis;
  double alpha_mis, beta_mis;
  double lambda_mis, loss_per_lambda_mis;

  double slowwave;

  /* frequency in rad/sec */
  omega = 2.0*M_PI*f;

  /* permeability and permitivitty of free space (H/m and F/m) */
  mu0 = 4*M_PI*1.0e-7;
  e0  = 1.0/(mu0*LIGHTSPEED*LIGHTSPEED);
  
  /* Metal resistivity relative to copper */
  rho = line->subs->rho;
  /* Metal thickness (mils) */
  tmet = line->subs->tmet;
  /* Metalization roughness */
  rough = line->subs->rough;

  /* Microstrip width */
  w = line->w;

  /* Oxide thickness */
  tox = line->subs->tox;
  /* Oxide relative permittivity */
  eox = line->subs->eox;


  /* Substrate dielectric thickness */
  h = line->subs->h;
  /* Substrate relative permittivity */
  es = line->subs->es;
  /* Substrate conductivity */
  sigmas = line->subs->sigmas;


  /* 
   * Find the oxide capacitance assuming tox << w
   */
  printf("Finding oxide capacitance\n");
  Cox = (eox*e0/tox)*w;
  printf("Cox = %g fF/um\n",Cox*1e9);

  /*
   * Find substrate capacitance using Wheelers microstrip equations
   */
  printf("Finding substrate capacitance\n");
  Lsemi = sqrt(mu0*e0)*Zustrip(h,w,tmet,1.0);
  Z0semi = Zustrip(h,w,tmet,es);
  printf("Calling Z0semi = Zustrip(h=%g,w=%g,tmet=%g,es=%g)\n",h,w,tmet,es);
  printf("Z0semi = %g ohms\n",Z0semi);
  Csemi = Lsemi/(Z0semi*Z0semi);
  printf("Csemi = %g fF/um\n",Csemi*1e15*1e-6);
  printf("Lsemi = %g pH/um\n",Lsemi*1e12*1e-6);

  /* substrate conductance */
  printf("Finding substrate conductance\n");
  Gsemi = (sigmas/(es*e0))*Csemi;
  printf("Gsemi = %g s/um\n",Gsemi*1e-6);


  /* admittance per unit length of the MIS transmission line */
  printf("Finding Ytot\n");
  Ytot = c_div(c_complex(-omega*omega*Csemi*Cox,omega*Cox*Gsemi),
	       c_complex(Gsemi,omega*(Csemi + Cox)));

  printf("Ytot (1/(ohm-cm)) = %g + j%g\n",
	  0.01*REAL(Ytot),0.01*IMAG(Ytot));

  /*
   * Now we need to find series impedance per unit length of the MIS
   * transmission line 
   */

  k = 3*h + w/2;
  a = h*w/(2*k-w);
  b = a + h;
  
  printf("k = %g\n",k);
  printf("a = %g\n",a);
  printf("b = %g\n",b);
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
  Zsemi = c_sqrt(c_div(c_complex(0,omega*mu0),
		       c_complex(sigmas,omega*es*e0)));

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
  Zsemi = c_div(Zsemi,c_complex(0,w));

  /*
   *               ----------------------------------------
   *             /              /                      \
   * betas =    /  j omega mu0 | j omega es e0 + sigmas |
   *          \/                \                      /
   *
   */
  betas = c_sqrt(c_complex(-omega*omega*mu0*es*e0,omega*mu0*sigmas));

  /* j betas a */
  jba = c_mul(c_complex(0.0,1.0),c_rmul(a,betas));

  /* j betas b */
  jbb = c_mul(c_complex(0.0,1.0),c_rmul(b,betas));


  /*
   * Now we need the multiplication factor which depends on the Hankel
   * functions 
   *
   *  H0,2(jbb) H0,1(jba)  -  H0,2(jba) H0,1(jbb)      num
   * ---------------------------------------------  = -----
   *  H0,2(jbb) H1,1(jba)  -  H1,2(jba) H0,1(jbb)      den
   *
   */
  num = c_sub(c_mul(c_hankel0_2(jbb),c_hankel0_1(jba)),
	      c_mul(c_hankel0_2(jba),c_hankel0_1(jbb)));

  den = c_sub(c_mul(c_hankel0_2(jbb),c_hankel1_1(jba)),
	      c_mul(c_hankel1_2(jba),c_hankel0_1(jbb)));

  /* multiply into our expression for Zsemi.  This is the final answer */
  Zsemi = c_mul(Zsemi, c_div(num,den));

  /* find the oxide impedance and propagation constant */
  Zi = sqrt(mu0/(eox*e0))/w;
  gammai = c_complex(0,omega*sqrt(mu0*eox*e0));

  /* combine to find the total MIS incremental series impedance */

  /*
   *            Zsemi + Zi     tanh(gammai tox)
   * Ztot = Zi ---------------------------------
   *            Zi    + Zsemi  tanh(gammai tox)
   */

  num = c_add(Zsemi               , c_rmul(Zi    , c_tanh(c_rmul(tox,gammai))));
  den = c_add(c_complex(Zi,0.0)   , c_mul (Zsemi , c_tanh(c_rmul(tox,gammai))));
  
  Ztot = c_rmul(Zi,c_div(num,den));


  printf("Zi    = %g ohms/um\n",Zi*1e-6);
  printf("Zsemi = %g + i%g\n",REAL(Zsemi),IMAG(Zsemi));

  /* whew! */

  /* 
   * now we have the incremental series impedance and shunt admittance
   * so we can proceed with finding things like the MIS characteristic
   * impedance, velocity, and loss.
   */

  /* elements in an incremental circuit model */
  Rmis = REAL(Ztot);
  Lmis = IMAG(Ztot)/omega;
  Gmis = REAL(Ytot);
  Cmis = IMAG(Ytot)/omega;


  printf("Lmis = %g pH/um\n",Lmis*1e6);
  printf("Cmis = %g fF/um\n",Cmis*1e9);
  printf("Rmis = %g ohm/um\n",Rmis*1e-6);
  printf("Gmis = %g s/um\n",Gmis*1e-6);

  printf("Zmis = %g + i%g ohm/cm\n",REAL(Ztot)*1e-2,IMAG(Ztot)*1e-2);
  printf("Zmis = %g + i%g ohm/um\n",REAL(Ztot)*1e-6,IMAG(Ztot)*1e-6);

  /* characteristic impedance */
  Zo_mis = c_sqrt(c_div(Ztot,Ytot));

  /* propagation constant */
  gamma_mis = c_sqrt(c_mul(Ztot,Ytot));
  alpha_mis = REAL(gamma_mis);
  beta_mis  = IMAG(gamma_mis);
  printf("gamma_mis = %g + i%g\n",alpha_mis,beta_mis);

  lambda_mis = 2.0*M_PI/beta_mis;

  slowwave = beta_mis/(omega*sqrt(mu0*e0));

  printf("omega = %g rad/sec\n",omega);

  /* loss in dB per wavelentgh of transmission line */
  loss_per_lambda_mis = alpha_mis*lambda_mis*8.68;

  printf("Zo (ohms) = %g + j%g\n",REAL(Zo_mis),IMAG(Zo_mis));
  printf("MIS wavelength           = %g mm\n",1e3*lambda_mis);
  printf("Free space wavelength    = %g mm\n",1e3*LIGHTSPEED/f);
  printf("Slow wave factor         = %g\n",slowwave);
  printf("Loss per wavelength (dB) = %g\n ",loss_per_lambda_mis);
  return(z0);
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


int ic_microstrip_syn(ic_microstrip_line *line, double f)
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
  er = line->subs->es;

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
  //  zold = ic_microstrip_calc(line,f,NOLOSS);
  
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
      //      zo = ic_microstrip_calc(line,f,NOLOSS);

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
  //  ic_microstrip_calc(line,f);
  eeff = line->keff;

  v = c / sqrt(eeff);


  l = (len/360)*(v/f);
  lmil = M2MIL(l);


  line->w=w;
  line->l=lmil;

  /* recalculate using real length to find loss  */
  //  ic_microstrip_calc(line,f);
  
#ifdef DEBUG
  printf("synthesis for Z0=%g ohms and len=%g deg\n",line->z0,line->len);
  printf("produced:\n");
  printf("\twidth = %g\n\tlength = %g\n",w,l);
#endif

  return(0);
}


ic_microstrip_line *ic_microstrip_line_new(void)
{
  ic_microstrip_line *newline;

  newline = (ic_microstrip_line *) malloc(sizeof(ic_microstrip_line));
  if(newline == NULL)
    {
      fprintf(stderr,"ic_microstrip_line_new: malloc() failed\n");
      exit(1);
    }

  newline->subs = ic_microstrip_subs_new();

  return(newline);
}

ic_microstrip_subs *ic_microstrip_subs_new(void)
{
  ic_microstrip_subs *newsubs;

  newsubs = (ic_microstrip_subs *) malloc(sizeof(ic_microstrip_subs));
  if(newsubs == NULL)
    {
      fprintf(stderr,"ic_microstrip_subs_new: malloc() failed\n");
      exit(1);
    }

  return(newsubs);
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

  e = exp(1);
  deltaw = (t/M_PI)*
    ((1.0 + (1.0/er))/2.0)*
    log(4*e/sqrt(pow(t/h,2.0) +	 pow((1/M_PI)/(1.1 + w/t),2.0)));

  wp = w + deltaw;

  b = ((14 + 8/er)/11)*(4*h/wp);

  z0 = 42.4/sqrt(1.0 + er);
  z0 = z0*log(1.0 + 
	      (4*h/wp)*(b + sqrt(pow(b,2.0) + 
				 0.5*(1.0 + 1.0/er)*pow(M_PI,2.0))));

  return(z0);
}



#define def

int main(int argc, char **argv)
{
  double h=.062;
  double w=.050;
  double t=.0014;
  double er=4.8;
  complex x,j0,j1,y0,y1;

  ic_microstrip_line *line;
  double freq;

#ifdef def
  line = ic_microstrip_line_new();

  /* Metal resistivity relative to copper */
  line->subs->rho = 1.0;
  /* Metal thickness (m) */
  line->subs->tmet = 5.0e-6;
  /* Metalization roughness */
  line->subs->rough = 1.0e-6;

  /* Microstrip width */
  line->w = 160e-6;

  /* Oxide thickness */
  line->subs->tox = 1.0e-6;
  /* Oxide relative permittivity */
  line->subs->eox = 4.0;


  /* Substrate dielectric thickness */
  line->subs->h = 250e-6;
  /* Substrate relative permittivity */
  line->subs->es = 10.0;
  /* Substrate conductivity s/m*/
  line->subs->sigmas = 100.0;

  freq = 1.0e9;

  if (argc == 9){
    printf("Using external data\n");
    /* Metal resistivity relative to copper */
    line->subs->rho = 1.0;
    /* Metal thickness (m) */
    line->subs->tmet = atof(argv[1]);
    /* Metalization roughness */
    line->subs->rough = 1.0;
    
    /* Microstrip width */
    line->w = atof(argv[2]);
    
    /* Oxide thickness */
    line->subs->tox = atof(argv[3]);
    /* Oxide relative permittivity */
    line->subs->eox = atof(argv[4]);
    
    
    /* Substrate dielectric thickness */
    line->subs->h = atof(argv[5]);
    /* Substrate relative permittivity */
    line->subs->es = atof(argv[6]);
    /* Substrate conductivity s/m*/
    line->subs->sigmas = atof(argv[7]);
    
    freq = atof(argv[8]);
  }


  printf("-------------- IC Microstrip Analysis ----------\n");
  printf("Metal width                 = %g um\n",line->w*1e6);
  printf("Oxide thickness             = %g um\n",line->subs->tox*1e6);
  printf("Oxide dielectric const.     = %g \n",line->subs->eox);
  printf("Substrate thickness         = %g um\n",line->subs->h*1e6);
  printf("Substrate dielectric const. = %g \n",line->subs->es);
  printf("Substrate conductivity      = %g 1/(ohm-cm)\n",line->subs->sigmas*0.01);
  printf("Substrate resistivity       = %g ohm-cm\n",100.0/(line->subs->sigmas));
  printf("Frequency                   = %g GHz\n",freq/1e9); 
  printf("-------------- ---------------------- ----------\n");

  ic_microstrip_calc(line,freq);

#endif

#ifdef notdef
  printf("------------------------------------\n");
  while(w<=0.120){
    printf("z0 (w=%6.4f) = %6.4f\n",w,Zustrip(h,w,t,er));
    w = w + 0.010;
  }
  printf("------------------------------------\n");
#endif

#ifdef notdef
  w=-4.0;
  while (w <= 4.001){
    x = c_complex(1.0,w);
    j0 = c_bessel_J0(x);
    w = w + 0.1;
    printf("J0( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));
  }
#endif

#ifdef notdef
  w=-4.0;
  while (w <= 4.001){
    x = c_complex(0.0,w);
    j1 = c_bessel_J1(x);
    w = w + 0.1;
    printf("J1( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j1),IMAG(j1));
  }
#endif

#ifdef notdef
  w=-4.0;
  while (w <= 4.001){
    x = c_complex(w,0.0);
    j0 = c_bessel_Y0(x);
    w = w + 0.1;
    printf("Y0( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));
  }
#endif

#ifdef notdef
  x = c_complex(0.1,0.2);
  j0 = c_cosh(x);
  printf("cosh( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));

  x = c_complex(0.1,-0.2);
  j0 = c_cosh(x);
  printf("cosh( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));

  x = c_complex(-0.1,-0.2);
  j0 = c_cosh(x);
  printf("cosh( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));

  x = c_complex(-0.1,0.2);
  j0 = c_cosh(x);
  printf("cosh( %8.6f + i %8.6f ) = %8.6f + i %8.6f\n",
	   REAL(x),IMAG(x),REAL(j0),IMAG(j0));
#endif

  return 0;
}


