/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2006, 2021 Dan McMahill
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

/* #define DEBUG_C_DIV */

/* #define BESSEL_WARNINGS */

#include "config.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mathutil.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

double coth(double x)
{
  return (1.0/tanh(x));
}


/*
 * The methods for most of the complex math are from :
 *
 * W. Press, S. Teukilsky, W. Vetterling, and B. Flannery,
 * "Numerical Recipes in C", 2nd ed, 1992, Cambridge University
 * Press, section 5.4
 *
 * I had to re-code these functions to come up with a version I
 * could distribute, but given the simplicity of the formulas, its
 * somewhat silly as I imagine my code looks pretty similar to theirs,
 * I mean, how many ways can you write "a = b + c"?  But, such is life...
 *
 */

/*
 * Bessel functions.  Complex Arguments
 */

/* 
 * Bessel function of the first kind, order 0, complex arguments 
 *
 * This function is based on the approximation given in section 9.4.1
 * and 9.4.3 of Abramowitz and Stegun
 *
 */
complex double cbessel_J0(complex double x)
{
  complex double x2;
  complex double J0;

  if (fabs(creal(x)) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    J0 = 0.0002100 + 0.0i;
    J0 = (J0 * x2) + (-0.0039444 + 0.0i);
    J0 = (J0 * x2) + ( 0.0444479 + 0.0i);
    J0 = (J0 * x2) + (-0.3163866 + 0.0i);
    J0 = (J0 * x2) + ( 1.2656208 + 0.0i);
    J0 = (J0 * x2) + (-2.2499997 + 0.0i);
    J0 = (J0 * x2) + ( 1.0000000 + 0.0i);
  } else {
    complex double f0, t0;
    bool neg = false;
    
    if (creal(x) < 0){
      x = -creal(x) + cimag(x);
      neg = true;
    }

    x2 = (3.0 + 0.0i) / x;

    f0 = 0.00014476 + 0.0i;
    f0 = (f0 * x2) + (-0.00072805 + 0.0i);
    f0 = (f0 * x2) + ( 0.00137237 + 0.0i);
    f0 = (f0 * x2) + (-0.00009512 + 0.0i);
    f0 = (f0 * x2) + (-0.00552740 + 0.0i);
    f0 = (f0 * x2) + (-0.00000077 + 0.0i);
    f0 = (f0 * x2) + ( 0.79788456 + 0.0i);
    
    t0 = 0.00013558 + 0.0i;
    t0 = (t0 * x2) + (-0.00029333 + 0.0i);
    t0 = (t0 * x2) + (-0.00054125 + 0.0i);
    t0 = (t0 * x2) + ( 0.00262373 + 0.0i);
    t0 = (t0 * x2) + (-0.00003954 + 0.0i);
    t0 = (t0 * x2) + (-0.04166397 + 0.0i);
    t0 = (t0 * x2) + (-0.78539816 + 0.0i);

    t0 = x + t0;

    J0 = f0 * ccos(t0);
    J0 = J0 / csqrt(x);

    if(neg)
      J0 = creal(J0) - cimag(J0)*I;

  }
  return J0;
}

/* 
 * Bessel function of the second kind, order 0, complex arguments 
 *
 * This function is based on the approximation given in section 9.4.2
 * and 9.4.3 of Abramowitz and Stegun
 *
 */
