/* $Id: coupled_stripline.c,v 1.9 2006/02/13 22:51:59 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2006 Dan McMahill
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

/* #define DEBUG_SYN  */ /* debug coupled_stripline_syn()  */
/* #define DEBUG_CALC */ /* debug coupled_stripline_calc() */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "alert.h"
#include "defaults.h"
#include "mathutil.h"
#include "physconst.h"
#include "units.h"
#include "coupled_stripline.h"
#include "coupled_stripline_loadsave.h"
#include "stripline.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/* Zero thickness characteristic impedance */
static int z0_zerot(double w, double s, double b, double er,
		    double *z0e, double *z0o);


/*
 * Impedance only calculation (used for incremental inductance loss
 * calculations) 
 */
static int find_z0(coupled_stripline_line *line);

/*
 * Analyze coupled stripline transmission line from physical parameters
 *
 *   /////////////////ground///////////////////////
 *   ----------------------------------------------
 *  (                                     /|\     (
 *   )      |<--W-->|<---S--->|<--W-->|    |       )
 *  (        _______           _______     |      (
 *   )      | metal |         | metal |    |       )
 *  (        -------           -------     |      (
 *   ) dielectric, er                  H   |       )
 *  (                                     \|/     (
 *   ----------------------------------------------
 *   /////////////////ground///////////////////////
 *
 *  Reference:
 *
 *    S. B. Cohn, "Shielded Coupled-Strip Transmission Line",
 *      Lines", IRE Transactions on Microwave Theory and Techniques, Vol MTT-3,
 *      No 10, October 1955, pp. 29-38.
 *
 */

