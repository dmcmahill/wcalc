
/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/*
 *   References:
 *
 * Robert Colin, "Foundations for Microwave Engineering" McGraw-Hill,
 * Inc. 1966, page 81.
 *
 * John Karahask, "Transmission Lines and Filter Networks", MacMillan, 
 * 1950, New York
 *
 * Stanislaw Rosloniec, "Algorithms For Computer-Aided Design of
 * Linear Microwave Circuits" Artech House, 1990, page 184.
 *
 * H. A. Wheeler, "Formulas for the skin effect", Proc. IRE,
 * Vol. 30, No. 9, September 1942, pp. 412-4124
 *
 * Ruel V. Churchill and James Ward Brown, "Complex Variables and
 * Applications, 4th ed", McGraw-Hill, Inc., 1984, New York.  This
 * book includes an appendix with a catalog of conformal
 * transformations of regions.  A transform which is most useful for
 * coax lines with offset center conductor is found here.
 *
 * Dan McMahill, 2001-11-26:
 * -------------------------
 * The expressions for shield and center conductor resistance as a
 * function of frequency have been modified by me to correctly compute
 * the low frequency (DC) limiting resistance and also provide the
 * appropriate transition between the DC region and the skindepth
 * limited region.
 *
 */

/* debug the coax_calc() function */
/* #define DEBUG_CALC */
/* debug the coax_syn() function  */
/* #define DEBUG_SYN */

#include "config.h"

