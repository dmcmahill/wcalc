/*      $Id: stripline.c,v 1.6 2002/01/14 20:23:51 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002 Dan McMahill
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
 *  References:
 *    Stanislaw Rosloniec
 *    "Algorithms For Computer-Aided Design of Linear Microwave Circuits"
 *    Archtech House, 1990
 *    ISBN 0-89006-354-0
 *    TK7876.R67 1990
 *
 *    H. A. Wheeler, "Formulas for the skin effect", Proc. IRE,
 *    Vol. 30, No. 9, September 1942, pp. 412-4124
 *
 */

/* debug stripline_syn()  */
/* #define DEBUG_SYN  */ 

/* debug stripline_calc() */
/* #define DEBUG_CALC */

#include "config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "alert.h"
#include "mathutil.h"
#include "misc.h"
#include "physconst.h"
#include "stripline.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/* flags for stripline_calc_int() */
#define NOLOSS   0
#define WITHLOSS 1

static int stripline_calc_int(stripline_line *line, double f, int flag);


/*
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
 * 
 *  In the case where Tmet~0 (or at least Tmet << H), an exact
 *  solution has been derived by Cohn for the characteristic
 *  impedance.  Wheeler has given a correction for Tmet.
 *
 */

int stripline_calc(stripline_line *line, double f)
{
  int rslt;
  
#ifdef DEBUG_CALC
  printf("stripline_calc(): --------------- Stripline Analysis ------------\n");
  printf("stripline_calc(): Metal width                 = %g m\n",line->w);
  printf("stripline_calc():                             = %g %s\n",
	 line->w/line->w_sf,line->w_units);
  printf("stripline_calc(): Metal thickness             = %g m\n",line->subs->tmet);
  printf("stripline_calc():                             = %g %s\n",
	 line->subs->tmet/line->subs->tmet_sf,line->subs->tmet_units);
  printf("stripline_calc(): Metal relative resistivity  = %g \n",line->subs->rho);
  printf("stripline_calc(): Metal surface roughness     = %g m-rms\n",line->subs->rough);
  printf("stripline_calc():                             = %g %s\n",
	 line->subs->rough/line->subs->rough_sf,line->subs->rough_units);
  printf("stripline_calc(): Substrate thickness         = %g m\n",line->subs->h);
  printf("stripline_calc():                             = %g %s\n",
	 line->subs->h/line->subs->h_sf,line->subs->h_units);
  printf("stripline_calc(): Substrate dielectric const. = %g \n",line->subs->er);
  printf("stripline_calc(): Substrate loss tangent      = %g \n",line->subs->tand);
  printf("stripline_calc(): Frequency                   = %g MHz\n",f/1e6); 
  printf("stripline_calc():                             = %g %s\n",
	 f/line->freq_sf,line->freq_units);
  printf("stripline_calc(): -------------- ---------------------- ----------\n");
#endif
  rslt=stripline_calc_int(line, f, WITHLOSS);
  return(rslt);
}