complex double cbessel_Y0(complex double x)
{
  complex double x2;
  complex double Y0;

  if (fabs(creal(x)) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    /* the polynomial in (x/3)^2 */
    Y0 = -0.00024846 + 0.0;
    Y0 = (Y0 * x2) + ( 0.00427916 + 0.0i);
    Y0 = (Y0 * x2) + (-0.04261214 + 0.0i);
    Y0 = (Y0 * x2) + ( 0.25300117 + 0.0i);
    Y0 = (Y0 * x2) + (-0.74350384 + 0.0i);
    Y0 = (Y0 * x2) + ( 0.60559366 + 0.0i);
    Y0 = (Y0 * x2) + ( 0.36746691 + 0.0i);

    /* the extra added term in front */
    x2 = clog(0.5 * x);
    x2 = (2.0 / M_PI) * x2;
    x2 = x2 * cbessel_J0(x);

    Y0 = x2 + Y0;

  } else {
    complex double f0, t0;
    bool neg = false;

    if (creal(x) < 0){
      x = -creal(x) + cimag(x);
      neg = true;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  cbessel_Y0 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }

    x2 = (3.0 + 0.0i) / x;

    f0 = 0.00014476 + 0.0i;
    f0 = (f0 * x2) + (-0.00072805 + 0.0i);
    f0 = (f0 * x2) + ( 0.00137237 + 0.0i);
    f0 = (f0 * x2) + (-0.00009512 + 0.0i);
    f0 = (f0 * x2) + (-0.00552740 + 0.0i);
    f0 = (f0 * x2) + (-0.00000077 + 0.0i);
    f0 = (f0 * x2) + ( 0.79788456 + 0.0i);
    
    t0 = 0.00013558 + 0.0i;
    t0 = (t0 * x2) + (-0.00029333 + 0.0i);
    t0 = (t0 * x2) + (-0.00054125 + 0.0i);
    t0 = (t0 * x2) + ( 0.00262373 + 0.0i);
    t0 = (t0 * x2) + (-0.00003954 + 0.0i);
    t0 = (t0 * x2) + (-0.04166397 + 0.0i);
    t0 = (t0 * x2) + (-0.78539816 + 0.0i);

    t0 = x + t0;

    Y0 = f0 * csin(t0);

    Y0 = Y0 / csqrt(x);

    if (neg)
      Y0 = creal(Y0) - cimag(Y0)*I;

  }
  return Y0;
}

/* 
 * Bessel function of the first kind, order 1, complex arguments 
 *
 * This function is based on the approximation given in section 9.4.4
 * and 9.4.6 of Abramowitz and Stegun
 *
 */
complex double cbessel_J1(complex double x)
{
  complex double x2;
  complex double J1;

  if (fabs(creal(x)) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    J1 = 0.00001109 + 0.0i;
    J1 = (J1 * x2) + (-0.00031761 + 0.0i);
    J1 = (J1 * x2) + ( 0.00443319 + 0.0i);
    J1 = (J1 * x2) + (-0.03954289 + 0.0i);
    J1 = (J1 * x2) + ( 0.21093573 + 0.0i);
    J1 = (J1 * x2) + (-0.56249985 + 0.0i);
    J1 = (J1 * x2) + ( 0.50000000 + 0.0i);

    J1 = x * J1;
  } else {
    complex double f1, t1;
    bool neg = false;

    if (creal(x) < 0){
      x = -creal(x) + cimag(x);
      neg = true;
    }

    x2 = (3.0 + 0.0i) / x;

    f1 = -0.00020033 + 0.0i;
    f1 = (f1 * x2) + ( 0.00113653 + 0.0i);
    f1 = (f1 * x2) + (-0.00249511 + 0.0i);
    f1 = (f1 * x2) + ( 0.00017105 + 0.0i);
    f1 = (f1 * x2) + ( 0.01659667 + 0.0i);
    f1 = (f1 * x2) + ( 0.00000156 + 0.0i);
    f1 = (f1 * x2) + ( 0.79788456 + 0.0i);
    
    t1 = -0.00029166 + 0.0i;
    t1 = (t1 * x2) + ( 0.00079824 + 0.0i);
    t1 = (t1 * x2) + ( 0.00074348 + 0.0i);
    t1 = (t1 * x2) + (-0.00637879 + 0.0i);
    t1 = (t1 * x2) + ( 0.00005650 + 0.0i);
    t1 = (t1 * x2) + ( 0.12499612 + 0.0i);
    t1 = (t1 * x2) + (-2.35619449 + 0.0i);

    t1 = x + t1;

    J1 = f1 * ccos(t1);
    J1 = J1 / csqrt(x);

    if(neg)
      J1 = -creal(J1) + cimag(J1)*I;

  }
  return J1;
}

/* 
 * Bessel function of the second kind, order 1, complex arguments 
 *
 * This function is based on the approximation given in section 9.4.5
 * and 9.4.6 of Abramowitz and Stegun
 *
 */