/* for DBL_EPSILON */
#include <float.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "alert.h"
#include "coax.h"
#include "coax_loadsave.h"
#include "defaults.h"
#include "mathutil.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * finds the error in the TE10 boundary condition.  Error should be 0.
 * Note that this assumes that c == 0 (center conductor is not offset
 * with respect to the shield.
 */
static double coax_TE10_err(coax_line *line, double k)
{
  double err;
  
  err = bessel_J1p(k*line->a)*bessel_Y1p(k*line->b) -
    bessel_J1p(k*line->b)*bessel_Y1p(k*line->a);

  return err;
}

static int coax_calc_int(coax_line *line, double freq, int flag);

/* coax_calc_int flags */
#define CALC_ALL  0  /* all calculation (calculate everything) */ 
#define CALC_MIN  1  /* minimum calculation (skip some)        */ 


int coax_calc(coax_line *line, double freq)
{
  int rslt;

#ifdef DEBUG_CALC
  printf("********************************************************\n");
  printf("* Starting top level coax_calc()                       *\n");
  printf("********************************************************\n");
#endif

  rslt=coax_calc_int(line,freq,CALC_ALL);
#ifdef DEBUG_CALC
  printf("coax_calc():  returning %d\n",rslt);
#endif
  return rslt;
}

static int coax_calc_int(coax_line *line, double freq, int flag)
{
  double x;
  double mu0,e0;
  double v;
  double Gc0, Gs0, Rc0, Rs0;
  double Rc, Rs;
  double Rc_delta, Rs_delta;
  double RcHF, RsHF;
  double delta_c=1.0;
  double delta_s=1.0;
  double omega;
  double db_per_np;

  /* for TE10 cutoff calculation */
  double k,kold;
  double err,errold;
  double ddk;
  int i;

  /* for incremental inductance calculation */
  coax_line tmp_line;
  double z1, z2, lc;
  int rslt;

#ifdef DEBUG_CALC
  double x1, x2, Ro;
#endif

#ifdef DEBUG_CALC
  printf("\n");
  printf("coax_calc_int():  ----------------------\n");
  printf("coax_calc_int():  Input values:\n");
  printf("coax_calc_int():  ----------------------\n");
  printf("coax_calc_int():  a     = %g meters\n",line->a);
  printf("coax_calc_int():  b     = %g meters\n",line->b);
  printf("coax_calc_int():  c     = %g meters\n",line->c);
  printf("coax_calc_int():  t     = %g meters\n",line->tshield);
  printf("coax_calc_int():  rho_a = %g ohm-meter\n",line->rho_a);
  printf("coax_calc_int():  rho_b = %g ohm-meter\n",line->rho_b);
  printf("coax_calc_int():  er    = %g \n",line->er);
  printf("coax_calc_int():  tand  = %g \n",line->tand);
  printf("coax_calc_int():  len   = %g meters\n",line->len);
  printf("coax_calc_int():  freq = %g MHz\n",line->freq*1e-6);
  printf("coax_calc_int():  ----------------------\n");
#endif


  /* qualify the inputs some */

  if (line->b <= line->a ) {
    alert("Error: b (%g) must be > a (%g)\r\n"
	  "for a coax line\r\n");
    return -1;
  }

  if (line->c >= line->b - line->a) {
    alert("Error:  c (%g)  must be < b - a (%g)\r\n"
	  "for a coax line\r\n",line->c,line->b-line->a);
    return -1;
  }

  /*
   * find characteristic impedance (from Rosloniec)
   *
   * Note that Rosloniec deals in diameters while I deal
   * in radii.  Also, Rosloniec swaps a and b from the notation I'm
   * using here.
   * x = (b + [a^2 - 4c^2]/b)/(2a) is on p. 184 of Rosloniec.
   *
   * substituting b = 2a, a=2b gives
   *
   * x = (2a + [4b^2 - 4c^2]/2a)/(4b)
   *   = ( a + [2b^2 - 2c^2]/2a)/(2b)
   *   = ( a + [ b^2 -  c^2]/ a)/(2b)
   */
  x = (line->a + ( pow(line->b,2.0) - pow(line->c,2.0) )/line->a) / 
    (2*line->b);


  mu0 = 4*M_PI*1.0e-7;
  e0 = 1.0/(mu0*LIGHTSPEED*LIGHTSPEED);

  line->z0 = (1/(2*M_PI))*sqrt(mu0/(e0*line->er))*log(x + sqrt(x*x - 1));

#ifdef DEBUG_CALC
  /* 
   * This calculation directly implements the conformal mapping of
   * offset circles found in fig 14, p 328 of Churchill and Brown.
   * This is a sanity check on the equation from Rosloniec
   */
  x1 = (line->a + line->c)/line->b;
  x2 = (-line->a + line->c)/line->b;
  Ro = (1 - x1*x2 + sqrt((1 - x1*x1)*(1 - x2*x2)))/(x1-x2);

  /* Print out Z0 based on this mapping */
  printf("Z0 based on alternate formulation = %g Ohms\n", 
	 (1/(2*M_PI))*sqrt(mu0/(e0*line->er))*log(Ro));
#endif

#ifdef DEBUG_CALC
  printf("coax_calc_int():  z0 = %g ohms\n",line->z0);
#endif

  /*
   * find velocity (meters/second)
   */
  v = 1.0 / sqrt(mu0*e0*line->er);

  /*
   * find L and C from the impedance and velocity
   * 
   * z0 = sqrt(L/C), v = 1/sqrt(LC)
   * 
   * this gives the result below
   */
  line->L = line->z0/v;
  line->C = 1.0/(line->z0*v);

#ifdef DEBUG_CALC
  printf("coax_calc_int():  Found line->L (%p) = %g\n",
	 (void *) &(line->L), line->L);
#endif

  if (flag == CALC_ALL) {
#ifdef DEBUG_CALC
    printf("coax_calc_int():  Starting loss calculations\n");
#endif
    /* 
     * find G. 
     * Colin gives G for the case where line->c == 0.  This should
     * still hold for c != 0 since we accurately find incremental
     * capacitance for the line with offset center conductor
     */
    omega = 2.0 * M_PI * line->freq;
    line->G = omega*line->tand*line->C;

    /*
     * find R. 
     */

    if (omega > 0) {
      /* center and shield skin depths */
      delta_c = sqrt(2*line->rho_a/(omega*mu0));
      delta_s = sqrt(2*line->rho_b/(omega*mu0));
#ifdef DEBUG_CALC
      printf("coax_calc():  Found skin depths:\n");
      printf("coax_calc():  delta_c = %g meters\n", delta_c);
      printf("coax_calc():  delta_s = %g meters\n", delta_s);
#endif

    }


    /* 
     * For conductor losses, we compute two limiting cases.  First we
     * find the DC limiting resistance.  Then find the limiting
     * resistance in the skindepth limited HF case.  Finally, we
     * figure out where we are on the curve and interpolate to try and
     * provide a smooth transition in the region where we transition
     * from the DC case to the skin effect case.
     */

    /*
     * Find DC resistance
     */

    /* shield */
    if (line->rho_b == 0) {
      Rs0 = 0;
      Rs  = 0;
    } else {
      /*
       * cross sectional area is pi*(b+t)^2 - pi*(b)^2 =
       * pi * [ b^2 + 2*b*t + t^2 - b^2 ] = pi*[2*b*t + t^2]
       * = pi*t*[2*b + t] = 2*pi*t*[b + t/2]
       */
      /* zero frequency incremental conductance and resistance */
      Gs0 = 2.0 * M_PI * line->tshield * (line->b + line->tshield/2) / line->rho_b;
      Rs0 = 1/Gs0;
    }

    /* center conductor */
    if (line->rho_a == 0) {
      Rc0 = 0;
      Rc  = 0;
    } else {
      Gc0 = M_PI * line->a * line->a / line->rho_a;
      Rc0 = 1/Gc0;
    }

    /*
     * Wheelers incremental inductance calculation for finding
     * conductor losses.  This should correctly deal with
     * c != 0 (i.e. center conductor is offset)
     */

    /* clone the line */
    tmp_line = *line;
    
    /* find Z0 with er = 1.0 */
    tmp_line.er = 1.0;

#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  "
	   "********** Starting Wheelers Incremental Inductance #1 (er = 1) **********\n");
#endif
    rslt = coax_calc_int(&tmp_line, line->freq, CALC_MIN);
    z1 = tmp_line.z0;
    
    /* find Z0 with the conductor sizes modified with the skindepth */

    /* 
     * verify that it's 1/2 the skin depth on each conductor we change
     */

    /* 
     * center conductor.  Note that "a" is a radius.  We need to
     * subtract the skin depth in the center conductor.  Instead of
     * the true skin depth, use 1/100 of the conductor size
     */
    /*
    if( line->a > 6.0*delta_c) {
      tmp_line.a = line->a - 0.5*delta_c;
    */
    /*
     * pick a new analysis frequency that gives a skin depth which is
     * 1/10 the radius
     */
    tmp_line.freq = (2.0 * line->rho_a ) / (pow(0.01*line->a,2.0) * mu0);
    tmp_line.freq = tmp_line.freq / (2.0 * M_PI);

    tmp_line.a = line->a - 0.5*0.01*line->a;
#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  "
	   "********** Starting Wheelers Incremental Inductance #2 (modify center) **********\n");
#endif
    rslt = coax_calc_int(&tmp_line, tmp_line.freq, CALC_MIN);
    z2 = tmp_line.z0;

    /* conduction losses, nepers per meter */
    lc = (M_PI*tmp_line.freq/LIGHTSPEED)*(z2 - z1)/line->z0;
    
    /* high frequency resistance */
    RcHF = lc*2*line->z0;
#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  Found RcHF = %g Ohms which will be "
	   "scaled by %g\n", RcHF, 0.01*line->a/delta_c);
