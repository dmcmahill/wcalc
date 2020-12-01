/*
 * Copyright (C) 2020 Dan McMahill
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

/* #define DEBUG_CALC */  /* debug the rods_calc() function */
/* #define DEBUG_SYN  */  /* debug the rods_syn() function  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "rods.h"
#include "rods_loadsave.h"
#include "alert.h"
#include "defaults.h"
#include "mathutil.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 *   Reference:
 *
 *
 *  The two circular cross section rods have their major axes parallel to each
 *  other and running along the Z-axis. 
 *
 *  Rod #1:
 *     diameter             = d1
 *     length (z direction) = l1
 *
 * Bar #2:
 *     diameter             = d2
 *     length (z direction) = l2
 *
 * The distance is defined as the distance between the centers of the projections onto
 * the x-y plane and the offset is the difference in z-axis starting point.
 *
 * NOTE:  Currently this only supports d1=d2, l1=l2, and offset=0.  I.e. the two wires
 * are identical and occupy the same z-axis range.
 * 
 * Primary reference:
 *
 * H. A. Aebischer and B. Aebischer, "Improved Formulae for the Inductance of Straight Wires",
 * Advanced Electromagnetics, Vol. 3, No. 1, September 2014, pp 31-43
 *
 * Note that other common results make assumuptions about long wires that have varying degrees of
 * error at shorter lengths.
 *
 * Other common expressions but less accurate may be found in
 *
 * E. B. Rosa, "The Self and Mutual Inductances of Linear Conductors", Bulletin of the Bureau of
 * Standards, vol. 4, no. 2, Washington, 1907.
 *
 * C. R. Paul, "Inductance", John Wiley & Sons, Hoboken NJ, 2010.
 *
 * R. W. P. King and S. Prasad, "Fundamental Electomagnetic Theory and Applications", prentice Hall,
 * Englewood Cliffs, NJ, 1986.
 *
 */



/*
 * l = length [meters]
 * R = radius [meters]
 *
 * L = self inductance [Henries]
 */
static double Lself(double l, double R)
{
  double L, omega, k, deltaL;


  /*
   * Rosa (9) for reference.  The 1e-7 factor is to
   * take meters in and give H out.
   *
   *
   * Lrosa = 2e-7*(l*log( (l + sqrt(l*l + R*R)) / R) - sqrt(l*l + R*R) + 0.25*l + R);
   *
   */

  /*
   * Aebischer (34)
   * Note that the only actual difference compared to Rosa's equation is the factor
   * of 0.905415 that Aebischer obtained
   * through numerical integration.  It is striking that this seemingly small adjustment
   * makes a big difference.  The Rosa formula has 10% error around length/radius = 2 or 2.5
   * and just under 2% at length/radius = 5.  In contrast the Aebisher (34) result has
   * an error of -0.1% (vs 10%) at length/radius ~ 2 or so and rapidly approaches 0 as
   * length/radius increases.
   */
  L = 2e-7*(l*log(sqrt(l*l + R*R) + l) - l*(log(R) - 0.25) - sqrt(l*l + R*R) + 0.905415*R);

  /* High frequency inductance, Aebischer (35) */
  /* Lhf = 2e-7*(l*log(sqrt(l*l + 2*R*R) + l) - l*log(R) - sqrt(l*l + 2*R*R) + (4.0/M_PI)*R); */

  /* Aebischer (49) */
  omega = sqrt(l*l + R*R);

  /* Aebischer (51) */
  k = 0.094585;

  /* Aebischer (52) */
  deltaL = 2e-7*k*R*R * (omega - l) / (omega*(omega + l));

  /* with this correction, the error is less than about 0.018% for length/radius >= 2 */
  L = L + deltaL;

  return L;
}

/*
 * NOTE:  This function is limited to d1=d2=2*R, l1=l2, offset=0.
 * Once that limitation is addressed here the rest of the module will
 * be in good shape.
 *
 * l = length [meters]
 * d = distance between centers [meters]
 * R = wire radius
 *
 */
static double Lmutual(double l, double d, double R)
{
  double M, w, deltaM;

  /*
   * Rosa (12)
   * Note:  This equation assumes that the lengths area identical and that
   * the wires are lined up in the z-axis (occupy the same range in z)
   *
   * In addition, it basically assumes that R=0 (i.e. it is the mutual inductance
   * between two filaments)
   * Mrosa = 2e-7*(l*log( (l + sqrt(l*l + d*d)) / d) - sqrt(l*l + d*d) + d);
   */

  /* Aebischer (63) */
  w = sqrt(l*l + d*d + R*R);

  /* Aebischer (63) */
  M = 2e-7*(l*log(w + l) - l*log(d) - w + d + R*R/(4.0*d));

  /* Aebischer (62) */
  deltaM = 2e-7*sqrt(d*d + R*R) * (sqrt(d*d + R*R) - d - R*R/(4.0*d)) * ((w - l) / (w*(w + l)));

  M = M + deltaM;

  return M;
}