complex double cbessel_Y1(complex double x)
{
  complex double x2;
  complex double Y1;

  if (fabs(creal(x)) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;

    /* the polynomial in (x/3)^2 */
    Y1 = 0.0027873 + 0.0i;
    Y1 = (Y1 * x2) + (-0.0400976 + 0.0i);
    Y1 = (Y1 * x2) + ( 0.3123951 + 0.0i);
    Y1 = (Y1 * x2) + (-1.3164827 + 0.0i);
    Y1 = (Y1 * x2) + ( 2.1682709 + 0.0i);
    Y1 = (Y1 * x2) + ( 0.2212091 + 0.0i);
    Y1 = (Y1 * x2) + (-0.6366198 + 0.0i);

    /* the extra added term in front */
    x2 = (2.0 / M_PI) *x;
    x2 = x2 * clog(0.5 * x);
    x2 = x2 * cbessel_J1(x);

    Y1 = x2 + Y1;

    Y1 = Y1 / x;

  } else {
    complex double f1, t1;
    bool neg = false;

    if (creal(x) < 0){
      x = -creal(x) + cimag(x);
      neg = true;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  cbessel_Y1 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }

    x2 = (3.0 + 0.0i) / x;

    f1 = -0.00020033 + 0.0i;
    f1 = (f1 * x2) + ( 0.00113653 + 0.0i);
    f1 = (f1 * x2) + (-0.00249511 + 0.0i);
    f1 = (f1 * x2) + ( 0.00017105 + 0.0i);
    f1 = (f1 * x2) + ( 0.01659667 + 0.0i);
    f1 = (f1 * x2) + ( 0.00000156 + 0.0i);
    f1 = (f1 * x2) + ( 0.79788456 + 0.0i);
    
    t1 = -0.00029166 + 0.0i;
    t1 = (t1 * x2) + ( 0.00079824 + 0.0i);
    t1 = (t1 * x2) + ( 0.00074348 + 0.0i);
    t1 = (t1 * x2) + (-0.00637879 + 0.0i);
    t1 = (t1 * x2) + ( 0.00005650 + 0.0i);
    t1 = (t1 * x2) + ( 0.12499612 + 0.0i);
    t1 = (t1 * x2) + (-2.35619449 + 0.0i);

    t1 = x + t1;

    Y1 = f1 * csin(t1);

    Y1 = Y1 / csqrt(x);

    if (neg)
      Y1 = creal(Y1) - cimag(Y1)*I;

  }
  return Y1;
}

/*
 * Bessel functions with real arguments
 */

/* 
 * Bessel function of the first kind, order 0, real arguments 
 *
 * This function is based on the approximation given in section 9.4.1
 * and 9.4.3 of Abramowitz and Stegun
 *
 */
double bessel_J0(double x)
{
  double x2;
  double J0;
  double f0, t0;

  if (fabs(x) <= 3.0){
    /* find (x/3)^2 */
    x2 = x/3.0;

    x2 = x2*x2;
    
    J0 = 0.0002100;
    J0 = J0*x2 + -0.0039444;
    J0 = J0*x2 +  0.0444479;
    J0 = J0*x2 + -0.3163866;
    J0 = J0*x2 +  1.2656208;
    J0 = J0*x2 + -2.2499997;
    J0 = J0*x2 +  1.0000000;
  }
  else{
    
    if (x < 0){
      x = -x;
    }
    x2 = 3.0/x;

    f0 = 0.00014476;
    f0 = f0*x2 + -0.00072805;
    f0 = f0*x2 +  0.00137237;
    f0 = f0*x2 + -0.00009512;
    f0 = f0*x2 + -0.00552740;
    f0 = f0*x2 + -0.00000077;
    f0 = f0*x2 +  0.79788456;
    
    t0 = 0.00013558;
    t0 = t0*x2 + -0.00029333;
    t0 = t0*x2 + -0.00054125;
    t0 = t0*x2 +  0.00262373;
    t0 = t0*x2 + -0.00003954;
    t0 = t0*x2 + -0.04166397;
    t0 = t0*x2 + -0.78539816;

    t0 = x + t0;

    J0 = f0 * cos(t0);
    J0 = J0 / sqrt(x);

  }
  return J0;
}


/* 
 * Bessel function of the second kind, order 0, real arguments 
 *
 * This function is based on the approximation given in section 9.4.2
 * and 9.4.3 of Abramowitz and Stegun
 *
 */