int coupled_stripline_calc(coupled_stripline_line *line, double f)
{
  coupled_stripline_line tmp_line;
  double z1e, z1o, z2e, z2o;
  double lce, lco, ld;
  double lce_hf, lco_hf;

  double Re_hf, Ro_hf, Re_dc, Ro_dc, freq_hf;

  /* for skindepth calculation  */
  double mu, sigma;
  double deltax_e, deltax_o, delta0;

  double db_per_np;

  int rslt;

 line->freq = f;

#ifdef DEBUG_CALC
  printf("coupled_stripline_calc(): --------- Coupled_Stripline Analysis ----------\n");
  printf("coupled_stripline_calc(): Metal width                 = %g %s\n",
	 line->w/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_calc(): Metal spacing               = %g %s\n",
	 line->s/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_calc(): Metal thickness             = %g %s\n",
	 line->subs->tmet/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_calc(): Metal resistivity           = %g %s\n",
	 line->subs->rho/line->units_rho->sf, line->units_rho->name);
  printf("coupled_stripline_calc(): Metal surface roughness     = %g %s-rms\n",
	 line->subs->rough/line->units_rough->sf, line->units_rough->name);
  printf("coupled_stripline_calc(): Substrate thickness         = %g %s\n",
	 line->subs->h/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_calc(): Substrate dielectric const. = %g \n",
	 line->subs->er);
  printf("coupled_stripline_calc(): Substrate loss tangent      = %g \n",
	 line->subs->tand);
  printf("coupled_stripline_calc(): Frequency                   = %g %s\n",
	 line->freq/line->units_freq->sf, line->units_freq->name);
  printf("coupled_stripline_calc(): -------------- ---------------------- ----------\n");
#endif

  /* find impedances */
  find_z0( line );

  /* Metal conductivity */
  sigma = 1.0 / line->subs->rho;

  /* permeability of free space */
  mu = 4.0*M_PI*1e-7;
  
  /* skin depth in meters */
  if( line->freq > 0.0 ) 
    line->skindepth = sqrt(1.0 / (M_PI * line->freq * mu * sigma));
  else
    line->skindepth = 0.0;


  if( line->freq > 0.0 ) {
    /* 
     * Find the high frequency asymptotic behaviour of the resistance
     * using Wheelers incremental inductance rule as a frequency where
     * we're well into the skin effect limited region.
     */
    
    tmp_line = *line;
    tmp_line.subs = stripline_subs_new();
    *(tmp_line.subs) = *(line->subs);
    tmp_line.subs->er = 1.0;
    
    /*
     * pick a new analysis frequency that gives a skin depth which is
     * 1/10 the metal thickness
     *
     * skindepth = sqrt(1.0 / (M_PI * freq * mu * sigma))
     * skindepth^2 * PI * freq * mu * sigma = 1.0
     * freq = 1.0 / (skindepth^2 * PI * mu * sigma)
     */
    tmp_line.freq = 
      1.0 / ( pow(0.1 * line->subs->tmet, 2.0) * M_PI * mu * sigma);
  
    rslt = find_z0(&tmp_line);
    z1e = tmp_line.z0e;
    z1o = tmp_line.z0o;

    /* skindepth at which we'll do the following analysis */
    delta0 = 0.1*line->subs->tmet;

    tmp_line.w = line->w - delta0;
    tmp_line.s = line->s + delta0;
    tmp_line.subs->tmet = line->subs->tmet - delta0;
    tmp_line.subs->h = line->subs->h + delta0;
    rslt = find_z0(&tmp_line);
    z2e = tmp_line.z0e;
    z2o = tmp_line.z0o;
    free(tmp_line.subs);

    /* find what frequency this analysis must have been at */
    freq_hf = tmp_line.freq;

    /* find high frequency resistance */
    lce_hf  = (M_PI*freq_hf/LIGHTSPEED)*(z2e - z1e)/line->z0e;
    lco_hf  = (M_PI*freq_hf/LIGHTSPEED)*(z2o - z1o)/line->z0o;
    Re_hf   = lce_hf * 2.0 * line->z0e;
    Ro_hf   = lco_hf * 2.0 * line->z0o;
  } else {
    /* not really used, but this shuts up the compiler */
    Re_hf = 0.0;
    Ro_hf = 0.0;
    delta0 = 0.0;
  }


  /* DC resistance */
  Re_dc = 1.0 / (line->w * line->subs->tmet * sigma);
  Ro_dc = 1.0 / (line->w * line->subs->tmet * sigma);
  
  
  /*
   * Now we have to figure out how to interpolate between our two
   * data points.
   *
   * To this end, we assume the resistance has the form:
   *
   * The high frequency resistance should vary as 1/skindepth so
   * if you know Rhf at skindepth = d1 you should be able to find it
   * at skindepth = d2 by multiplying by d1/d2
   *
   * Since skindepth is proportional to 1/sqrt(freq), our true high
   * frequency resistance is R_hf * sqrt(freq / freq_hf)
   *
   *
   * An approximate form for the resistance which is valid in both
   * skin effect and low frequency regions is:
   *
   *               Rdc * T
   * R = ----------------------------        (1)
   *      delta * (1 - exp(-T/delta))
   *
   * where Rdc is the DC resistance, T is the thickness, and delta is
   * the skin depth.
   *
   * At low frequencies, this becomes Rdc
   * At high frequencies, this becomes Rdc*T/delta
   *
   * The trick now is we have expressions for both the DC resistance
   * of our line and for the high frequency even/odd resistance
   * behaviour.  If we define delta0 as the skindepth used to
   * calculate R = Rhf, then at a different frequency, we have
   * R = Rhf * delta0 / delta.  
   *
   * What we can do, which is a bit of a hack, but it makes us get the
   * right values well into either region along with smooth curves in
   * the middle is to modify the value of delta used in (1) above from
   * the true skin depth in order to obtain the right limiting value
   * for resistance well into the skin effect region.
   *
   * To do this, we simply set
   *
   * Rdc * T / deltaX = Rhf * delta0 / delta 
   *
   *                        Rdc * T
   * =>  deltaX = delta * -------------
   *                       Rhf * delta0
   */

  if( line->freq > 0.0) {
    /* find our "effective skindepth" */
    deltax_e = line->skindepth * Re_dc * line->subs->tmet / (Re_hf * delta0);
    deltax_o = line->skindepth * Ro_dc * line->subs->tmet / (Ro_hf * delta0);
    
    /*
     * and plug into the resistance equation which should give the
     * correct answer when well into either region and a decent answer
     * in the in between region.
     */
    
    line->Rev  = Re_dc * line->subs->tmet / (deltax_e * (1.0 - exp(-line->subs->tmet / deltax_e)));
    line->Rodd = Ro_dc * line->subs->tmet / (deltax_o * (1.0 - exp(-line->subs->tmet / deltax_o)));
    
#ifdef DEBUG_CALC
    printf("Rev   = %g Ohms\n", line->Rev);
    printf("Re_dc = %g Ohms\n", Re_dc);
    printf("Re_hf = %g Ohms\n", Re_hf * delta0 / line->skindepth);

    printf("Rod   = %g Ohms\n", line->Rodd);
    printf("Ro_dc = %g Ohms\n", Ro_dc);
    printf("Ro_hf = %g Ohms\n", Ro_hf * delta0 / line->skindepth);
#endif

  } else {
    line->Rev  = Re_dc;
    line->Rodd = Ro_dc;
  }

  /*
   * Apply the microstrip equation for surface roughness correction.
   *
   * FIXME -- it would be nice to see if this is even close to right
   * for coupled stripline....
   */

  if( line->freq > 0.0 ) {
    line->Rev  = line->Rev  * (1.0 + (2.0/M_PI)*atan(1.4*pow((line->subs->rough / line->skindepth),2.0)));
    line->Rodd = line->Rodd * (1.0 + (2.0/M_PI)*atan(1.4*pow((line->subs->rough / line->skindepth),2.0)));
  }

  /* incremental circuit model */
  line->Lev = line->z0e * sqrt(line->subs->er) / LIGHTSPEED;
  line->Cev = sqrt(line->subs->er) / (line->z0e * LIGHTSPEED);
  line->Lodd = line->z0o * sqrt(line->subs->er) / LIGHTSPEED;
  line->Codd = sqrt(line->subs->er) / (line->z0o * LIGHTSPEED);

  /* incremental conductance */
  line->Gev  = 2.0 * M_PI * line->freq * line->Cev  * line->subs->tand;
  line->Godd = 2.0 * M_PI * line->freq * line->Codd * line->subs->tand;

  /* dielectric loss in nepers/meter  --  same for both even and odd modes */
  ld = M_PI * line->freq * line->subs->tand * sqrt(line->subs->er) / LIGHTSPEED;

  /* copper loss in nepers/meter */
  lce = line->Rev  / (2.0 * line->z0e);
  lco = line->Rodd / (2.0 * line->z0e);

  /* total losses in dB/meter */
  db_per_np = 20.0*log10(exp(1.0));
  line->losslen_ev  = (ld + lce) * db_per_np;
  line->losslen_odd = (ld + lco) * db_per_np;

  line->loss_ev  = line->losslen_ev  * line->l;
  line->loss_odd = line->losslen_odd * line->l;

  /* electrical length */
  line->len = 360.0 * line->l * line->freq * sqrt(line->subs->er) / LIGHTSPEED;

  return 0;
}


