/*      $Id: stripline.c,v 1.1 2001/02/11 19:26:27 dan Exp $ */

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


//#define DEBUG_SYN  /* debug stripline_syn()  */
//#define DEBUG_CALC /* debug stripline_calc() */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "stripline.h"


/* flags for stripline_calc_int() */
#define NOLOSS   0
#define WITHLOSS 1

static double stripline_calc_int(stripline_line *line, double f, int flag);


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

double stripline_calc(stripline_line *line, double f)
{
  double z0;
#ifdef DEBUG_CALC
  printf("stripline_calc():  just called\n");
#endif
  z0=stripline_calc_int(line, f, WITHLOSS);
  return(z0);
}

static double stripline_calc_int(stripline_line *line, double f, int flag)
{
  /* physical width/length of line */
  double wmil,w,lmil,l;

  /* substrate parameters */
  double h,hmil,er,rho,tand,t,tmil,rough,roughmil;

  /* calculation variables */
  double k,kp,r,kf,z0,m,deltaW,A,v,len,loss;

  /* loss variables*/
  double z1,z2,lc,ld,delta,depth,Res;
  double mu,sigma;

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
#ifdef DEBUG_CALC
       printf("stripline_calc_int(): Thin stripline analysis\n");
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
#ifdef DEBUG_CALC
       printf("stripline_calc_int(): Finite stripline analysis\n");
#endif

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
   if(flag == WITHLOSS)
     {
#ifdef DEBUG_CALC
       printf("stripline_calc_int():  starting loss calculations\n");
#endif
       /* length in wavelengths */
       if(f > 0.0)
	 len= (l)/(v/f);
       else
	 len = 0.0;
       
       /* convert to degrees */
       len = 360.0*len;
       line->len = len;

       /* calculate loss */

       /*
	* Dielectric Losses
	*/
   
       /* loss in nepers/meter */
       // XXX fixme
       //ld=(M_PI*f/v)*(er/EF)*((EF-1.0)/(er-1.0))*tand;
       ld=0;

       /* loss in dB/meter */
       ld = 20.0*log10(exp(1.0)) * ld;
   
       /* loss in dB */
       ld = ld * l;
   
       /*
	* Conduction Losses
	*/
   
   
       /* calculate skin depth */
   
       /* conductivity (rho is relative to copper) */
       sigma = 5.8e7 / rho;
   
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
	   // XXX fixme
	   //z2=microstrip_calc_int(line,f,NOLOSS);
	   z2 = 0;

	   line->subs->h = hmil + delta;
	   line->subs->tmet = tmil - delta;
	   line->w = wmil-delta;
	   // XXX fixme
	   //z1=microstrip_calc_int(line,f,NOLOSS);
	   z1 = 0;

	   line->subs->er = er;
	   line->subs->h = hmil;
	   line->subs->tmet = tmil;
	   line->w = wmil;

	   /* conduction losses, nepers per meter */
	   lc = (M_PI*f/LIGHTSPEED)*(z1 - z2)/z0;
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): HF conduction loss, z1=%g,z2=%g,z0=%g,lc=%g\n",
		  z1,z2,z0,lc);
#endif
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
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): LF conduction loss, Res=%g, lc=%g\n",
		  Res,lc);
#endif
	   /* no conduction loss case */
	 }
       else
	 {
	   lc=0.0;
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): 0 thickness strip.  setting loss=0\n");
#endif
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

  /* XXX - need to get loss equations for stripline */
  loss = 0;

  /* find the incremental circuit model */
  line->Ls = 0.0;
  line->Rs = 0.0;
  line->Cs = 0.0;
  line->Gs = 0.0;

  line->Ro = z0;
  line->Xo = 0.0;

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

#ifdef USE_OLD_SYN
int stripline_syn(stripline_line *line, double f, int flag)
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
#ifdef DEBUG_SYN
      printf("stripline_syn(): Took %d iters\n",i);