#endif
    /*
    } else {
      RcHF = 0.0;
    }
    */
    /* restore the center conductor size */
    tmp_line.a = line->a;

    /* 
     * Shield conductor.  Note that "b" is a radius. We need to
     * add the skin depth in the shield conductor 
     */
    /*
    if( 3.0*delta_s < line->tshield ) {
    tmp_line.b = line->b + 0.5*delta_s;
    */

    tmp_line.freq = (2.0 * line->rho_b ) / (pow(0.01*line->b,2.0) * mu0);
    tmp_line.freq = tmp_line.freq / (2.0 * M_PI);

    tmp_line.b = line->b + 0.5*0.01*line->b;
#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  "
	   "********** Starting Wheelers Incremental Inductance #3 (modify shield) **********\n");
#endif
    rslt = coax_calc_int(&tmp_line, tmp_line.freq, CALC_MIN);
    z2 = tmp_line.z0;
    lc = (M_PI*tmp_line.freq/LIGHTSPEED)*(z2 - z1)/line->z0;
    RsHF = lc*2*line->z0;
#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  Found RsHF = %g Ohms which will be "
	   "scaled by %g\n", RsHF, 0.01*line->b/delta_s);
#endif
    /*
    } else {
      RsHF = 0.0;
    }
    */

#ifdef DEBUG_CALC
    printf("\ncoax_calc_int():  "
	   "********** Finished Wheelers Incremental Inductance **********\n");
