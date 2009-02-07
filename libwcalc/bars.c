/* $Id: bars.c,v 1.6 2009/01/10 20:56:48 dan Exp $ */

/*
 * Copyright (C) 2008 Dan McMahill
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

/* #define DEBUG_CALC */  /* debug the bars_calc() function */
/* #define DEBUG_SYN  */  /* debug the bars_syn() function  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "bars.h"
#include "bars_loadsave.h"
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
 *     Cletus Hoer and Carl Love, "Exact Inductance Equations for
 *     Rectangular Conductors With Applications to More Complicated
 *     Geometries", Journal of Research of the National Bureau of
 *     Standards -- C. Engineering and Instrumentation, Vol. 69C,
 *     No. 2, April-June 1965.
 *
 *
 *
 *  The two rectangular bars have their major axes parallel to each
 *  other and running along the Z-axis. 
 *
 *  Bar #1:
 *     width  (x direction) = a
 *     height (y direction) = b
 *     length (z direction) = l1
 *     one corner at (0, 0, 0), the other at (a, b, l1)
 *
 * Bar #2:
 *     width  (x direction) = d
 *     height (y direction) = c
 *     length (z direction) = l2
 *     one corner at (E, P, l3), the other at (E+d, P+c, l3+l2)
 *
 */


/* This is the function in (14) in Hoer and Love */
static double Mb_fn(double x, double y, double z)
{

  double rslt = 0.0;
  double mag, mag2;

  /*
  printf("%s(x=%g, y=%g, z=%g)\n", __FUNCTION__, x, y, z);
  */
  mag2 = x*x + y*y + z*z;
  mag = sqrt(mag2);

#if defined(DEBUG_CALC) &&  (DEBUG_CALC > 1)
  printf("%s():  mag = %g\n", __FUNCTION__, mag);
#endif

  /* 
   * we have to watch out for y and z both being equal to zero.
   * in that case we have 0/0
   *
   * Try setting z = y and we have
   *
   * (y^4 / 4.0 - y^4 / 24.0 - y^4 / 24) * x * log (x + m) / sqrt(2
   * y^2)
   * => 0
   */

  if ( (x != 0 && y != 0) ||
       (x != 0 && z != 0) ||
       (y != 0 && z != 0) ) {
    /* at least 2 != 0 */
    rslt += (pow(y,2.0)*pow(z,2.0)/4.0 - pow(y,4.0)/24.0 - pow(z,4.0)/24.0) * x * 
      log( (x + mag) / sqrt( y*y + z*z)) ;
    
    rslt += (pow(x,2.0)*pow(z,2.0)/4.0 - pow(x,4.0)/24.0 - pow(z,4.0)/24.0) * y * 
      log( (y + mag) / sqrt( x*x + z*z));
    
    rslt += (pow(x,2.0)*pow(y,2.0)/4.0 - pow(x,4.0)/24.0 - pow(y,4.0)/24.0) * z * 
      log( (z + mag) / sqrt( x*x + y*y));
    
  }

  if( x!= 0 || y != 0 || z != 0) {
    rslt += (1.0 / 60.0) * (pow(x,4.0) + pow(y,4.0) + pow(z,4.0) - 3*pow(x,2.0)*pow(y,2.0)
			    - 3*pow(y,2.0)*pow(z,2.0) - 3*pow(x,2.0)*pow(z,2.0)) * mag ;
  }

  /* if any of x, y, z are zero the the followng terms are zero */
  if ( x != 0 && y != 0 && z != 0) {
    /* all != 0 */

    rslt -= (x*y*pow(z,3.0)/6.0) * atan2( x*y, z*mag);
      
    rslt -= (x*pow(y,3.0)*z/6.0) * atan2( x*z, y*mag);
    
    rslt -= (pow(x,3.0)*y*z/6.0) * atan2( y*z, x*mag);

  }

  return rslt;
}