int coupled_stripline_syn(coupled_stripline_line *line, double f)
{

  double h, er, l, wmin, wmax, abstol, reltol;
  int maxiters;
  double z0, w;
  int iters;
  int done;
  double len;

  double s, smin, smax, z0e, z0o, k;
  double loss, delta, cval, err, d;

  double AW,F1,F2,F3;

  double ai[] = {1, -0.301, 3.209, -27.282, 56.609, -37.746};
  double bi[] = {0.020, -0.623, 17.192, -68.946, 104.740, -16.148};
  double ci[] = {0.002, -0.347, 7.171, -36.910, 76.132, -51.616};

  int i;
  double dw, ds;
  double ze0=0, ze1, ze2, dedw, deds;
  double zo0=0, zo1, zo2, dodw, dods;

#ifdef DEBUG_SYN
  printf("coupled_stripline_syn(): -------- Coupled_Stripline Synthesis ----------\n");
  printf("coupled_stripline_syn(): Metal width                 = %g %s\n",
	 line->w/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_syn(): Metal spacing               = %g %s\n",
	 line->s/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_syn(): Metal thickness             = %g %s\n",
	 line->subs->tmet/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_syn(): Metal relative resistivity  = %g %s\n",
	 line->subs->rho/line->units_rho->sf, line->units_rho->name);
  printf("coupled_stripline_syn(): Metal surface roughness     = %g %s-rms\n",
	 line->subs->rough/line->units_rough->sf, line->units_rough->name);
  printf("coupled_stripline_syn(): Substrate thickness         = %g %s\n",
	 line->subs->h/line->units_lwst->sf, line->units_lwst->name);
  printf("coupled_stripline_syn(): Substrate dielectric const. = %g \n",
	 line->subs->er);
  printf("coupled_stripline_syn(): Substrate loss tangent      = %g \n",
	 line->subs->tand);
  printf("coupled_stripline_syn(): Frequency                   = %g %s\n",
	 line->freq/line->units_freq->sf, line->units_freq->name);
  printf("coupled_stripline_syn(): -------------- ---------------------- ----------\n");
  printf("coupled_stripline_syn(): Desired Zo                  = %g ohm\n", line->z0);
  printf("coupled_stripline_syn(): Desired k                   = %g \n", line->k);
  printf("coupled_stripline_syn(): Desired Even Mode Zo        = %g ohm\n", line->z0e);
  printf("coupled_stripline_syn(): Desired Odd Mode Zo         = %g ohm\n", line->z0o);
  printf("coupled_stripline_syn(): Desired electrical length   = %g degrees\n",line->len);
  printf("coupled_stripline_syn(): -------------- ---------------------- ----------\n");
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
  line->l = l;


  /* FIXME - change limits to be normalized to substrate thickness */
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
   * Initial guess at a solution -- FIXME:  This is an initial guess
   * for coupled microstrip _not_ coupled stripline.
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
  printf("coupled_stripline_syn():  AW=%g, F1=%g, F2=%g, F3=%g\n",
	 AW, F1, F2, F3);
  
  printf("coupled_stripline_syn():  Initial estimate:\n"
	     "                w = %g %s, s = %g %s\n", 
	     w/line->units_lwst->sf, line->units_lwst->name,
	     s/line->units_lwst->sf, line->units_lwst->name);
#endif

  l=100;
  loss=0;


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
      coupled_stripline_calc(line, line->freq);
      
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
	coupled_stripline_calc(line, line->freq);
	ze1 = line->z0e;
	zo1 = line->z0o;

	line->w = w;
	line->s = s + delta;
	coupled_stripline_calc(line, line->freq);
	ze2 = line->z0e;
	zo2 = line->z0o;

	dedw = (ze1 - ze0)/delta;
	dodw = (zo1 - zo0)/delta;
	deds = (ze2 - ze0)/delta;
	dods = (zo2 - zo0)/delta;
	
	/* find the determinate */
	d = dedw*dods - deds*dodw;
	
	/* 
	 * estimate the new solution, but don't change by more than
	 * 10% at a time to avoid convergence problems
	 */
	dw = -1.0 *  ((ze0-z0e)*dods - (zo0-z0o)*deds)/d;
	if( fabs(dw) > 0.1*w ) {
	  if( dw > 0.0 )
	    dw = 0.1 * w;
	  else
	    dw = -0.1 * w;
	}
	w = fabs(w + dw);

	ds =         ((ze0-z0e)*dodw - (zo0-z0o)*dedw)/d;
	if( fabs(ds) > 0.1*s ) {
	  if( ds > 0.0 )
	    ds = 0.1 * s;
	  else
	    ds = -0.1 * s;
	}
	s = fabs(s + ds);

#ifdef DEBUG_SYN
	printf("coupled_stripline_syn():  delta = %g, determinate = %g\n", delta, d);
	printf("coupled_stripline_syn():  ze0 = %16.8g,  ze1 = %16.8g,  ze2 = %16.8g\n",
	       ze0, ze1, ze2);
	printf("coupled_stripline_syn():  zo0 = %16.8g,  zo1 = %16.8g,  zo2 = %16.8g\n",
	       zo0, zo1, zo2);
	printf("coupled_stripline_syn(): dedw = %16.8g, dodw = %16.8g\n",
	       dedw, dodw);
	printf("coupled_stripline_syn(): ze1-ze0 = %16.8g, ze2-ze0 = %16.8g\n",
	       ze1-ze0, ze2-ze0);
	printf("coupled_stripline_syn(): deds = %16.8g, dods = %16.8g\n",
	       deds, dods);
	printf("coupled_stripline_syn(): zo1-zo0 = %16.8g, zo2-zo0 = %16.8g\n",
	       zo1-zo0, zo2-zo0);
	printf("coupled_stripline_syn(): dw = %g %s, ds = %g %s\n",
	       dw/line->units_lwst->sf, line->units_lwst->name,
	       ds/line->units_lwst->sf, line->units_lwst->name);
	printf("-----------------------------------------------------\n");
#endif
      }
    }

  line->w = w;
  line->s = s;
  coupled_stripline_calc(line, line->freq);

  /* scale the line length to get the desired electrical length */
  line->l = line->l * len/line->len;
  coupled_stripline_calc(line, line->freq);