int rods_calc(rods *b, double freq)
{
    double r1, r2;
    int touching = 1;

    if(freq < 0.0) {
	alert(_("Frequency must be >= 0"));
	return -1;
    }

    /* check that none of our individual bar dimensions are negative */
    if( b->d1 < 0 || b->d2 < 0 ) {
	alert(_("The rod diameters, d1 and d2, must both be >= 0"));
	return -1;
    }

    if(b->l1 <= 0 || b->l2 <= 0) {
	alert(_("The rod lengths, l1 and l2, must be > 0"));
    }

    /* now see if the rods are touching. */
    r1 = 0.5*b->d1;
    r2 = 0.5*b->d2;

    /* see if we have overlap in the circles when we project to a plane */
    if( (r1 + r2) < b->distance) {
	touching = 0;
    }
    
    /* see if we have overlap in the direction parallel to the rods */
    if( (b->offset < -1*b->l2) || (b->offset > b->l1) ) {
	touching = 0;
    }
    
    /* for the rods to touch, we have to have overlap in both dimensions */

    if(touching) {
	alert(_("The rods are touching.  This is not allowed."));
	return -1;
    }

  b->L1 = Lself(b->l1, r1);
  b->L2 = Lself(b->l2, r2);
  b->M = Lmutual(b->l1, b->distance, r1);
  b->k = b->M / sqrt(b->L1 * b->L2);

  b->R1 = b->rho*b->l1/(M_PI*r1*r1);
  b->R2 = b->rho*b->l2/(M_PI*r2*r2);

#ifdef DEBUG_CALC
  printf("%s ----------------------\n", __FUNCTION__);
  printf("%s Input values:\n", __FUNCTION__);
  printf("%s ----------------------\n", __FUNCTION__);

  
  printf("%s  Rod #1 dimensions:\n", __FUNCTION__);
  printf("%s  d1   = %g\n", __FUNCTION__, b->d1);
  printf("%s  l1   = %g\n", __FUNCTION__, b->l1);

  printf("%s  ----------------------\n", __FUNCTION__);
  printf("%s  Rod #2 dimensions:\n", __FUNCTION__);
  printf("%s  d2   = %g\n", __FUNCTION__, b->d2);
  printf("%s  l2   = %g\n", __FUNCTION__, b->l2);

  
  printf("%s  ----------------------\n", __FUNCTION__);
  printf("%s  Bar #2 radial distance and axial offset:\n", __FUNCTION__);
  printf("%s  distance = %g\n", __FUNCTION__, b->distance);
  printf("%s  offset   = %g\n", __FUNCTION__, b->offset);

  printf("%s  ----------------------\n", __FUNCTION__);
  printf("%s  freq = %g MHz\n", __FUNCTION__, b->freq/1e6);
  printf("%s  ----------------------\n",  __FUNCTION__);
#endif


#ifdef DEBUG_CALC
  printf("%s  ----------------------\n",  __FUNCTION__);
  printf("%s  Calculated L1 = %g nH\n",  __FUNCTION__, b->L1*1e9);
  printf("%s  Calculated L2 = %g nH\n",  __FUNCTION__, b->L2*1e9);
  printf("%s  Calculated M  = %g nH\n",  __FUNCTION__, b->M*1e9);
  printf("%s  Calculated k  = %g\n",  __FUNCTION__, b->k);
  printf("%s  Calculated R1 = %g Ohm\n",  __FUNCTION__, b->R1);
  printf("%s  Calculated R2 = %g Ohm\n",  __FUNCTION__, b->R2);
  printf("%s  ----------------------\n");
#endif

  return 0;
}

/*
 * 
 */

int rods_syn(rods *b, double f, int flag)
{
  alert (_("rods_syn() -- not yet implemented\n"));
  return -1;


  return 0;
}


void rods_free(rods *b)
{

  wc_units_free(b->units_xy);
  wc_units_free(b->units_L);
  wc_units_free(b->units_R);
  wc_units_free(b->units_rho);
  wc_units_free(b->units_freq);

  free(b);
}


rods *rods_new()
{
  rods *newb;

  newb = (rods *) malloc(sizeof(rods));
  if(newb == NULL)
    {
      fprintf(stderr,"%s: malloc() failed\n", __FUNCTION__);
      exit(1);
    }

  /* Create the units */
  newb->units_xy = wc_units_new(WC_UNITS_LENGTH);
  newb->units_L = wc_units_new(WC_UNITS_INDUCTANCE);
  newb->units_R = wc_units_new(WC_UNITS_RESISTANCE);
  newb->units_freq = wc_units_new(WC_UNITS_FREQUENCY);
  newb->units_rho  = wc_units_new(WC_UNITS_RESISTIVITY);

  /* load in the defaults */
  rods_load_string(newb, default_rods);

  /* get the rest of the entries in sync */
  rods_calc(newb, newb->freq);

  return(newb);
}


