/* $Id: coax.c,v 1.2 2001/11/28 07:18:57 dan Exp $ */

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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mathutil.h"
#include "physconst.h"
#include "coax.h"
#include "alert.h"
#include "misc.h"



static int coax_calc_int(coax_line *line, double freq, int flag);

/* coax_calc_int flags */
#define CALC_ALL  0  /* all calculation (calculate everything) */ 
#define CALC_MIN  1  /* minimum calculation (skip some)        */ 


int coax_calc(coax_line *line, double freq)
{
  int rslt;

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
  double Gc,Gs;
  double delta_c,delta_s;
  double omega;

#ifdef DEBUG_CALC
  printf("coax_calc():  ----------------------\n");
  printf("coax_calc():  Input values:\n");
  printf("coax_calc():  ----------------------\n");
  printf("coax_calc():  a    = %g meters\n",line->a);
  printf("coax_calc():  b    = %g meters\n",line->b);
  printf("coax_calc():  c    = %g meters\n",line->c);
  printf("coax_calc():  freq = %g MHz\n",line->freq*1e-6);
  printf("coax_calc():  ----------------------\n");
#endif


  if (line->c >= line->b - line->a) {
    alert("Error:  c (%g)  must be < b - a (%g)\r\n"
	  "for a coax line\r\n",line->c,line->b-line->a);
    return -1;
  }

  /*
   * find characteristic impedance (from Rosloniec)
   */
  x = (line->a + ( pow(line->b,2.0) - pow(line->c,2.0) )/line->a) / 
    (2*line->b);


  mu0 = 4*M_PI*1.0e-7;
  e0 = 1.0/(mu0*LIGHTSPEED*LIGHTSPEED);

  line->z0 = (1/(2*M_PI))*sqrt(mu0/(e0*line->er))*log(x + sqrt(x*x - 1));

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

  if (flag == CALC_ALL) {
    /*
     * XXX
     * The loss equations are a bit more of an approximation
     */

    /* 
     * find G. 
     * Colin gives G for the case where line->c == 0.  This should
     * still hold for c != 0.
     */
    omega = 2.0 * M_PI * line->freq;
    line->G = omega*line->tand*line->C;

    /*
     * find R. 
     *
     * XXX 
     * Current crowding more to one side of the conductor than another
     * when  c != 0 breaks these equations.
     */
    if (omega > 0) {
      /* center and shield skindepths */
      delta_c = sqrt(2*line->rho_a/(omega*mu0));
      delta_s = sqrt(2*line->rho_b/(omega*mu0));
    }
    
    /*
     * Resistance per length of shield (outer conductor)
     */
    if ( (omega == 0) || (delta_s/line->tshield > 1e6) ) {
      /* DC case */
      Gs = 2.0 * M_PI * line->tshield * (line->b + line->tshield/2) / line->rho_b;
    }
    else {
      /* frequency dependent case */
      Gs = ( (2.0 * M_PI * delta_s) / line->rho_b ) *
	( (line->b + delta_s) - (line->b + line->tshield + delta_s) * 
	  exp(-line->tshield/delta_s) );
    }

    /*
     * Resistance per length of center conductor
     */
    if ( (omega == 0) || (delta_c/line->a > 1e6) ) {
      /* DC case */
      Gc = M_PI * line->a * line->a;
    }
    else {
      /* frequency dependent case */
      Gc = ( (2.0 * M_PI * delta_c) / line->rho_a ) *
	( line->a*delta_c - delta_c*delta_c*(1-exp(-line->a/delta_c)) );
    }

    line->R = 1.0/Gs + 1.0/Gc;

    /* 
     * compute conductor losses and dielectric losses 
     */
    line->alpha_c = 0.5*line->R/line->z0;
    line->alpha_d = 0.5*line->G*line->z0;
  }

  /*
   * electrical length 2*pi*f*(180/pi) = 360*f
   */
  line->elen = 360.0*line->freq*line->len/v;

  /*
   * TE11 mode cutoff frequency
   * XXX This is only for line->c == 0 despite the fact that this
   * equation was found in Rosloniec along with the eccentric line
   * impedance formula
   */
  line->fc = v/(M_PI*(line->a + line->b));

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

  newline->a = 1.0;
  newline->b = 2.0;
  newline->c = 0.0;
  newline->tshield = 0.1;
  newline->len = 1.0;

  newline->rho_a = 3e-8;
  newline->rho_b = 3e-8;
  newline->er    = 1.0;
  newline->tand  = 1e-6;
  newline->freq  = 10e6;
  newline->emax  = 1e8;

  newline->a_sf = 1.0;
  newline->b_sf = 1.0;
  newline->c_sf = 1.0;
  newline->tshield_sf = 1.0;
  newline->len_sf = 1.0;
  newline->rho_a_sf = 1.0;
  newline->rho_b_sf = 1.0;
  newline->emax_sf = 1.0;
  newline->L_sf = 1.0;
  newline->R_sf = 1.0;
  newline->C_sf = 1.0;
  newline->G_sf = 1.0;

  newline->fc_sf = 1.0;
  newline->freq_sf = 1.0e6;

  newline->delay_sf = 1.0;
  newline->loss_sf = 1.0;
  newline->losslen_sf = 1.0;

  newline->a_units = "m";
  newline->b_units = "m";
  newline->c_units = "m";
  newline->tshield_units = "m";
  newline->len_units = "m";
  newline->rho_a_units = "Ohm-m";
  newline->rho_b_units = "Ohm-m";
  newline->emax_units = "V/m";
  newline->L_units = "Henries/m";
  newline->R_units = "Ohms/m";
  newline->C_units = "Farads/m";
  newline->G_units = "Siemens/m";
  newline->freq_units="MHz";

  newline->delay_units = "s";
  newline->loss_units = "dB";
  newline->losslen_units = "dB/m";


  /* get the rest of the entries in sync */
  coax_calc(newline,newline->freq);

  return(newline);
}