#endif

      k = sqrt(q)*(pow((N/D),2.0));
      w = (2.0*h/M_PI)*log(1.0/k + sqrt(pow((1.0/k),2.0) - 1.0));
    }
  else
    {
      /*
       * Finite strip case: 
       */
#ifdef DEBUG_SYN
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

#else /* !USE_OLD_SYN */

int stripline_syn(stripline_line *line, double f, int flag)
{

  double l;
  double Ro, Xo;
  double v,len;

  /* the parameters which define the structure */
  double w;
  double tmet;
  double h,es,tand;

  /* permeability and permitivity of free space */
  double mu0, e0;


  /* the optimization variables, current, min/max, and previous values */
  double var, varmax, varmin, varold;
  
  /* errors due to the above values for the optimization variable */
  double err, errmax, errmin, errold;

  /* derivative */
  double deriv;

  /* the sign of the slope of the function being optimized */
  double sign;

  /* pointer to which parameter of the line is being optimized */
  double *optpar;

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

  switch(flag){
  case SLISYN_W:
    optpar = &(line->w);
    varmax = 1000.0;
    varmin = 1.0;
    var    = 50.0;
    break;

  case SLISYN_H:
    optpar = &(line->subs->h);
    varmax = 1000.0;
    varmin = 1.0;
    var    = 62.0;
    break;

  case SLISYN_ES:
    optpar = &(line->subs->er);
    varmax = 100.0;
    varmin = 1.0;
    var    = 5.0;
    break;

  case SLISYN_L:
    optpar = &(line->l);
    varmax = 100.0;
    varmin = 1.0;
    var    = 5.0;
    done = 1;
    break;

  default:
    fprintf(stderr,"stripline_synth():  illegal flag=%d\n",flag);
    exit(1);
    break;
  }

  /*
   * read values from the input line structure
   */

  Ro = line->Ro;
  Xo = line->Xo;
  len = line->len;

  /* Metal width, length, and thickness */
  w = line->w;
  l = line->l;
  tmet = line->subs->tmet;

  /* Substrate thickness, relative permitivity, and loss tangent */
  h = line->subs->h;
  es = line->subs->er;
  tand = line->subs->tand;


  /*
   * temp value for l used while synthesizing the other parameters.
   * We'll correct l later.
   */

  l = 1000.0;
  line->l=l;


#ifdef DEBUG_SYN
  printf("stripline_syn(): --------------- Stripline Synthesis -----------\n");
  printf("stripline_syn(): Metal width                 = %g mil\n",line->w);
  printf("stripline_syn(): Metal thickness             = %g mil\n",line->subs->tmet);
  printf("stripline_syn(): Metal relative resistivity  = %g \n",line->subs->rho);
  printf("stripline_syn(): Metal surface roughness     = %g mil-rms\n",line->subs->rough);
  printf("stripline_syn(): Substrate thickness         = %g mil\n",line->subs->h);
  printf("stripline_syn(): Substrate dielectric const. = %g \n",line->subs->er);
  printf("stripline_syn(): Substrate loss tangent      = %g \n",line->subs->tand);
  printf("stripline_syn(): Frequency                   = %g MHz\n",f/1e6); 
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
  printf("stripline_syn(): Desired Zo                  = %g ohm\n",Ro);
  printf("stripline_syn(): Desired electrical length   = %g degrees\n",len);
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
  printf("stripline_syn(): Starting optimization value = %g\n",var);
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
#endif


  if (!done){
    /* Initialize the various error values */
    *optpar = varmin;
    errmin = stripline_calc_int(line,f,NOLOSS) - Ro;

    *optpar = varmax;
    errmax = stripline_calc_int(line,f,NOLOSS) - Ro;

    *optpar = var;
    err = stripline_calc_int(line,f,NOLOSS) - Ro;

    varold = 0.99*var;
    *optpar = varold;
    errold = stripline_calc_int(line,f,NOLOSS) - Ro;


    /* see if we've actually been able to bracket the solution */
    if (errmax*errmin > 0){
      fprintf(stderr,
	      "stripline_syn():  could not bracket the solution\n");
      exit(1);
    }
  
    /* figure out the slope of the error vs variable */
    if (errmax > 0)
      sign =  1.0;
    else
      sign = -1.0;

    iters = 0;
  }

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
      printf("stripline_syn():  Taking a bisection step\n");
#endif
      var = (varmin + varmax)/2.0;
    }

    /* update the error value */
    *optpar = var;
    err = stripline_calc_int(line,f,NOLOSS) - Ro;
    
      
    /* update our bracket of the solution. */

    if (sign*err > 0)
      varmax = var;
    else
      varmin = var;

 
    /* check to see if we've converged */
    if (fabs(err) < abstol){
      done = 1;
#ifdef DEBUG_SYN
      printf("stripline_syn():  abstol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if ( fabs((var-varold)/var) < reltol){
      done = 1;
#ifdef DEBUG_SYN
      printf("stripline_syn():  reltol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if (iters >= maxiters){
      fprintf(stderr,"stripline_syn(): failed to converge in %d iterations\n",
	      maxiters);
      exit(1);
    }
    

#ifdef DEBUG_SYN
      printf("stripline_syn(): iteration #%d:  var = %g\terr = %g\n",iters,var,err);
#endif
      /* done with iteration */
  }

  /* velocity on line */
  stripline_calc(line,f);

  v = LIGHTSPEED / sqrt(line->subs->er);

  l = (len/360)*(v/f);

  line->l=M2MIL(l);

  /* recalculate using real length to find loss  */
  stripline_calc(line,f);
  
#ifdef DEBUG_SYN
  printf("synthesis for Z0=%g [ohms] and len=%g [deg]\n",line->z0,line->len);
  printf("produced:\n");
  printf("\twidth = %g [mil] \n\tlength = %g [mil]\n",line->w,line->l);
#endif

  return(0);
}

#endif /* USE_OLD_SYN */


void stripline_line_free(stripline_line * line)
{
  free(line->subs);
  free(line);
}


stripline_line *stripline_line_new()
{
  stripline_line *newline;

  newline = (stripline_line *) malloc(sizeof(stripline_line));
  if(newline == NULL)
    {
      fprintf(stderr,"stripline_line_new: malloc() failed\n");
      exit(1);
    }

  newline->subs = stripline_subs_new();

  return(newline);
}


stripline_subs *stripline_subs_new(void)
{
  stripline_subs *newsubs;

  newsubs = (stripline_subs *) malloc(sizeof(stripline_subs));
  if(newsubs == NULL)
    {
      fprintf(stderr,"stripline_subs_new: malloc() failed\n");
      exit(1);
    }

  return(newsubs);
}

