/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2006, 2009, 
 *               2011, 2012, 2020, 2021 Dan McMahill
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

/* debug coupled_stripline_syn()  */
/* #define DEBUG_SYN  */

/* debug coupled_stripline_calc() */
/* #define DEBUG_CALC */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "alert.h"
#include "defaults.h"
#include "mathutil.h"
#include "messages.h"
#include "misc.h"
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
 *    S. B. Cohn, "Characteristic Impedance of the Shielded-Strip
 *    Transmission Line", IRE Transactions on Microwave Theory and
 *    Techniques, Vol. MTT-2, pp. 52-57, July, 1954.
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


  if( (line->freq > 0.0) && (line->subs->tmet > 0.0) ) {
    /* 
     * Find the high frequency asymptotic behaviour of the resistance
     * using Wheelers incremental inductance rule at a frequency where
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
    if(rslt != 0) {
      alert(_("Failed step 1 of Wheeler's incremental inductance analysis.\n"));
      alert_bug();
    }
    z1e = tmp_line.z0e;
    z1o = tmp_line.z0o;

    /* skindepth at which we'll do the following analysis */
    delta0 = 0.1*line->subs->tmet;

    tmp_line.w = line->w - delta0;
    tmp_line.s = line->s + delta0;
    tmp_line.subs->tmet = line->subs->tmet - delta0;
    tmp_line.subs->h = line->subs->h + delta0;

    rslt = find_z0(&tmp_line);
    if(rslt != 0) {
      alert(_("Failed step 2 of Wheeler's incremental inductance analysis.\n"));
      alert_bug();
    }

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


  if( line->subs->tmet > 0.0 ) {
    /* DC resistance */
    Re_dc = 1.0 / (line->w * line->subs->tmet * sigma);
    Ro_dc = 1.0 / (line->w * line->subs->tmet * sigma);
  } else {
    /* if we have zero metal thickness, just shut off conductor loss */
    Re_dc = 0.0;
    Ro_dc = 0.0;
  }
  
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

  if( (line->freq > 0.0) && (line->subs->tmet > 0.0) ) {
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

  /* 
   * delay on line.
   *
   * delay = length / velocity
   */
   line->delay = line->l * sqrt(line->subs->er) / LIGHTSPEED;

  return 0;
}


int coupled_stripline_syn(coupled_stripline_line *line, double f)
{

  double l;
  int maxiters;
  double z0, w;
  int iters;
  int done;
  double len;

  double s, z0e, z0o, k;
  double loss, delta, cval, err, d;

  double dw, ds;
  double ze0=0, ze1, ze2, dedw, deds;
  double zo0=0, zo1, zo2, dodw, dods;

  stripline_line *single;
  int rslt;

  len = line->len;

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
  printf("coupled_stripline_syn(): Desired Zo                  = %g ohm\n", z0);
  printf("coupled_stripline_syn(): Desired k                   = %g \n", k);
  printf("coupled_stripline_syn(): Desired Even Mode Zo        = %g ohm\n", z0e);
  printf("coupled_stripline_syn(): Desired Odd Mode Zo         = %g ohm\n", z0o);
  printf("coupled_stripline_syn(): Desired electrical length   = %g degrees\n",line->len);
  printf("coupled_stripline_syn(): -------------- ---------------------- ----------\n");
#endif


  /* temp value for l used while finding w and s */
  l = 1000.0;
  line->l = l;


  /* 
   * FIXME -- should I put in some limits that clamp the range for w
   * and s during synthesis?
   */
  
  /* limits on the allowed range for w */
  /* wmin = 0.01*line->subs->h; */
  
  /* limits on the allowed range for s */


  maxiters = 50;


  /*
   * As an initial guess use the following approach.  The impedance of
   * a single trace is typically not too far from sqrt(z0e*z0o) so
   * pick the starting width based on a single line.  Then choose a
   * gap which is comparable to the width.  It is not horribly
   * accurate, but we typically don't need many iterations and each
   * iteration is fairly fast on any computer from the last 15 years.
   */
#ifdef DEBUG_SYN
  printf("%s():  Creating single stripline for initial guess...\n", __FUNCTION__);
#endif
  single = stripline_line_new();
  *(single->subs) = *(line->subs);
  single->z0 = z0;
  single->Ro = z0;
  single->freq = line->freq;

#ifdef DEBUG_SYN
  printf("%s(): single->z0 = %g\n", __FUNCTION__, single->z0);
#endif
  rslt = stripline_syn(single, single->freq, SLISYN_W);
  if(rslt != 0) {
    alert(_("Synthesis of a single stripline to use as an initial condition failed.\n"));
    alert_bug();
  }
  w = single->w;
#ifdef DEBUG_SYN
  printf("%s(): rslt = %d, single->z0 = %g\n", __FUNCTION__, rslt, single->z0);
#endif
  s = w;
  stripline_line_free(single);
#ifdef DEBUG_SYN
  printf("coupled_stripline_syn():  Initial estimate:\n"
	     "                w = %g %s, s = %g %s\n", 
	     w/line->units_lwst->sf, line->units_lwst->name,
	     s/line->units_lwst->sf, line->units_lwst->name);
#endif

  l=100;
  loss=0;


  iters = 0;
  done = 0;

  /* FIXME -- think about calculating a new delta based on this at
     each iteration */
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

	ds = ((ze0-z0e)*dodw - (zo0-z0o)*dedw)/d;
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


/* 
 * Zero thickness characteristic impedance 
 *
 * w = width of trace
 * s = gap between traces
 * b = dielectric thickness
 */
static int z0_zerot(double w, double s, double b, double er,
		    double *z0e, double *z0o)
{
  double ke, ko;

  /* (3) from Cohn */
  ke = tanh( M_PI*w / (2.0*b) ) * tanh( M_PI*(w + s) / (2.0*b) );

  /* (6) from Cohn */
  ko = tanh( M_PI*w / (2.0*b) ) * coth( M_PI*(w + s) / (2.0*b) );
  

  /*
   * some observations here...
   *
   * ke * ko = [tanh(M_PI*w/(2.0*b))]^2 always
   *
   * As w/b gets large, tanh() and coth() both tend towards 1.0
   * and we end up with ke and ko tending towards 1.0.  This causes
   * issues because K(1)/K'(1) (i.e. k_over_kp(1.0)) becomes infinite.
   *
   * What I should in fact see though is that as the trace width gets
   * very large, the even mode and odd mode impedances come together
   * and approach the single strip impedance.
   *
   * To avoid some of the numerical nastiness in this area
   * for large w/b, we will switch to an alternate formula that is
   * close enough.  In reality, about the only time we may hit this is
   * if we have a marginal guess during some of our iterations for the
   * synthesis.  FIXME -- this part hasn't been implemented yet.  I need
   * to see if this is something I can solve for by hand.
   */

  /* (2) from Cohn */
  *z0e = (FREESPACEZ0 / 4.0) * sqrt(1.0/er) / k_over_kp( ke );

  /* (5) from Cohn */
  *z0o = (FREESPACEZ0 / 4.0) * sqrt(1.0/er) / k_over_kp( ko );

#ifdef DEBUG_CALC
  printf("z0_zerot():  ke = %.16f, ko = %.16f, *z0e = %g, *z0o = %g\n", ke, ko, *z0e, *z0o);
#endif

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
  stripline_line *single;
  double z0s, z0s_0t;

  double cf_t, cf_0;

  double tb;

  int rslt;

  /*
   * Start of coupled stripline calculations
   */

  /* zero thickness coupled line */
  z0_zerot( line->w, line->s, line->subs->h,
	    line->subs->er, &z0e_0t, &z0o_0t);

#ifdef DEBUG_CALC
  printf("find_z0():  z0o_0t = %3.8f Ohms, z0e_0t = %3.8f Ohms\n", z0e_0t, z0o_0t);
#endif

  if( line->subs->tmet == 0.0 ) {
    /* 
     * if metal thickness is zero, then we are done.  Just use the
     * zero thickness result.
     */
    line->z0e = z0e_0t;
    line->z0o = z0o_0t;
  } else {

    /*
     * we have a non-zero line thickness and so we use the
     * approximations to correct for this.
     */
    single = stripline_line_new();
    *(single->subs) = *(line->subs);
    single->w = line->w;
    single->l = line->l;
    single->freq = line->freq;

    rslt = stripline_calc(single, line->freq);
    if( rslt != 0 ) {
      alert ("%s():  stripline_calc failed (%d)", __FUNCTION__, rslt);
      alert_bug();
    }
    z0s = single->z0;

    single->subs->tmet = 0.0;
    rslt = stripline_calc(single, line->freq);
    if( rslt != 0 ) {
      alert ("%s():  stripline_calc failed (%d)", __FUNCTION__, rslt);
      alert_bug();
    }
    z0s_0t = single->z0;

    stripline_line_free(single);

#ifdef DEBUG_CALC
    printf("find_z0():  z0s = %g, z0s_0t = %g\n", z0s, z0s_0t);
#endif

    /* 
     * Just before (11) in Cohn, there is a statement that says
     * ".... stripes are wide enough to allow the fringing capacitances
     * at the two edges of each strip to be independent of w/b.  This
     * will hold with good accuracy for w/b >= 0.35...".  In the
     * notation of this function, the condition is 
     * line->w / line->subs->h > 0.35 (trace width divided by total
     * dielectric thickness).  Later on at the bottom left of p. 34,
     * it says the best accuracy is for t/b less than about 0.1
     * (i.e. line->subs->tmet / line->subs->h < 0.1
     */

    /* 
     * fringing capacitance from (2) of the 2nd Cohn reference (MTT-2)
     * The text in the paper says that this is usable for
     * w/(b-t) >= 0.35 with a maximum error of
     * 1.2% at the lower limit for W.
     * w = line->w
     * b = line->subs->h
     * t = line->subs->tmet
     */

    tb = line->subs->tmet / line->subs->h;
    cf_t = (FREESPACE_E0 * line->subs->er / M_PI) * (
						     (2.0 / (1.0 - tb)) *
						     log( (1.0/(1.0 - tb)) + 1.0) -
						     (1.0 / (1.0 - tb) - 1.0) *
						     log( (1.0/pow(1.0 - tb, 2.0)) - 1.0) );

    /*
     * zero thickness fringing capacitance .  (13) from Cohn.  This is
     * the fringing capacitance from one edge to one ground plane for
     * a single strip of zero thickness.  
     * 
     * C'(0) in the notation of Cohn.
     *  f
     */
    cf_0 = (FREESPACE_E0 * line->subs->er / M_PI) * 2.0 * log(2.0);


    /* (18) from Cohn, (4.6.5.1) in Wadell */
    line->z0e = 1.0 / ( (1.0 / z0s) - (cf_t / cf_0) * ( (1.0 / z0s_0t) - (1.0 / z0e_0t) ) );
#ifdef DEBUG_CALC
    printf("find_z0():  cf_t = %g, cf_0 = %g, z0e = %g\n", cf_t, cf_0, line->z0e);
#endif

    if( line->s >= 5.0 * line->subs->tmet) {
      /*
       * (20) from Cohn, (4.6.5.2) in Wadell -- note, Wadell has a sign
       * error in the equation 
       */
      line->z0o = 1.0 / ( (1.0 / z0s) + (cf_t / cf_0) * ( (1.0 / z0o_0t) - (1.0 / z0s_0t) ) );
#ifdef DEBUG_CALC
      printf("find_z0():  Using s > 5 tmet case => z0o = %g\n", line->z0o);
#endif
    } else {
      /*
       * (22) from Cohn, (4.6.5.3) in Wadell -- note, Wadell has a
       * couple of errors in the transcription from the original (Cohn)
       */
      line->z0o = 1.0 / ( (1.0 / z0o_0t) +
			  ( (1.0 / z0s) - (1.0 / z0s_0t) ) -
			  (2.0 / FREESPACEZ0) * (cf_t/FREESPACE_E0 - cf_0/FREESPACE_E0) +
			  (2.0 * line->subs->tmet) / (FREESPACEZ0 * line->s)
			  );
#ifdef DEBUG_CALC
      printf("find_z0():  Using s < 5 tmet case => z0o = %g\n", line->z0o);
#endif
    }
  }

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