/* (14) */
static double Mb(bars *b)
{
  double rslt, tmp;
  double q[5], r[5], s[5];
  unsigned int i, j, k;

  q[1] = b->E - b->a;
  q[2] = b->E + b->d - b->a;
  q[3] = b->E + b->d;
  q[4] = b->E;

  r[1] = b->P - b->b;
  r[2] = b->P + b->c - b->b;
  r[3] = b->P + b->c;
  r[4] = b->P;

  s[1] = b->l3 - b->l1;
  s[2] = b->l3 + b->l2 - b->l1;
  s[3] = b->l3 + b->l2;
  s[4] = b->l3;

#if 0
  for(i=1; i<=4; i++) {
    printf("%s():  q[%d] = %g\n", __FUNCTION__, i, q[i]);
  }

  for(i=1; i<=4; i++) {
    printf("%s():  r[%d] = %g\n", __FUNCTION__, i, r[i]);
  }

  for(i=1; i<=4; i++) {
    printf("%s():  s[%d] = %g\n", __FUNCTION__, i, s[i]);
  }
#endif

  rslt = 0.0;
  for(i=1 ; i<=4 ; i++) {
    for(j=1 ; j<=4 ; j++) {
      for(k=1 ; k<=4 ; k++) {
	tmp = pow(-1.0, (double) (i + j + k + 1)) * 
	  Mb_fn(q[i], r[j], s[k]);
	rslt += tmp;
#ifdef DEBUG_CALC
	printf("[%d %d %d] [%g %g %g] : %.8g  -------> %.8g\n", 
	       i-1, j-1, k-1, q[i], r[j], s[k], tmp, rslt);
#endif
      }
    }
  }
  
  /*
   *  the 1e-7 differs from the 0.001 in the paper so I get a function
   * that takes meters in and gives Henries out instead of cm in and
   * microhenries out
   */
  rslt = (1e-7 / (b->a * b->b * b->c * b->d)) * rslt;

  return rslt;
}

/* (20) */
static double Lb(bars *b, int w)
{
  double rslt;
  double q[3], r[3], s[3];
  unsigned int i, j, k;
  double a, bb;

  if (w == 1) {
    q[1] = b->a;
    r[1] = b->b;
    s[1] = b->l1;
    a = b->a;
    bb = b->b;
  } else if (w==2) {
    q[1] = b->c;
    r[1] = b->d;
    s[1] = b->l2;
    a = b->d;
    bb = b->c;
  } else {
    fprintf(stderr, "bars.c: Lb() - w = %d is not allowed.  Internal error.\n", w);
    exit(1);
  }

  q[2] = 0.0;
  r[2] = 0.0;
  s[2] = 0.0;

  rslt = 0.0;
  for(i=1 ; i<=2 ; i++) {
    for(j=1 ; j<=2 ; j++) {
      for(k=1 ; k<=2 ; k++) {
	rslt += pow(-1.0, (double)(i + j + k + 1)) * Mb_fn(q[i], r[j], s[k]);
      }
    }
  }

  /*
   *  the 8e-7 differs from the 0.008 in the paper so I get a function
   * that takes meters in and gives Henries out instead of cm in and
   * microhenries out
   */
  rslt = (8e-7 / (a*a*bb*bb)) * rslt;

  return rslt;
}



int bars_calc(bars *b, double freq)
{
    int touching = 1;

    if(freq < 0.0) {
	alert(_("Frequency must be >= 0"));
	return -1;
    }

    /* check that none of our individual bar dimensions are negative */
    if( b->b < 0 || b->a < 0 || b->c < 0 || b->d < 0) {
	alert(_("a, b, d, c must all be >= 0"));
	return -1;
    }

    if(b->l1 <= 0 || b->l2 <= 0) {
	alert(_("l1 and l2 must be > 0"));
    }

    /* now see if the bars are touching. */

    /* see if we have overlap in the X-dimension */
    if( (b->E > b->a) || (b->E + b->d < 0) ) {
	touching = 0;
    }
    
    /* see if we have overlap in the Y-dimension */
    if( (b->P > b->b) || (b->P + b->c < 0) ) {
	touching = 0;
    }
    
    /* see if we have overlap in the Z-dimension */
    if( (b->l3 > b->l1) || (b->l3 + b->l2 < 0) ) {
	touching = 0;
    }

    /* for the bars to touch, we have to have overlap in all 3 dimensions */

    if(touching) {
	alert(_("The bars are touching.  This is not allowed."));
	return -1;
    }

  b->L1 = Lb(b, 1);
  b->L2 = Lb(b, 2);
  b->M = Mb(b);
  b->k = b->M / sqrt(b->L1 * b->L2);

#ifdef DEBUG_CALC
  printf("bars_calc():  ----------------------\n");
  printf("bars_calc():  Input values:\n");
  printf("bars_calc():  ----------------------\n");

  
  printf("bars_calc():  Bar #1 x-y-z dimensions:\n");
  printf("bars_calc():  a    = %g\n", b->a);
  printf("bars_calc():  b    = %g\n", b->b);
  printf("bars_calc():  l1   = %g\n", b->l1);

  printf("bars_calc():  ----------------------\n");
  printf("bars_calc():  Bar #2 x-y-z dimensions:\n");
  printf("bars_calc():  d    = %g\n", b->d);
  printf("bars_calc():  c    = %g\n", b->c);
  printf("bars_calc():  l2   = %g\n", b->l2);

  
  printf("bars_calc():  ----------------------\n");
  printf("bars_calc():  Bar #2 x-y-z offsts:\n");
  printf("bars_calc():  E    = %g\n", b->E);
  printf("bars_calc():  P    = %g\n", b->P);
  printf("bars_calc():  l3   = %g\n", b->l3);

  printf("bars_calc():  ----------------------\n");
  printf("bars_calc():  freq = %g MHz\n",b->freq/1e6);
  printf("bars_calc():  ----------------------\n");
#endif


#ifdef DEBUG_CALC
  printf("bars_calc():  ----------------------\n");
  printf("bars_calc():  Calculated L1 = %g nH\n", b->L1*1e9);
  printf("bars_calc():  Calculated L2 = %g nH\n", b->L2*1e9);
  printf("bars_calc():  Calculated M  = %g nH\n", b->M*1e9);
  printf("bars_calc():  Calculated k  = %g\n", b->k);
  printf("bars_calc():  ----------------------\n");
#endif

  return 0;
}