static int stripline_calc_int(stripline_line *line, double f, int flag)
{

  /* calculation variables */
  double k,kp,r,kf,z0,m,deltaW,A,v,loss;

  /* loss variables*/
  double z1,z2,lc,ld,delta;
  double mu,sigma;

  int rslt;
  stripline_line tmp_line;

#ifdef DEBUG_CALC
  printf("stripline_calc_int(): --------------- Stripline Analysis ------------\n");
  printf("stripline_calc_int(): flag                        = %d\n",flag);
  printf("stripline_calc_int():       WITHLOSS              = %d\n",WITHLOSS);
  printf("stripline_calc_int():       NOLOSS                = %d\n",NOLOSS);
  printf("stripline_calc_int(): -------------- ---------------------- ----------\n");
#endif

  /*
   * Characteristic Impedance
   */

  if(line->subs->tmet < line->subs->h/1000)
    {
      /*
       * Thin strip case:
       */
#ifdef DEBUG_CALC
       printf("stripline_calc_int(): Thin stripline analysis\n");
#endif
       k = 1/cosh(M_PI*line->w/(2*line->subs->h));
   
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
   
   
      z0 = 29.976*M_PI*r/sqrt(line->subs->er);
    }
  else
    {

      /*
       * Finite strip case: 
       */
#ifdef DEBUG_CALC
       printf("stripline_calc_int(): Finite stripline analysis\n");
#endif

      m = 6.0*(line->subs->h-line->subs->tmet)/(3.0*line->subs->h-line->subs->tmet);
      deltaW = (line->subs->tmet/M_PI)*
	(1.0 - 0.5*log(
		       pow((line->subs->tmet/(2*line->subs->h-line->subs->tmet)),2.0) + 
		       pow((0.0796*line->subs->tmet/(line->w + 1.1*line->subs->tmet)),m) ));
      A = 4.0*(line->subs->h-line->subs->tmet)/(M_PI*(line->w + deltaW));
      z0 = (30/sqrt(line->subs->er))*log(1.0 + A*(2.0*A + sqrt(4.0*A*A + 6.27)));
    }

  /* done with Z0 calculation */



  /*
   * Electrical Length
   */

  /* propagation velocity (meters/sec) */
  v = LIGHTSPEED / sqrt(line->subs->er);
  line->len = 360*line->l*f/v;

  /*
   * delay on line (ns)
   */
  line->delay = line->l / v;


   line->z0 = z0;
   line->Ro = z0;
   line->Xo = 0.0;

   /* find the incremental circuit model */
   
   /*
    * find L and C from the impedance and velocity
    * 
    * z0 = sqrt(L/C), v = 1/sqrt(LC)
    * 
    * this gives the result below
    */
   line->Ls = line->z0/v;
   line->Cs = 1.0/(line->z0*v);

   /* resistance will be updated below */
   line->Rs = 0.0;
   line->Gs = 2*M_PI*f*line->Cs*line->subs->tand;


  /* Loss */
   if(flag == WITHLOSS) {
#ifdef DEBUG_CALC
       printf("stripline_calc_int():  starting loss calculations\n");
#endif
       /*
	* Dielectric Losses
	*/
   
       /* loss in nepers/meter */
       ld=line->Gs*line->z0/2;

       /* loss in dB/meter */
       ld = 20.0*log10(exp(1.0)) * ld;
   
       /* loss in dB */
       ld = ld * line->l;
   
       /*
	* Conduction Losses
	*/
       
       /* calculate skin depth */
   
       /* conductivity (rho is relative to copper) */
       sigma = 5.8e7 / line->subs->rho;
   
       /* permeability of free space */
       mu = 4.0*M_PI*1e-7;
   
       /* skin depth in meters */
       line->skindepth = sqrt(1.0/(M_PI*f*mu*sigma));
       delta = line->skindepth;

#ifdef notdef
       /* warn the user if the loss calc is suspect. */
       if(line->subs->tmet < 3.0*line->skindepth)
	 {
	   printf("Warning:  The metal thickness is less than\n");
	   printf("three skin depths.  Use the loss results with\n");
	   printf("caution.\n");
	 }
#endif

       /*
	* if the skinDepth is greater than Tmet, assume current
	* flows uniformly through  the conductor.  Then loss
	* is just calculated from the dc resistance of the
	* trace.  This is somewhat incorrect
	* but I dont have time right now to come up
	* with a better result.
	*/
       if(line->skindepth <= line->subs->tmet)
	 {
   
	   /* Use Wheelers "" approach */

	   /* clone the line */
	   tmp_line = *line;
	   tmp_line.subs = stripline_subs_new();
	   *(tmp_line.subs) = *(line->subs);
	   
	   tmp_line.subs->er = 1.0;
	   rslt = stripline_calc_int(&tmp_line,f,NOLOSS);
	   z1=tmp_line.z0;

	   tmp_line.w = line->w - line->skindepth;
	   tmp_line.subs->tmet = line->subs->tmet - line->skindepth;
	   tmp_line.subs->h = line->subs->h + line->skindepth;
	   rslt = stripline_calc_int(&tmp_line,f,NOLOSS);
	   z2 = tmp_line.z0;
	   free(tmp_line.subs);

	   /* conduction losses, nepers per meter */
	   lc = (M_PI*f/LIGHTSPEED)*(z2 - z1)/z0;
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): HF conduction loss, z1=%g,z2=%g,z0=%g,lc=%g\n",
		  z1,z2,z0,lc);
#endif
	   line->Rs = lc*2*line->z0;

	 }

       /* "dc" case  */
       else if(line->subs->tmet > 0.0)
	 {
	   /* resistance per meter = 1/(Area*conductivity) */
	   line->Rs = 1/(line->w*line->subs->tmet*sigma);  
  
	   /* conduction losses, nepers per meter */
	   lc = line->Rs/(2.0*z0);

	   /*
	    * change delta to be equal to the metal thickness for
	    * use in surface roughness correction
	    */
	   delta = line->subs->tmet;
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): LF conduction loss, R=%g, lc=%g\n",
		  line->Rs,lc);