#ifdef DEBUG_SYN
  printf("Took %d iterations, err = %g\n", iters, err);
  printf("ze = %g\tzo = %g\tz0e = %g\tz0o = %g\n", ze0, zo0, z0e, z0o);
#endif

  return(0);
}


/* Zero thickness characteristic impedance */
static int z0_zerot(double w, double s, double b, double er,
		    double *z0e, double *z0o)
{
  double ke, ko;

  /* (3) from Cohn */
  ke = tanh( M_PI*w / (2.0*b) ) * tanh( M_PI*(w + s) / (2.0*b) );

  /* (6) from Cohn */
  ko = tanh( M_PI*w / (2.0*b) ) * coth( M_PI*(w + s) / (2.0*b) );
  

  /* (2) from Cohn */
  *z0e = (FREESPACEZ0 / 4.0) * sqrt(1.0/er) / k_over_kp( ke );

  /* (5) from Cohn */
  *z0o = (FREESPACEZ0 / 4.0) * sqrt(1.0/er) / k_over_kp( ko );
    
  return 0;
}

/*
 * Impedance only calculation (used for incremental inductance loss
 * calculations) 
 */
static int find_z0(coupled_stripline_line *line)
{
  /* zero thickness even and odd impedances */
  double z0e_0t, z0o_0t;

  /* single stripline */
  stripline_line single;
  double z0s, z0s_0t;

  double cf_t, cf_0;

  int rslt;

  /*
   * Start of coupled stripline calculations
   */

  /* zero thickness coupled line */
  z0_zerot( line->w, line->s, line->subs->h,
	    line->subs->er, &z0e_0t, &z0o_0t);


  single.subs = stripline_subs_new();
  *(single.subs) = *(line->subs);
  single.w = line->w;
  single.l = line->l;
  single.freq = line->freq;

  rslt = stripline_calc(&single, line->freq);
  if( rslt != 0 ) {
    alert ("%s():  stripline_calc failed (%d)", __FUNCTION__);
  }
  z0s = single.z0;

  single.subs->tmet = 0.0;
  rslt = stripline_calc(&single, line->freq);
  if( rslt != 0 ) {
    alert ("%s():  stripline_calc failed (%d)", __FUNCTION__);
  }
  z0s_0t = single.z0;


  /* fringing capacitance (uF / cm) */
  cf_t = (0.885 * line->subs->er / M_PI) * (
	(2.0 / (1.0 - line->subs->tmet / line->subs->h)) *
		log( (1.0/(1.0 - line->subs->tmet / line->subs->h)) + 1.0) -
	(1.0 / (1.0 - line->subs->tmet / line->subs->h) - 1.0) *
		log( (1.0/pow(1.0 - line->subs->tmet / line->subs->h, 2.0)) - 1.0) );

  /* zero thickness fringing capacitance (uF / cm) */
  cf_0 = (0.885 * line->subs->er / M_PI) * 2.0 * log(2.0);


  line->z0e = 1.0 / ( (1.0 / z0s) - (cf_t / cf_0) * ( (1.0 / z0s_0t) - (1.0 / z0e_0t) ) );
  line->z0o = 1.0 / ( (1.0 / z0s) - (cf_t / cf_0) * ( (1.0 / z0o_0t) - (1.0 / z0s_0t) ) );

  printf ("zero thickness:  z0e = %6.3f Ohms, z0o = %6.3f Ohms\n", z0e_0t, z0o_0t);
  printf ("real thickness:  z0e = %6.3f Ohms, z0o = %6.3f Ohms\n", line->z0e, line->z0o);

  /*
   * find impedance and coupling coefficient
   */
  line->z0  = sqrt(line->z0e * line->z0o);

  /* coupling coefficient */
  line->k = (line->z0e - line->z0o)/(line->z0e + line->z0o);

  line->deltale = 0.0;
  line->deltalo = 0.0;
  
  /* 
   * electrical length = 360 degrees * physical length / wavelength 
   * 
   * freq * wavelength = velocity => 1/wavelength = freq / velocity
   *
   * 1/wavelength = freq * LIGHTSPEED/sqrt(keff)
   */
  line->len = 360.0 * line->l * line->freq * LIGHTSPEED / sqrt(line->subs->er);
  
  return 0;
}