#endif

    /*
     * Now we have to figure out how to interpolate between our two
     * data points.
     *
     * To this end, we assume the resistance has the form:
     *
     * Rtot = Rdc  +  Rhf * ( (0.001*a_or_b)/skindepth )
     *
     * where Rdc = DC resistance and Rhf is the high frequency
     * resistance you would get if the skindepth were equal to 0.01*a
     * for the center conductor or 0.01*b for the shield.
     *
     * The high frequency resistance should vary as 1/skindepth so
     * if you know Rhf at skindepth = d1 you should be able to find it
     * at skindepth = d2 by multiplying by d1/d2
     */

    if (omega > 0) {
      Rc_delta = RcHF * 0.01*line->a / delta_c ;
      Rs_delta = RsHF * 0.01*line->b / delta_s ;
    } else {
      Rc_delta = 0.0;
      Rs_delta = 0.0;
    }

    /* Find total resistances for the center and shield */
    Rc = Rc0 + Rc_delta;
    Rs = Rs0 + Rs_delta;

    /* and finally, the total resistance */
    line->R = Rc + Rs;

#ifdef DEBUG_CALC
    printf("coax_calc_int(): Rshield  = %10.6g (dc) + %10.6g (rf) = %10.6g  Ohms/Meter\n",
	   Rs0, Rs_delta, Rs);
    printf("coax_calc_int(): Rcenter  = %10.6g (dc) + %10.6g (rf) = %10.6g  Ohms/Meter\n",
	   Rc0, Rc_delta, Rc);
    printf("coax_calc_int(): Rtotal   = %10.6g (dc) + %10.6g (rf) = %10.6g  Ohms/Meter\n",
	   Rc0 + Rs0, Rc_delta + Rs_delta, Rc + Rs);
#endif

    /* 
     * compute conductor losses and dielectric losses 
     */
    db_per_np = 20.0*log10(exp(1.0));
    line->alpha_c = db_per_np*0.5*line->R/line->z0;
    line->alpha_d = db_per_np*0.5*line->G*line->z0;

    line->losslen = line->alpha_c + line->alpha_d;

    line->loss = line->losslen*line->len;


    /*
     * TE10 mode cutoff frequency
     * XXX This is only for line->c == 0 despite the fact that this
     * equation was found in Rosloniec along with the eccentric line
     * impedance formula
     */

    /* 
     * approximate value
     */
    line->fc = v/(M_PI*(line->a + line->b));
    
    /*
     * Rather than use the approximate TE10 cutoff frequency from
     * Rosloniec, use an exact solution.  This can actually be solved
     * by hand without too much pain for the case where c == 0.
     *
     * Start with the value of 'k' found by the approximate cutoff
     * frequency calculation and iterate until we exactly meet the
     * boundary conditions for the TE10 cutoff.
     *
     */

    k = 2*M_PI*line->fc / v;
    kold = 0.99*k;
    err = coax_TE10_err(line,k);
    errold = coax_TE10_err(line,kold);
    
#ifdef DEBUG_CALC
    printf("coax_calc_int():  finding TE10 cutoff freq.  initial = %g Hz\n",
	   line->fc);
#endif
    i=0;
    while ( (fabs(err/k) > 5*DBL_EPSILON) && 
	    (k != kold) &&
	    (i < 10)  ){

      ddk = (err - errold)/(k - kold);
      
      /* avoid divide by zero */
      if (ddk == 0.0)
	break;

      kold = k;
      errold = err;
      k = k - err/ddk;
      err = coax_TE10_err(line,k);
      i++;

#ifdef DEBUG_CALC
      printf("coax_calc_int():  k=%10.6g,"
	     " err=%12.8g\n", k, err);
#endif
    }

    line->fc = k*v/(2*M_PI);