#endif
	   /* no conduction loss case */
	 }
       else
	 {
	   lc=0.0;
#ifdef DEBUG_CALC
	   printf("stripline_calc_int(): 0 thickness strip.  setting"
		  "conduction loss=0\n");
#endif
	 }

 
       /* loss in dB/meter */
       lc = 20.0*log10(exp(1.0)) * lc;
   
       /* loss in dB */
       lc = lc * line->l;
   
       /* factor due to surface roughness
	* note that the equation in Fooks and Zakarevicius is slightly 
	* errored.   
	* the correct equation is penciled in my copy and was 
	* found in Hammerstad and Bekkadal
	*/
       lc = lc * (1.0 + (2.0/M_PI)*atan(1.4*pow((line->subs->rough/delta),2.0)));
   
       /*
	* Total Loss
	*/
   
       loss = ld + lc;
   
     } /* if (flag == WITHLOSS ) */
   else
     {
       loss = 0.0;
     }

   line->loss = loss;
   line->losslen = loss/line->l;
   
   /* XXX - need open circuit end correction for stripline */
   line->deltal=0;
   
   return 0;
}



/*
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
 */


int stripline_syn(stripline_line *line, double f, int flag)
{
  int rslt;

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
    varmax = 100.0*line->subs->h;
    varmin = 0.01*line->subs->h;
    var    = line->subs->h;
    break;

  case SLISYN_H:
    optpar = &(line->subs->h);
    varmax = 100.0*line->w;
    varmin = 0.01*line->w;
    var    = line->w;
    break;

  case SLISYN_ER:
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
  printf("stripline_syn(): Metal width                 = %g m\n",line->w);
  printf("stripline_syn():                             = %g %s\n",
	 line->w/line->w_sf,line->w_units);
  printf("stripline_syn(): Metal thickness             = %g m\n",line->subs->tmet);
  printf("stripline_syn():                             = %g %s\n",
	 line->subs->tmet/line->subs->tmet_sf,line->subs->tmet_units);
  printf("stripline_syn(): Metal relative resistivity  = %g \n",line->subs->rho);
  printf("stripline_syn(): Metal surface roughness     = %g m-rms\n",line->subs->rough);
  printf("stripline_syn():                             = %g %s\n",
	 line->subs->rough/line->subs->rough_sf,line->subs->rough_units);
  printf("stripline_syn(): Substrate thickness         = %g m\n",line->subs->h);
  printf("stripline_syn():                             = %g %s\n",
	 line->subs->h/line->subs->h_sf,line->subs->h_units);
  printf("stripline_syn(): Substrate dielectric const. = %g \n",line->subs->er);
  printf("stripline_syn(): Substrate loss tangent      = %g \n",line->subs->tand);
  printf("stripline_syn(): Frequency                   = %g MHz\n",f/1e6); 
  printf("stripline_syn():                             = %g %s\n",
	 f/line->f_sf,line->f_units);
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
  printf("stripline_syn(): Desired Zo                  = %g ohm\n",Ro);
  printf("stripline_syn(): Desired electrical length   = %g degrees\n",len);
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
  printf("stripline_syn(): Starting optimization value = %g\n",var);
  printf("stripline_syn(): Optimization flag           = %d\n",flag);
  printf("stripline_syn(): -------------- ---------------------- ----------\n");
#endif

  if (!done){
    /* Initialize the various error values */
    *optpar = varmin;
    rslt = stripline_calc_int(line,f,NOLOSS);
    if (rslt)
      return rslt;
    errmin = line->z0 - Ro;

    *optpar = varmax;
    rslt = stripline_calc_int(line,f,NOLOSS);
    if (rslt)
      return rslt;
    errmax = line->z0 - Ro;

    *optpar = var;
    rslt = stripline_calc_int(line,f,NOLOSS);
    if (rslt)
      return rslt;
    err = line->z0 - Ro;

    varold = 0.99*var;
    *optpar = varold;
    rslt = stripline_calc_int(line,f,NOLOSS);
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
      printf("stripline_syn():  Taking a bisection step\n");
#endif
      var = (varmin + varmax)/2.0;
    }

    /* update the error value */
    *optpar = var;
    rslt = stripline_calc_int(line,f,NOLOSS);
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
      alert("Synthesis failed to converge in\n"
	    "%d iterations\n", maxiters);
      return -1;
    }
    