/*
 * 
 */

int bars_syn(bars *b, double f, int flag)
{
  alert (_("bars_syn() -- not yet implemented\n"));
  return -1;

#ifdef DEBUG_SYN
  printf("bars_syn():  ----------------------\n");
  printf("bars_syn():  Input values:\n");
  printf("bars_syn():  ----------------------\n");
  printf("bars_syn():  N    = %g\n",coil->Nf);
  printf("bars_syn():  AWG  = %g\n",coil->AWGf);
  printf("bars_syn():  I.D. = %g inches\n",M2INCH(coil->dia));
  printf("bars_syn():  len  = %g inches\n",M2INCH(coil->len));
  printf("bars_syn():  rho  = %g\n",coil->rho);
  printf("bars_syn():  freq = %g MHz\n",coil->freq*1e-6);
  printf("bars_syn():  ----------------------\n");
  printf("bars_syn():  desired inductance  = %g nH\n",L*1e9);
  printf("bars_syn():  \n");
  printf("bars_syn():  ----------------------\n");
#endif

#ifdef FIXME
  error=1.0;
  while(error > 1e-8){
    len1 = len2;
    len2 = len;
    
    coil->len = INCH2M(len1);
    if (bars_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn1     = coil->L;
    
    coil->len = INCH2M(len2);
    if (bars_calc_int(coil,f,CALC_MIN) != 0)
      return -1;
    Lsyn2     = coil->L;
    
  
    len = len2 + (L - Lsyn2)*(len2-len1)/(Lsyn2-Lsyn1);
    error = fabs(len-len2)/len;

#ifdef DEBUG_SYN
    printf("bars_syn():  len=%g\tlen1=%g\tlen2=%g\n",len,len1,len2);
    printf("                 L1=%g\tL2=%g\terror=%g\n",Lsyn1,Lsyn2,error);
#endif
  
    coil->len = INCH2M(len);

    /* fill in the rest of the data */
    if (bars_calc_int(coil,f,CALC_ALL) != 0)
      return -1;

  }
#endif

  return 0;
}


void bars_free(bars *b)
{

  wc_units_free(b->units_xy);
  wc_units_free(b->units_L);
  wc_units_free(b->units_freq);

  free(b);
}


bars *bars_new()
{
  bars *newb;

  newb = (bars *) malloc(sizeof(bars));
  if(newb == NULL)
    {
      fprintf(stderr,"bars.c:bars_new(): malloc() failed\n");
      exit(1);
    }

  /* Create the units */
  newb->units_xy = wc_units_new(WC_UNITS_LENGTH);
  newb->units_L = wc_units_new(WC_UNITS_INDUCTANCE);
  newb->units_freq = wc_units_new(WC_UNITS_FREQUENCY);

  /* load in the defaults */
  bars_load_string(newb, default_bars);

  /* get the rest of the entries in sync */
  bars_calc(newb, newb->freq);

  return(newb);
}