#ifdef DEBUG_CALC
    printf("coax_calc_int():  finding TE10 cutoff freq.  final = %g Hz\n",
	   line->fc);
#endif

  } else {	   
#ifdef DEBUG_CALC
    printf("coax_calc_int():  Skipping loss calculations\n");
#endif
  }

  /*
   * electrical length 2*pi*f*(180/pi) = 360*f
   */
  line->delay = line->len/v;
  line->elen = 360.0*line->freq*line->delay;

  /* XXX need to calculate max E-field strength */
  line->emax = 0.0;

  return 0;
}


int coax_syn(coax_line *line, double f, int flag)
{
  int rslt = 0;
  double Ro;
  double v;
  double elen;

  /* the optimization variables, current, min/max, and previous values */
  double var=0, varmax=0, varmin=0, varold=0;
  
  /* errors due to the above values for the optimization variable */
  double err=0, errmax=0, errmin=0, errold=0;

  /* derivative */
  double deriv;

  /* the sign of the slope of the function being optimized */
  double sign=0;

  /* pointer to which parameter of the line is being optimized */
  double *optpar;

  /* number of iterations so far, and max number allowed */
  int iters=0;
  int maxiters=100;
  
  /* convergence parameters */
  double abstol=0.1e-6;
  double reltol=0.01e-6;

  /* flag to end optimization */
  int done=0;

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
  case COAXSYN_A:
    optpar = &(line->a);
    varmax = 0.999*line->b;
    varmin = 0.001*line->b;
    var    = 0.2*line->b;
    break;

  case COAXSYN_B:
    optpar = &(line->b);
    varmax = 1000.0*line->a;
    varmin = 1.001*line->a;
    var    = 5*line->a;
    break;

  case COAXSYN_C:
    optpar = &(line->c);
    varmax = 0.999*(line->b - line->a);
    varmin = 0;
    var    = 0.1*varmax;
    break;

  case COAXSYN_ER:
    optpar = &(line->er);
    varmax = 100.0;
    varmin = 1.0;
    var    = 5.0;
    break;

  default:
    fprintf(stderr,"coax_syn():  illegal flag=%d\n",flag);
    exit(1);
    break;
  }

  /*
   * read values from the input line structure
   */

  Ro   = line->z0;
  elen = line->elen;

  /*
   * temp value for len used while synthesizing the other parameters.
   * We'll correct len later.
   */

  line->len=1.0;

  line->freq = f;

#ifdef DEBUG_SYN
  printf("coax_syn():  ----------------------\n");
  printf("coax_syn():  Input values:\n");
  printf("coax_syn():  ----------------------\n");
  printf("coax_syn():  a    = %g meters\n",line->a);
  printf("coax_syn():  b    = %g meters\n",line->b);
  printf("coax_syn():  c    = %g meters\n",line->c);
  printf("coax_syn():  freq = %g MHz\n",line->freq*1e-6);
  printf("coax_syn():  ----------------------\n");
  printf("coax_syn():  desired z0  = %g ohms\n",line->z0);
  printf("coax_syn():  \n");
  printf("coax_syn():  ----------------------\n");
  printf("coax_syn(): Starting optimization value = %g\n",var);
  printf("coax_syn():  ----------------------\n");