#ifdef DEBUG_SYN
      printf("stripline_syn(): iteration #%d:  var = %g\terr = %g\n",iters,var,err);
#endif
      /* done with iteration */
  }

  /* velocity on line */
  stripline_calc(line,f);

  v = LIGHTSPEED / sqrt(line->subs->er);

  line->l = (len/360)*(v/f);


  /* recalculate using real length to find loss  */
  stripline_calc(line,f);
  
#ifdef DEBUG_SYN
  printf("synthesis for Z0=%g [ohms] and len=%g [deg]\n",line->z0,line->len);
  printf("produced:\n");
  printf("\twidth = %g [m] \n\tlength = %g [m]\n",line->w,line->l);
  printf("\twidth = %g [%s] \n\tlength = %g [%s]\n",
	 line->w/line->w_sf,line->w_units,
	 line->l/line->l_sf,line->l_units);
#endif

  return 0;
}



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

  /* XXX need better units initialization */
  /* initialize the values to something */
  newline->l    = 1000.0;
  newline->l_sf = 1.0;
  newline->l_units = "m";
  newline->w    = 110.0;
  newline->w_sf = 1.0;
  newline->w_units = "m";
  newline->freq = 900;
  newline->freq_sf = 1e6;
  newline->freq_units = "MHz";

  newline->skindepth_units="m";
  newline->skindepth_sf=1.0;

  newline->delay_units="s";
  newline->delay_sf=1.0;

  newline->Ls_sf = 1.0;
  newline->Rs_sf = 1.0;
  newline->Cs_sf = 1.0;
  newline->Gs_sf = 1.0;

  newline->Ls_units = "Henries/m";
  newline->Rs_units = "Ohms/m";
  newline->Cs_units = "Farads/m";
  newline->Gs_units = "Siemens/m";

  newline->subs->h     = 62.0;
  newline->subs->h_sf = 1.0;
  newline->subs->h_units = "m";
  newline->subs->er    = 4.8;
  newline->subs->tand  = 0.01;
  newline->subs->tmet  = 1.4;
  newline->subs->tmet_sf = 1.0;
  newline->subs->tmet_units = "m";
  newline->subs->rho   = 1.0;
  newline->subs->rho_sf = 1.0;
  newline->subs->rho_units = "Ohm-m";
  newline->subs->rough = 0.055;
  newline->subs->rough_sf = 1.0;
  newline->subs->rough_units = "m";

  /* and do a calculation to finish the initialization */
  stripline_calc(newline,newline->freq);

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
