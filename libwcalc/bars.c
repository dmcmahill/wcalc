/* $Id: bars.c,v 1.14 2006/04/06 04:20:03 dan Exp $ */

/*
 * Copyright (c) 2008 Dan McMahill
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

/* #define DEBUG_CALC */  /* debug the bars_calc() function */
/* #define DEBUG_SYN  */  /* debug the bars_syn() function  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
static double Mb_fn(double a, double b, double c, double d, double x,double y,double z)
{

  double rslt = 0.0;
  double mag, mag2;

  mag2 = x*x + y*y + z*z;
  mag = sqrt(mag);

  rslt += (y*y*z*z/4.0 - y*y*y*y/24.0 - z*z*z*z/24.0) * x * 
    log( (x + mag) / sqrt( y*y + z*z));

  rslt += (x*x*z*z/4.0 - x*x*x*x/24.0 - z*z*z*z/24.0) * y * 
    log( (y + mag) / sqrt( x*x + z*z));

  rslt += (x*x*y*y/4.0 - x*x*x*x/24.0 - y*y*y*y/24.0) * z * 
    log( (z + mag) / sqrt( x*x + y*y));

  rslt += (1.0 / 60.0) * (x*x*x*x + y*y*y*y + z*z*z*z - 3*x*x*y*y - 3*y*y*z*z - 3*x*x*z*z) * mag;
  
  rslt -=  (x*y*z*z*z/6.0) * atan( x*y / (z*mag));

  rslt -=  (x*y*y*y*z/6.0) * atan( x*z / (y*mag));

  rslt -=  (x*x*x*y*z/6.0) * atan( y*z / (x*mag));

  return rslt;
}


/* (14) */
static double Mb(bars *b)
{
  double rslt;
  double q[5], r[5], s[5];

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

  rslt = 0.0;
  for(i=1 ; i<=4 ; i++) {
    for(j=1 ; j<=4 ; j++) {
      for(k=1 ; k<=4 ; k++) {
	rslt += pow(-1.0, i + j + k + 1) * Mb_fn(b->a, b->b, b->c, b->d, q[i], r[j], s[k]);
      }
    }
  }
  
  return rslt;
}

/* (20) */
static double Lb(bars *b, int w)
{
  double rslt;
  double q[5], r[5], s[5];

  if (w == 1) {
    q[1] = b->a;
    r[1] = b->b;
    s[1] = b->l1;
    a = b->a;
    b = b->b;
  } else if (w==2) {
    q[1] = b->c;
    r[1] = b->d;
    s[1] = b->l2;
    a = b->c;
    b = b->d;
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
	rslt += pow(-1.0, i + j + k + 1) * Mb_fn(a, b, a, b, q[i], r[j], s[k])
	  }
    }
  }

  return rslt;
}



int bars_calc(bars *b, double freq)
{

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
  printf("bars_calc():  freq = %g MHz\n",b->freq*1e6);
  printf("bars_calc():  ----------------------\n");
#endif

  b->L1 = Lb(b, 1);
  b->L2 = Lb(b, 2);
  b->M = Mb(b);
  b->k = b->M / sqrt(b->L1 * b->L2);

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
  alert ("bars_syn() -- not yet implemented\n");
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
  wc_units_free(b->units_z);
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
  newb->units_z = wc_units_new(WC_UNITS_LENGTH);
  newb->units_L = wc_units_new(WC_UNITS_INDUCTANCE);
  newb->units_freq = wc_units_new(WC_UNITS_FREQUENCY);

  /* load in the defaults */
  bars_load_string(newb, default_bars);

  /* get the rest of the entries in sync */
  bars_calc(newb, newb->freq);

  return(newb);
}


