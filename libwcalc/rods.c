
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
 * p = radius [meters]
 *
 * L = self inductance [Henries]
 */
static double Lself(double l, double p)
{
  double Lrosa, L, omega, k, deltaL;

  printf("%s:  l = %g, p = %g\n", __FUNCTION__, l, p);

  /*
   * Rosa (9) for reference.  The 1e-7 factor is to
   * take meters in and give H out.
   */
  Lrosa = 2e-7*(l*log( (l + sqrt(l*l + p*p)) / p) - sqrt(l*l + p*p) + 0.25*l + p);

  /*
   * Aebischer (34)
   * Note that the only actual difference is the factor of 0.905415 that Aebischer obtained
   * through numerical integration.  It is striking that this seemingly small adjustment
   * makes a big difference.  The Rosa formula has 10% error around length/radius = 2 or 2.5
   * and just under 2% at length/radius = 5.  In contrast the Aebisher (34) result has
   * an error of -0.1% (vs 10%) at length/radius ~ 2 or so and rapidly approaches 0 as
   * length/radius increases.
   */
  L = 2e-7*(l*log(sqrt(l*l + p*p) + l) - l*(log(p) - 0.25) - sqrt(l*l + p*p) + 0.905415*p);

  printf("Lrosa = %g nH\n", 1e9*Lrosa);
  printf("L34   = %g nH\n", 1e9*L);

  /* Aebischer (49) */
  omega = sqrt(l*l + p*p);

  /* Aebischer (51) */
  k = 0.094585;

  /* Aebischer (52) */
  deltaL = 2e-7*k*p*p * (omega - l) / (omega*(omega + l));
  printf("deltaL = %g nH\n", 1e9*deltaL);

  /* with this correction, the error is less than about 0.018% for length/radius >= 2 */
  L = L + deltaL;
  printf("L     = %g nH\n", 1e9*L);

  return L;
}

static double Lmutual(double l, double d, double r)
{
  double Mrosa, M, w, deltaM;

  /*
   * Rosa (12)
   * Note:  This equation assumes that the lengths area identical and that
   * the wires are lined up in the z-axis (occupy the same range in z)
   */
  Mrosa = 2e-7*(l*log( (l + sqrt(l*l + d*d)) / d) - sqrt(l*l + d*d) + d);

  /* Aebischer (63) */
  w = sqrt(l*l + d*d + r*r);

  /* Aebischer (63) */
  M = 2e-7*(l*log(w + l) - l*log(d) - w + d + r*r/(4.0*d));

  /* Aebischer (62) */
  deltaM = 2e-7*sqrt(d*d + r*r) * (sqrt(d*d + r*r) - d - r*r/(4.0*d)) * ((w - l) / (w*(w + l)));
  printf("Mrosa = %g\n", Mrosa);
  printf("M     = %g\n", M);

  return M;
}


int rods_calc(rods *b, double freq)
{
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

    /* see if we have overlap in the circles when we project to a plane */
    if( 0.5*(b->d1 + b->d2) < b->distance) {
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

  b->L1 = Lself(b->l1, 0.5*b->d1);
  b->L2 = Lself(b->l2, 0.5*b->d2);
  b->M = Lmutual(b->l1, b->distance, 0.5*b->d1);
  b->k = b->M / sqrt(b->L1 * b->L2);

#ifdef DEBUG_CALC
  printf("%s ----------------------\n", __FUNCTION__);
  printf("%s Input values:\n", __FUNCTION__);
  printf("%s ----------------------\n", __FUNCTION__);

  
  printf("%s  Bar #1 dimensions:\n", __FUNCTION__);
  printf("%s  d1   = %g\n", __FUNCTION__, b->d1);
  printf("%s  l1   = %g\n", __FUNCTION__, b->l1);

  printf("%s  ----------------------\n", __FUNCTION__);
  printf("%s  Bar #2 dimensions:\n", __FUNCTION__);
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

#ifdef DEBUG_SYN
  printf("rods_syn():  ----------------------\n");
  printf("rods_syn():  Input values:\n");
  printf("rods_syn():  ----------------------\n");
  printf("rods_syn():  N    = %g\n",coil->Nf);
  printf("rods_syn():  AWG  = %g\n",coil->AWGf);
  printf("rods_syn():  I.D. = %g inches\n",M2INCH(coil->dia));
  printf("rods_syn():  len  = %g inches\n",M2INCH(coil->len));
  printf("rods_syn():  rho  = %g\n",coil->rho);
  printf("rods_syn():  freq = %g MHz\n",coil->freq*1e-6);
  printf("rods_syn():  ----------------------\n");
  printf("rods_syn():  desired inductance  = %g nH\n",L*1e9);
  printf("rods_syn():  \n");
  printf("rods_syn():  ----------------------\n");
#endif

#ifdef FIXME
  error=1.0;
  while(error > 1e-8){
    len1 = len2;
    len2 = len;
    
    coil->len = INCH2M(len1);
    if (rods_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn1     = coil->L;
    
    coil->len = INCH2M(len2);
    if (rods_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn2     = coil->L;
    
  
    len = len2 + (L - Lsyn2)*(len2-len1)/(Lsyn2-Lsyn1);
    error = fabs(len-len2)/len;

#ifdef DEBUG_SYN
    printf("rods_syn():  len=%g\tlen1=%g\tlen2=%g\n",len,len1,len2);
    printf("                 L1=%g\tL2=%g\terror=%g\n",Lsyn1,Lsyn2,error);
#endif
  
    coil->len = INCH2M(len);

    /* fill in the rest of the data */
    if (rods_calc_int(coil,f,CALC_ALL) != 0)
      return -1;

  }
#endif

  return 0;
}


void rods_free(rods *b)
{

  wc_units_free(b->units_xy);
  wc_units_free(b->units_L);
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
  newb->units_freq = wc_units_new(WC_UNITS_FREQUENCY);

  /* load in the defaults */
  rods_load_string(newb, default_rods);

  /* get the rest of the entries in sync */
  rods_calc(newb, newb->freq);

  return(newb);
}