double bessel_Y0(double x)
{
  double x2;
  double Y0;
  double f0, t0;

  if (fabs(x) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    /* the polynomial in (x/3)^2 */
    Y0 = -0.00024846;
    Y0 = Y0*x2 +  0.00427916;
    Y0 = Y0*x2 + -0.04261214;
    Y0 = Y0*x2 +  0.25300117;
    Y0 = Y0*x2 + -0.74350384;
    Y0 = Y0*x2 +  0.60559366;
    Y0 = Y0*x2 +  0.36746691;

    /* the extra added term in front */
    x2 = log(0.5*fabs(x));
    x2 = (2.0/M_PI) * x2;
    x2 = x2 * bessel_J0(x);

    Y0 = x2 + Y0;

  }
  else{

    if (x < 0){
      x = -x;
    }
    x2 = 3.0/x;

    f0 = 0.00014476;
    f0 = f0*x2 + -0.00072805;
    f0 = f0*x2 +  0.00137237;
    f0 = f0*x2 + -0.00009512;
    f0 = f0*x2 + -0.00552740;
    f0 = f0*x2 + -0.00000077;
    f0 = f0*x2 +  0.79788456;
    
    t0 = 0.00013558;
    t0 = t0*x2 + -0.00029333;
    t0 = t0*x2 + -0.00054125;
    t0 = t0*x2 +  0.00262373;
    t0 = t0*x2 + -0.00003954;
    t0 = t0*x2 + -0.04166397;
    t0 = t0*x2 + -0.78539816;

    t0 = x + t0;

    Y0 = f0 * sin(t0);

    Y0 = Y0 / sqrt(x);

  }
  return Y0;
}

/* 
 * Bessel function of the first kind, order 1, real arguments 
 *
 * This function is based on the approximation given in section 9.4.4
 * and 9.4.6 of Abramowitz and Stegun
 *
 */
double bessel_J1(double x)
{
  double x2;
  double J1;
  double f1, t1;
  int neg=0;

  if (fabs(x) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    J1 = 0.00001109;
    J1 = J1*x2 + -0.00031761;
    J1 = J1*x2 +  0.00443319;
    J1 = J1*x2 + -0.03954289;
    J1 = J1*x2 +  0.21093573;
    J1 = J1*x2 + -0.56249985;
    J1 = J1*x2 +  0.50000000;

    J1 = x*J1;
  }
  else{

    if (x < 0){
      x = -x;
      neg = 1;
    }

    x2 = 3.0/x;

    f1 = -0.00020033;
    f1 = f1*x2 +  0.00113653;
    f1 = f1*x2 + -0.00249511;
    f1 = f1*x2 +  0.00017105;
    f1 = f1*x2 +  0.01659667;
    f1 = f1*x2 +  0.00000156;
    f1 = f1*x2 +  0.79788456;
    
    t1 = -0.00029166;
    t1 = t1*x2 +  0.00079824;
    t1 = t1*x2 +  0.00074348;
    t1 = t1*x2 + -0.00637879;
    t1 = t1*x2 +  0.00005650;
    t1 = t1*x2 +  0.12499612;
    t1 = t1*x2 + -2.35619449;

    t1 = x + t1;

    J1 = f1 * cos(t1);
    J1 = J1 / sqrt(x);

    if(neg)
      J1 = -J1;

  }
  return J1;
}

/* 
 * Bessel function of the second kind, order 1, real arguments 
 *
 * This function is based on the approximation given in section 9.4.5
 * and 9.4.6 of Abramowitz and Stegun
 *
 */