#endif

  if (!done){
    /* Initialize the various error values */
    *optpar = varmin;
    rslt = coax_calc_int(line,f,CALC_MIN);
    if (rslt)
      return rslt;
    errmin = line->z0 - Ro;

    *optpar = varmax;
    rslt = coax_calc_int(line,f,CALC_MIN);
    if (rslt)
      return rslt;
    errmax = line->z0 - Ro;

    *optpar = var;
    rslt = coax_calc_int(line,f,CALC_MIN);
    if (rslt)
      return rslt;
    err = line->z0 - Ro;

    varold = 0.99*var;
    *optpar = varold;
    rslt = coax_calc_int(line,f,CALC_MIN);
    if (rslt)
      return rslt;
    errold = line->z0 - Ro;


    /* see if we've actually been able to bracket the solution */
    if (errmax*errmin > 0){
      alert("Could not bracket the solution.\n"
	    "Synthesis failed.\n");
      return -1;
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
      printf("coax_syn():  Taking a bisection step\n");
#endif
      var = (varmin + varmax)/2.0;
    }

    /* update the error value */
    *optpar = var;
    rslt = coax_calc_int(line,f,CALC_MIN);
    err = line->z0 - Ro;
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
      printf("coax_syn():  abstol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if ( fabs((var-varold)/var) < reltol){
      done = 1;
#ifdef DEBUG_SYN
      printf("coax_syn():  reltol converged after iteration #%d\n",
	     iters);
#endif
    }
    else if (iters >= maxiters){
      alert("Synthesis failed to converge in\n"
	    "%d iterations\n", maxiters);
      return -1;
    }
    

#ifdef DEBUG_SYN
      printf("coax_syn(): iteration #%d:  var = %g\terr = %g\n",iters,var,err);
#endif
      /* done with iteration */
  }

  /* velocity on line */
  rslt = coax_calc(line,f);
  if (rslt)
    return rslt;

  v = LIGHTSPEED / sqrt(line->er);

  line->len = (elen/360)*(v/f);

  /* recalculate using real length to find loss  */
  rslt = coax_calc(line,f);
  if (rslt)
    return rslt;
  
#ifdef DEBUG_SYN
  printf("synthesis for Z0=%g [ohms] and elen=%g [deg]\n",
	 line->z0,line->elen);
  printf("produced:\n");
  printf("\ta = %g [m] \n\tb = %g [m] \n\t c = %g [m]\n",
	 line->a,line->b,line->c);
#endif

  return 0;
}


void coax_free(coax_line *line)
{

  wc_units_free(line->units_emax);
  wc_units_free(line->units_fc);
  wc_units_free(line->units_delay);
  wc_units_free(line->units_loss);
  wc_units_free(line->units_losslen);
  wc_units_free(line->units_abct);
  wc_units_free(line->units_len);
  wc_units_free(line->units_freq);
  wc_units_free(line->units_rho);
  wc_units_free(line->units_L);
  wc_units_free(line->units_R);
  wc_units_free(line->units_C);
  wc_units_free(line->units_G);
  free(line);
}


coax_line *coax_new()
{
  coax_line *newline;

  newline = (coax_line *) malloc(sizeof(coax_line));
  if(newline == NULL)
    {
      fprintf(stderr,"coax.c:coax_new(): malloc() failed\n");
      exit(1);
    }

  /* create the units */

  newline->units_emax    = wc_units_new(WC_UNITS_ELECTRIC_FIELD);
  newline->units_fc      = wc_units_new(WC_UNITS_FREQUENCY);
  newline->units_delay   = wc_units_new(WC_UNITS_TIME);
  newline->units_loss    = wc_units_new(WC_UNITS_DB);
  newline->units_losslen = wc_units_new(WC_UNITS_DB_PER_LEN);

  newline->units_abct = wc_units_new(WC_UNITS_LENGTH);
  newline->units_len = wc_units_new(WC_UNITS_LENGTH);
  newline->units_freq = wc_units_new(WC_UNITS_FREQUENCY);
  newline->units_rho = wc_units_new(WC_UNITS_RESISTIVITY);
  newline->units_L = wc_units_new(WC_UNITS_INDUCTANCE_PER_LEN);
  newline->units_R = wc_units_new(WC_UNITS_RESISTANCE_PER_LEN);
  newline->units_C = wc_units_new(WC_UNITS_CAPACITANCE_PER_LEN);
  newline->units_G = wc_units_new(WC_UNITS_CONDUCTANCE_PER_LEN);
  
  /* load in the defaults */
  coax_load_string(newline, default_coax);

  /* get the rest of the entries in sync */
  coax_calc(newline,newline->freq);

  return(newline);
}