void coupled_stripline_line_free(coupled_stripline_line * line)
{
  free(line->subs);
  wc_units_free(line->units_lwst);
  wc_units_free(line->units_L);
  wc_units_free(line->units_R);
  wc_units_free(line->units_C);
  wc_units_free(line->units_G);
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


coupled_stripline_line *coupled_stripline_line_new()
{
  coupled_stripline_line *newline;

  newline = (coupled_stripline_line *) malloc(sizeof(coupled_stripline_line));
  if(newline == NULL)
    {
      fprintf(stderr,"coupled_stripline_line_new: malloc() failed\n");
      exit(1);
    }

  newline->subs = stripline_subs_new();

  /* create the units */
  newline->units_lwst    = wc_units_new(WC_UNITS_LENGTH);
  newline->units_L       = wc_units_new(WC_UNITS_INDUCTANCE_PER_LEN);
  newline->units_R       = wc_units_new(WC_UNITS_RESISTANCE_PER_LEN);
  newline->units_C       = wc_units_new(WC_UNITS_CAPACITANCE_PER_LEN);
  newline->units_G       = wc_units_new(WC_UNITS_CONDUCTANCE_PER_LEN);
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
  coupled_stripline_load_string(newline, default_coupled_stripline);

  /* and do a calculation to finish the initialization */
  coupled_stripline_calc(newline, newline->freq);

  return(newline);
}