double bessel_Y1(double x)
{
  double x2;
  double Y1;
  double f1, t1;

  if (fabs(x) <= 3.0){
    /* find (x/3)^2 */
    x2 = x / 3.0;
    x2 = x2 * x2;
    
    /* the polynomial in (x/3)^2 */
    Y1 = 0.0027873;
    Y1 = Y1*x2 + -0.0400976;
    Y1 = Y1*x2 +  0.3123951;
    Y1 = Y1*x2 + -1.3164827;
    Y1 = Y1*x2 +  2.1682709;
    Y1 = Y1*x2 +  0.2212091;
    Y1 = Y1*x2 + -0.6366198;

    /* the extra added term in front */
    x2 = (2.0/M_PI)*x;
    x2 = x2*log(0.5*fabs(x));
    x2 = x2*bessel_J1(x);

    Y1 = x2 + Y1;

    Y1 = Y1 / x;
    if (x < 0)
      Y1 = -Y1;

  }
  else{

    if (x < 0){
      x = -x;
    }
    x2 = 3.0 / x;

    f1 = -0.00020033;
    f1 = f1*x2 +  0.00113653;
    f1 = f1*x2 + -0.00249511;
    f1 = f1*x2 +  0.00017105;
    f1 = f1*x2 +  0.01659667;
    f1 = f1*x2 +  0.00000156;
    f1 = f1*x2 +  0.79788456;
    
    t1 = -0.00029166;
    t1 = t1*x2 +  0.00079824;
    t1 = t1*x2 +  0.00074348;
    t1 = t1*x2 + -0.00637879;
    t1 = t1*x2 +  0.00005650;
    t1 = t1*x2 +  0.12499612;
    t1 = t1*x2 + -2.35619449;

    t1 = x + t1;

    Y1 = f1 * sin(t1);

    Y1 = Y1 / sqrt(x);

  }
  return Y1;
}

/*
 * derivatives of bessel functions
 */

double bessel_J1p(double x)
{
  double y;
  y = bessel_J0(x) - bessel_J1(x)/x;

  return y;
}

double bessel_Y1p(double x)
{
  double y;
  y = bessel_Y0(x) - bessel_Y1(x)/x;

  return y;
}

/*
 * Hankel functions
 */

/* Type 1, order 0 Hankel function: J0(x) + j*Y0(x) */
complex double chankel0_1(complex double x)
{
  complex double y;
  complex double j;
  
  j = 0.0 + 1.0i;

  y = cbessel_J0(x) + j * cbessel_Y0(x);

  return y;
}

/* Type 2, order 0 Hankel function: J0(x) - j*Y0(x) */
complex double chankel0_2(complex double x)
{
  complex double y;
  complex double j;
  
  j = 0.0 + 1.0i;

  y = cbessel_J0(x) - j * cbessel_Y0(x);

  return y;
}

/* Type 1, order 1 Hankel function: J1(x) + j*Y1(x) */
complex double chankel1_1(complex double x)
{
  complex double y;
  complex double j;
  
  j = 0.0 + 1.0i;

  y = cbessel_J1(x) + j * cbessel_Y1(x);

  return y;
}

/* Type 2, order 1 Hankel function: J1(x) - j*Y1(x) */
complex double chankel1_2(complex double x)
{
  complex double y;
  complex double j;
  
  j = 0.0 + 1.0i;

  y = cbessel_J1(x) - j * cbessel_Y1(x);

  return y;
}

 /*
  *  compute K(k)/K'(k) where
  * K is the complete elliptic integral of the first kind,
  * K' is the complementary complete elliptic integral of the first kind
  */

double k_over_kp(double k)
{
  double kp, r, kf;
  int i = 0;

  /* 
   * the series calculation below works for k < 1.0, but if k == 1.0
   * then the series does not converge because k = 1, kp = 0,
   * and kf = 2 for every iteration which means that r just doubles
   * each time around.  That makes sense because 
   * K(k) -> infinity when k = 1.  Note:
   * K(0) = pi/2, K(1) = +infinity
   */

  kp = sqrt(1.0-pow(k,2.0));
  r = 1.0;
  do {
    kf = (1.0 + k) / (1.0 + kp);
    r = r*kf;
    k = 2.0*sqrt(k) / (1.0 + k);
    kp = 2.0*sqrt(kp) / (1.0 + kp);
    i++;
  } while( (fabs(kf - 1.0) > 1e-15) && ( i < 100) );

/* alternate approach
  if( k < sqrt(0.5) ) {
    kp = sqrt(1.0 - k*k);
    r = M_PI / log(2.0 * (1.0 + sqrt(kp)) / (1.0 - sqrt(kp)) );
  } else {
    r = log(2.0 * (1.0 + sqrt(k)) / (1.0 - sqrt(k)) ) / M_PI;
  }
*/
  return r;
}


