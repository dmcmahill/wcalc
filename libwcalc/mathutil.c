/* $Id: mathutil.c,v 1.11 2006/01/07 00:02:49 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2006 Dan McMahill
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
 * NOTE:  All the _p functions pass inputs/outputs by reference rather
 * than by value (like the non _p ones).  This avoids a GCC bug (stack
 * corruption) on sparc hardware (seen on solaris and NetBSD) and on
 * m68k (clobbering input args).  The last argument is always the
 * pointer to the result.  The functions always return that pointer.
 * If NULL is used, then storage is allocated to hold the return
 * value. 
 */

complex c_add(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a) + REAL(b);
  IMAG(z) = IMAG(a) + IMAG(b);
  return z;
}

complex * c_add_p(complex *a, complex *b, complex *c)
{
  if (c == NULL) 
    c = c_complex_new();

  c->re = a->re + b->re;
  c->im = a->im + b->im;
  
  return c;
}

complex c_sub(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a) - REAL(b);
  IMAG(z) = IMAG(a) - IMAG(b);
  return z;
}

complex * c_sub_p(complex *a, complex *b, complex *c)
{
  if (c == NULL) 
    c = c_complex_new();

  c->re = a->re - b->re;
  c->im = a->im - b->im;
  
  return c;
}


complex c_mul(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a)*REAL(b) - IMAG(a)*IMAG(b);
  IMAG(z) = IMAG(a)*REAL(b) + REAL(a)*IMAG(b);
  return z;
}

complex * c_mul_p(complex *a, complex *b, complex *c)
{
  double a_re, b_re;
  
  a_re = a->re;
  b_re = b->re;

  if (c == NULL) 
    c = c_complex_new();

  c->re = a->re*b->re - a->im*b->im;
  c->im = a->im*b_re + a_re*b->im;

  return c;
}

complex c_rmul(double x, complex a)
{
  complex z;
  REAL(z) = x*REAL(a);
  IMAG(z) = x*IMAG(a);
  return z;
}

complex * c_rmul_p(double x, complex *a, complex *b)
{
  if (b == NULL) 
    b = c_complex_new();

  b->re = x*a->re;
  b->im = x*a->im;
  
  return b;
}


complex c_div(complex x, complex y)
{
  complex z;
  double a,b,c,d;
  /* temp variables to reduce duplicated calculations */
  double rat, den;

  a = REAL(x);
  b = IMAG(x);
  c = REAL(y);
  d = IMAG(y);

#ifdef DEBUG_C_DIV  
  printf("c_div(%g + %g i, %g + %g i)(%p,%p)\n",a,b,c,d,&x,&y);
#endif

  if (d == 0){
    /* the divisor is real */
    REAL(z) = a/c;
    IMAG(z) = b/c;
  }
  else if (c == 0){
    REAL(z) = b/d;
    IMAG(z) = -a/d;
  }
  else{
    if (fabs(c) >= fabs(d)){
      rat = d/c;
      den = c + d*rat;
      REAL(z) = (a + b*rat)/den;
      IMAG(z) = (b - a*rat)/den;
    }
    else{
      rat = c/d;
      den = d + c*rat;
      REAL(z) = (a*rat + b)/den;
      IMAG(z) = (b*rat - a)/den;
    }
  }

  return z;
}

complex * c_div_p(complex *x, complex *y, complex *z)
{
  double a,b,c,d;
  /* temp variables to reduce duplicated calculations */
  double rat, den;

  a = x->re;
  b = x->im;
  c = y->re;
  d = y->im;

  if (z == NULL) 
    z = c_complex_new();

#ifdef DEBUG_C_DIV  
  printf("c_div(%g + %g i, %g + %g i)(%p,%p)\n",a,b,c,d,x,y);
#endif

  if (d == 0){
    /* the divisor is real */
    z->re = a/c;
    z->im = b/c;
  }
  else if (c == 0){
    z->re = b/d;
    z->im = -a/d;
  }
  else{
    if (fabs(c) >= fabs(d)){
      rat = d/c;
      den = c + d*rat;
      z->re = (a + b*rat)/den;
      z->im = (b - a*rat)/den;
    }
    else{
      rat = c/d;
      den = d + c*rat;
      z->re = (a*rat + b)/den;
      z->im = (b*rat - a)/den;
    }
  }

  return z;
}

complex c_conj(complex a)
{
  complex z;
  REAL(z) = REAL(a);
  IMAG(z) = -IMAG(a);
  return z;
}

complex * c_conj_p(complex *a, complex *b)
{
  if (b == NULL) 
    b = c_complex_new();

  b->re = a->re;
  b->im = -b->im;

  return b;
}


double c_abs(complex z)
{
  double a,b,zabs;

  a=REAL(z);
  b=IMAG(z);

  if (b == 0.0){
    /* we're purely real, so lets bail out now */
    zabs = fabs(a);
  }
  else if (a == 0.0){
    /* we're purely imaginary, so lets bail out now */
    zabs = fabs(b);
  }
  else{
    /* 
     * the input is actually complex, so we have to do some work 
     * and implement (5.4.4) from NR
     */
    if (fabs(a) >= fabs(b))
      zabs = fabs(a)*sqrt(1.0 + pow(b/a,2.0));
    else
      zabs = fabs(b)*sqrt(1.0 + pow(a/b,2.0));
  }
  
  return zabs;
}

double c_abs_p(complex *z)
{
  double a,b,zabs;

  a=z->re;
  b=z->im;

  if (b == 0.0){
    /* we're purely real, so lets bail out now */
    zabs = fabs(a);
  }
  else if (a == 0.0){
    /* we're purely imaginary, so lets bail out now */
    zabs = fabs(b);
  }
  else{
    /* 
     * the input is actually complex, so we have to do some work 
     * and implement (5.4.4) from NR
     */
    if (fabs(a) >= fabs(b))
      zabs = fabs(a)*sqrt(1.0 + pow(b/a,2.0));
    else
      zabs = fabs(b)*sqrt(1.0 + pow(a/b,2.0));
  }
  
  return zabs;
}

/* returns the angle */
double c_arg(complex x)
{
  double phi;
  phi = atan2(IMAG(x),REAL(x));
  return phi;
}

double c_arg_p(complex *x)
{
  double phi;
  phi = atan2(x->im,x->re);
  return phi;
}

complex c_complex(double re, double im)
{
  complex z;
  REAL(z) = re;
  IMAG(z) = im;
  return z;
}

complex *c_complex_p(double re, double im, complex *z)
{
  if (z == NULL)
    z=c_complex_new();

  z->re = re;
  z->im = im;
  return z;
}

complex *c_complex_new(void)
{
  complex *z;
  
  z = (complex * ) malloc(sizeof(complex));
  if (z == NULL) {
    fprintf(stderr,"mathutil.c: c_complex_new():  malloc failed\n");
    exit(1);
  }

  return z;
}

/* complex square root.  branch cut is along negative real axis */
complex c_sqrt(complex x)
{
  complex z;
  double c, d;
  double w;

  c = REAL(x);
  d = IMAG(x);

  if ( (c == 0.0) && (d == 0.0) ){
    REAL(z) = 0.0;
    IMAG(z) = 0.0;
    return z;
  }
  else if (fabs(c) >= fabs(d)){
    w = sqrt(fabs(c)) * sqrt( (1.0 + sqrt(1.0 + pow(d/c,2.0))) / 2.0);
  }
  else{
    w = sqrt(fabs(d)) * sqrt( (fabs(c/d) + sqrt(1.0 + pow(c/d,2.0))) / 2.0);
  }

  if (w == 0.0){
    REAL(z) = 0.0;
    IMAG(z) = 0.0;
  }
  else if (c >= 0.0){
    REAL(z) = w;
    IMAG(z) = d/(2.0*w);
  }
  else if (d >= 0.0){
    REAL(z) = fabs(d)/(2.0*w);
    IMAG(z) = w;
  }
  else{
    REAL(z) = fabs(d)/(2.0*w);
    IMAG(z) = -w;
  }

  return z;
}


complex * c_sqrt_p(complex *x, complex *z)
{
  double c, d;
  double w;

  if (z == NULL)
    z = c_complex_new();

  c = x->re;
  d = x->im;

  if ( (c == 0.0) && (d == 0.0) ){
    z->re = 0.0;
    z->im = 0.0;
    return z;
  }
  else if (fabs(c) >= fabs(d)){
    w = sqrt(fabs(c)) * sqrt( (1.0 + sqrt(1.0 + pow(d/c,2.0))) / 2.0);
  }
  else{
    w = sqrt(fabs(d)) * sqrt( (fabs(c/d) + sqrt(1.0 + pow(c/d,2.0))) / 2.0);
  }

  if (w == 0.0){
    z->re = 0.0;
    z->im = 0.0;
  }
  else if (c >= 0.0){
    z->re = w;
    z->im = d/(2.0*w);
  }
  else if (d >= 0.0){
    z->re = fabs(d)/(2.0*w);
    z->im = w;
  }
  else{
    z->re = fabs(d)/(2.0*w);
    z->im = -w;
  }

  return z;
}


/*
 * complex trig functions
 */

complex c_cos(complex x)
{
  complex y;

  y = c_complex( cosh(IMAG(x))*cos(REAL(x)),
		 -sinh(IMAG(x))*sin(REAL(x)) );
  return y;
}

complex * c_cos_p(complex *x, complex *z)
{
  z = c_complex_p( cosh(x->im)*cos(x->re),
		   -sinh(x->im)*sin(x->re), z );
  return z;
}

complex c_cosh(complex x)
{
  complex y;

  y = c_complex( cosh(REAL(x))*cos(IMAG(x)),
		 sinh(REAL(x))*sin(IMAG(x)) );
  return y;
}

complex * c_cosh_p(complex *x, complex *z)
{

  z = c_complex_p( cosh(x->re)*cos(x->im),
		   sinh(x->re)*sin(x->im), z );

  return z;
}

complex c_sin(complex x)
{
  complex y;

  y = c_complex( cosh(IMAG(x))*sin(REAL(x)),
		 sinh(IMAG(x))*cos(REAL(x)) );
  return y;
}

complex * c_sin_p(complex *x, complex *z)
{
  z = c_complex_p( cosh(x->im)*sin(x->re),
		   sinh(x->im)*cos(x->re), z);

  return z;
}

complex c_sinh(complex x)
{
  complex y;

  y = c_complex( sinh(REAL(x))*cos(IMAG(x)),
		 cosh(REAL(x))*sin(IMAG(x)) );
  return y;
}

complex * c_sinh_p(complex *x, complex *z)
{

  z =  c_complex_p( sinh(x->re)*cos(x->im),
		    cosh(x->re)*sin(x->im), z);

  return z;
}

complex c_tan(complex x)
{
  complex y;

  y = c_div(c_sin(x),c_cos(x));

  return y;
}

complex * c_tan_p(complex *x, complex *z)
{
  complex *s,*c;

  s=c_sin_p(x,NULL);
  c=c_cos_p(x,NULL);

  z = c_div_p(s,c,z);

  free(s);
  free(c);

  return z;
}

complex c_cot(complex x)
{
  complex y;

  y = c_div(c_cos(x),c_sin(x));

  return y;
}

complex * c_cot_p(complex *x, complex *z)
{
  complex *s,*c;

  s=c_sin_p(x,NULL);
  c=c_cos_p(x,NULL);

  z=c_div_p(c,s,z);

  free(s);
  free(c);

  return z;
}

complex c_tanh(complex x)
{
  complex y;

  y = c_div(c_sinh(x),c_cosh(x));

  return y;
}

complex * c_tanh_p(complex *x, complex *z)
{
  complex *s,*c;

  s=c_sinh_p(x,NULL);
  c=c_cosh_p(x,NULL);

  z=c_div_p(s,c,z);

  free(s);
  free(c);

  return z;
}

complex c_coth(complex x)
{
  complex y;

  y = c_div(c_cosh(x),c_sinh(x));

  return y;
}

complex * c_coth_p(complex *x, complex *z)
{
  complex *s,*c;

  s=c_sinh_p(x,NULL);
  c=c_cosh_p(x,NULL);

  z=c_div_p(c,s,z);

  free(s);
  free(c);

  return z;
}


/*
 * Log's
 */
complex c_log(complex x)
{
  double r,i;
  complex z;

  r = log(c_abs(x));
  i = c_arg(x);
  z = c_complex(r,i);
  return z;
}

complex * c_log_p(complex *x, complex *z)
{
  double r,i;

  r = log(c_abs_p(x));
  i = c_arg_p(x);
  z = c_complex_p(r,i,z);
  return z;
}

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
complex c_bessel_J0(complex x)
{
  complex x2;
  complex J0;
  complex f0, t0;
  int neg=0;

  if (fabs(REAL(x)) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = REAL(x)/3.0;
    IMAG(x2) = IMAG(x)/3.0;
    x2 = c_mul(x2,x2);
    
    J0 = c_complex(0.0002100,0.0);
    J0 = c_add(c_mul(J0,x2),c_complex(-0.0039444 , 0.0));
    J0 = c_add(c_mul(J0,x2),c_complex( 0.0444479 , 0.0));
    J0 = c_add(c_mul(J0,x2),c_complex(-0.3163866 , 0.0));
    J0 = c_add(c_mul(J0,x2),c_complex( 1.2656208 , 0.0));
    J0 = c_add(c_mul(J0,x2),c_complex(-2.2499997 , 0.0));
    J0 = c_add(c_mul(J0,x2),c_complex( 1.0000000 , 0.0));
  }
  else{
    
    if (REAL(x) < 0){
      REAL(x) = -REAL(x);
      neg = 1;
    }
    x2 = c_div(c_complex(3.0,0.0),x);

    f0 = c_complex(0.00014476,0.0);
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00072805 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex( 0.00137237 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00009512 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00552740 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00000077 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex( 0.79788456 , 0.0));
    
    t0 = c_complex(0.00013558,0.0);
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00029333 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00054125 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex( 0.00262373 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00003954 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.04166397 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.78539816 , 0.0));

    t0 = c_add(x, t0);

    J0 = c_mul(f0,c_cos(t0));
    J0 = c_div(J0,c_sqrt(x));

    if(neg)
      IMAG(J0) = -IMAG(J0);

  }
  return J0;
}

#define C_BESSEL_POLY(J,x,k,c) {\
    c_complex_p((c),0.0,(k)); \
    c_mul_p((J),(x),(J)); \
    c_add_p((J),(k),(J));}

#define C_BESSEL_POLY2(J,x,k,c) {\
    c_complex_p((c),0.0,(k)); \
    c_mul_p((J),(x),(J)); \
    c_add_p((J),(k),(J));}

complex * c_bessel_J0_p(complex *x, complex *J0)
{
  complex x2;
  complex f0;
  complex t0;
  complex k;
  int neg=0;

  
  if (fabs(x->re) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = x->re/3.0;
    IMAG(x2) = x->im/3.0;
    c_mul_p(&x2,&x2,&x2);
    
    J0 = c_complex_p(0.0002100,0.0, J0);

    C_BESSEL_POLY2(J0,&x2,&k,-0.0039444);
    C_BESSEL_POLY2(J0,&x2,&k, 0.0444479);
    C_BESSEL_POLY2(J0,&x2,&k,-0.3163866);
    C_BESSEL_POLY2(J0,&x2,&k, 1.2656208);
    C_BESSEL_POLY2(J0,&x2,&k,-2.2499997);
    C_BESSEL_POLY2(J0,&x2,&k, 1.0000000);
  }
  else{
    
    if (x->re < 0){
      x->re = -x->re;
      neg = 1;
    }
    c_complex_p(3.0,0.0,&k);
    c_div_p(&k,x,&x2);

    c_complex_p(0.00014476,0.0,&f0);

    C_BESSEL_POLY2(&f0,&x2,&k,-0.00072805);
    C_BESSEL_POLY2(&f0,&x2,&k, 0.00137237);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00009512);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00552740);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00000077);
    C_BESSEL_POLY2(&f0,&x2,&k, 0.79788456);

    c_complex_p(0.00013558,0.0,&t0);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.00029333);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.00054125);
    C_BESSEL_POLY2(&t0,&x2,&k, 0.00262373);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.00003954);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.04166397);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.78539816);
    c_add_p(x, &t0, &t0);


    c_cos_p(&t0,&k);
    J0 = c_mul_p(&f0,&k,J0);
    c_sqrt_p(x,&k);
    J0 = c_div_p(J0,&k,J0);

    if(neg) {
      J0->im = -J0->im;
      x->re = -x->re;
    }
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
complex c_bessel_Y0(complex x)
{
  complex x2;
  complex Y0;
  complex f0, t0;
  int neg=0;

  if (fabs(REAL(x)) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = REAL(x)/3.0;
    IMAG(x2) = IMAG(x)/3.0;
    x2 = c_mul(x2,x2);
    
    /* the polynomial in (x/3)^2 */
    Y0 = c_complex(-0.00024846,0.0);
    Y0 = c_add(c_mul(Y0,x2),c_complex( 0.00427916 , 0.0));
    Y0 = c_add(c_mul(Y0,x2),c_complex(-0.04261214 , 0.0));
    Y0 = c_add(c_mul(Y0,x2),c_complex( 0.25300117 , 0.0));
    Y0 = c_add(c_mul(Y0,x2),c_complex(-0.74350384 , 0.0));
    Y0 = c_add(c_mul(Y0,x2),c_complex( 0.60559366 , 0.0));
    Y0 = c_add(c_mul(Y0,x2),c_complex( 0.36746691 , 0.0));

    /* the extra added term in front */
    x2 = c_log(c_rmul(0.5,x));
    x2 = c_rmul(2.0/M_PI , x2);
    x2 = c_mul(x2,c_bessel_J0(x));

    Y0 = c_add(x2,Y0);

  }
  else{

    if (REAL(x) < 0){
      REAL(x) = -REAL(x);
      neg = 1;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  c_bessel_Y0 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }
    x2 = c_div(c_complex(3.0,0.0),x);

    f0 = c_complex(0.00014476,0.0);
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00072805 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex( 0.00137237 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00009512 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00552740 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex(-0.00000077 , 0.0));
    f0 = c_add(c_mul(f0,x2),c_complex( 0.79788456 , 0.0));
    
    t0 = c_complex(0.00013558,0.0);
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00029333 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00054125 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex( 0.00262373 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.00003954 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.04166397 , 0.0));
    t0 = c_add(c_mul(t0,x2),c_complex(-0.78539816 , 0.0));

    t0 = c_add(x, t0);

    Y0 = c_mul(f0,c_sin(t0));

    Y0 = c_div(Y0,c_sqrt(x));

    if (neg)
      IMAG(Y0) = -IMAG(Y0);

  }
  return Y0;
}


complex * c_bessel_Y0_p(complex *x, complex *Y0)
{
  complex x2, k;
  complex f0, t0;
  int neg=0;


  if (fabs(x->re) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = x->re/3.0;
    IMAG(x2) = x->im/3.0;
    c_mul_p(&x2,&x2,&x2);
    
    /* the polynomial in (x/3)^2 */
    Y0 = c_complex_p(-0.00024846, 0.0, Y0);

    C_BESSEL_POLY2(Y0,&x2,&k, 0.00427916);
    C_BESSEL_POLY2(Y0,&x2,&k,-0.04261214);
    C_BESSEL_POLY2(Y0,&x2,&k, 0.25300117);
    C_BESSEL_POLY2(Y0,&x2,&k,-0.74350384);
    C_BESSEL_POLY2(Y0,&x2,&k, 0.60559366);
    C_BESSEL_POLY2(Y0,&x2,&k, 0.36746691);

    /* the extra added term in front */
    c_rmul_p(0.5,x,&x2);
    c_log_p(&x2,&x2);
    c_rmul_p(2.0/M_PI , &x2, &x2);

    c_bessel_J0_p(x,&t0);
    c_mul_p(&x2,&t0,&x2);

    Y0 = c_add_p(&x2,Y0,Y0);

  }
  else{

    if (x->re < 0){
      x->re = -x->re;
      neg = 1;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  c_bessel_Y0_p called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }
    c_complex_p(3.0,0.0,&k);
    c_div_p(&k,x,&x2);

    c_complex_p(0.00014476,0.0,&f0);

    C_BESSEL_POLY2(&f0,&x2,&k,-0.00072805);
    C_BESSEL_POLY2(&f0,&x2,&k, 0.00137237);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00009512);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00552740);
    C_BESSEL_POLY2(&f0,&x2,&k,-0.00000077);
    C_BESSEL_POLY2(&f0,&x2,&k, 0.79788456);

    
    c_complex_p(0.00013558,0.0,&t0);

    C_BESSEL_POLY2(&t0,&x2,&k,-0.00029333);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.00054125);
    C_BESSEL_POLY2(&t0,&x2,&k, 0.00262373);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.00003954);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.04166397);
    C_BESSEL_POLY2(&t0,&x2,&k,-0.78539816);

    c_add_p(x, &t0, &t0);

    c_sin_p(&t0,&k);
    Y0 = c_mul_p(&f0,&k,Y0);

    c_sqrt_p(x,&x2);
    Y0 = c_div_p(Y0,&x2,Y0);

    if (neg) {
      Y0->im = -Y0->im;
      x->re = -x->re;
    }
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
complex c_bessel_J1(complex x)
{
  complex x2;
  complex J1;
  complex f1, t1;
  int neg=0;

  if (fabs(REAL(x)) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = REAL(x)/3.0;
    IMAG(x2) = IMAG(x)/3.0;
    x2 = c_mul(x2,x2);
    
    J1 = c_complex(0.00001109 , 0.0);
    J1 = c_add(c_mul(J1,x2),c_complex(-0.00031761 , 0.0));
    J1 = c_add(c_mul(J1,x2),c_complex( 0.00443319 , 0.0));
    J1 = c_add(c_mul(J1,x2),c_complex(-0.03954289 , 0.0));
    J1 = c_add(c_mul(J1,x2),c_complex( 0.21093573 , 0.0));
    J1 = c_add(c_mul(J1,x2),c_complex(-0.56249985 , 0.0));
    J1 = c_add(c_mul(J1,x2),c_complex( 0.50000000 , 0.0));

    J1 = c_mul(x,J1);
  }
  else{

    if (REAL(x) < 0){
      REAL(x) = -REAL(x);
      neg = 1;
    }

    x2 = c_div(c_complex(3.0,0.0),x);

    f1 = c_complex(-0.00020033 , 0.0);
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00113653 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex(-0.00249511 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00017105 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.01659667 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00000156 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.79788456 , 0.0));
    
    t1 = c_complex(-0.00029166 , 0.0);
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00079824 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00074348 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex(-0.00637879 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00005650 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.12499612 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex(-2.35619449 , 0.0));

    t1 = c_add(x, t1);

    J1 = c_mul(f1,c_cos(t1));
    J1 = c_div(J1,c_sqrt(x));

    if(neg)
      REAL(J1) = -REAL(J1);

  }
  return J1;
}


complex * c_bessel_J1_p(complex *x, complex *J1)
{
  complex x2, k;
  complex f1, t1;
  int neg=0;


  if (fabs(x->re) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = x->re/3.0;
    IMAG(x2) = x->im/3.0;
    c_mul_p(&x2,&x2,&x2);

    J1 = c_complex_p(0.00001109 , 0.0, J1);

    C_BESSEL_POLY(J1,&x2,&k,-0.00031761);
    C_BESSEL_POLY(J1,&x2,&k, 0.00443319);
    C_BESSEL_POLY(J1,&x2,&k,-0.03954289);
    C_BESSEL_POLY(J1,&x2,&k, 0.21093573);
    C_BESSEL_POLY(J1,&x2,&k,-0.56249985);
    C_BESSEL_POLY(J1,&x2,&k, 0.50000000);
    
    J1 = c_mul_p(x,J1,J1);
  }
  else{

    if (x->re < 0){
      x->re = -x->re;
      neg = 1;
    }
    c_complex_p(3.0,0.0,&k);
    c_div_p(&k,x,&x2);

    c_complex_p(-0.00020033 , 0.0, &f1);

    C_BESSEL_POLY(&f1,&x2,&k, 0.00113653);
    C_BESSEL_POLY(&f1,&x2,&k,-0.00249511);
    C_BESSEL_POLY(&f1,&x2,&k, 0.00017105);
    C_BESSEL_POLY(&f1,&x2,&k, 0.01659667);
    C_BESSEL_POLY(&f1,&x2,&k, 0.00000156);
    C_BESSEL_POLY(&f1,&x2,&k, 0.79788456);
    
    c_complex_p(-0.00029166 , 0.0, &t1);

    C_BESSEL_POLY(&t1,&x2,&k, 0.00079824);
    C_BESSEL_POLY(&t1,&x2,&k, 0.00074348);
    C_BESSEL_POLY(&t1,&x2,&k,-0.00637879);
    C_BESSEL_POLY(&t1,&x2,&k, 0.00005650);
    C_BESSEL_POLY(&t1,&x2,&k, 0.12499612);
    C_BESSEL_POLY(&t1,&x2,&k,-2.35619449);

    c_add_p(x, &t1, &t1);

    c_cos_p(&t1,&k);
    J1 = c_mul_p(&f1,&k,J1);
    c_sqrt_p(x,&k);
    J1 = c_div_p(J1,&k,J1);

    if(neg) {
      J1->re = -J1->re;
      x->re = -x->re;
    }

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
complex c_bessel_Y1(complex x)
{
  complex x2;
  complex Y1;
  complex f1, t1;
  int neg=0;

  if (fabs(REAL(x)) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = REAL(x)/3.0;
    IMAG(x2) = IMAG(x)/3.0;
    x2 = c_mul(x2,x2);
    
    /* the polynomial in (x/3)^2 */
    Y1 = c_complex(0.0027873 , 0.0);
    Y1 = c_add(c_mul(Y1,x2),c_complex(-0.0400976 , 0.0));
    Y1 = c_add(c_mul(Y1,x2),c_complex( 0.3123951 , 0.0));
    Y1 = c_add(c_mul(Y1,x2),c_complex(-1.3164827 , 0.0));
    Y1 = c_add(c_mul(Y1,x2),c_complex( 2.1682709 , 0.0));
    Y1 = c_add(c_mul(Y1,x2),c_complex( 0.2212091 , 0.0));
    Y1 = c_add(c_mul(Y1,x2),c_complex(-0.6366198 , 0.0));

    /* the extra added term in front */
    x2 = c_rmul(2.0/M_PI,x);
    x2 = c_mul(x2,c_log(c_rmul(0.5,x)));
    x2 = c_mul(x2,c_bessel_J1(x));

    Y1 = c_add(x2,Y1);

    Y1 = c_div(Y1,x);

  }
  else{

    if (REAL(x) < 0){
      REAL(x) = -REAL(x);
      neg = 1;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  c_bessel_Y1 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }
    x2 = c_div(c_complex(3.0,0.0),x);

    f1 = c_complex(-0.00020033 , 0.0);
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00113653 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex(-0.00249511 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00017105 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.01659667 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.00000156 , 0.0));
    f1 = c_add(c_mul(f1,x2),c_complex( 0.79788456 , 0.0));
    
    t1 = c_complex(-0.00029166 , 0.0);
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00079824 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00074348 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex(-0.00637879 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.00005650 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex( 0.12499612 , 0.0));
    t1 = c_add(c_mul(t1,x2),c_complex(-2.35619449 , 0.0));

    t1 = c_add(x, t1);

    Y1 = c_mul(f1,c_sin(t1));

    Y1 = c_div(Y1,c_sqrt(x));

    if (neg)
      IMAG(Y1) = -IMAG(Y1);

  }
  return Y1;
}

complex * c_bessel_Y1_p(complex *x, complex *Y1)
{
  complex x2, k;
  complex f1, t1;
  int neg=0;


  if (fabs(x->re) <= 3.0){
    /* find (x/3)^2 */
    REAL(x2) = x->re/3.0;
    IMAG(x2) = x->im/3.0;
    c_mul_p(&x2,&x2,&x2);
    
    /* the polynomial in (x/3)^2 */
    Y1 = c_complex_p(0.0027873 , 0.0, Y1);

    C_BESSEL_POLY(Y1,&x2,&k,-0.0400976);
    C_BESSEL_POLY(Y1,&x2,&k, 0.3123951);
    C_BESSEL_POLY(Y1,&x2,&k,-1.3164827);
    C_BESSEL_POLY(Y1,&x2,&k, 2.1682709);
    C_BESSEL_POLY(Y1,&x2,&k, 0.2212091);
    C_BESSEL_POLY(Y1,&x2,&k,-0.6366198);


    /* the extra added term in front */
    c_rmul_p(2.0/M_PI,x,&x2);
    c_rmul_p(0.5,x,&k);
    c_log_p(&k,&k);
    c_mul_p(&x2,&k,&x2);
    c_bessel_J1_p(x,&k);
    c_mul_p(&x2,&k,&x2);

    Y1 = c_add_p(&x2,Y1,Y1);

    Y1 = c_div_p(Y1,x,Y1);

  }
  else{

    if (x->re < 0){
      x->re = -x->re;
      neg = 1;
#ifdef BESSEL_WARNINGS
      fprintf(stderr,"WARNING:  c_bessel_Y1_p called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
#endif
    }
    c_complex_p(3.0,0.0,&k);
    c_div_p(&k,x,&x2);

    c_complex_p(-0.00020033 , 0.0, &f1);

    C_BESSEL_POLY(&f1,&x2,&k, 0.00113653);
    C_BESSEL_POLY(&f1,&x2,&k,-0.00249511);
    C_BESSEL_POLY(&f1,&x2,&k, 0.00017105);
    C_BESSEL_POLY(&f1,&x2,&k, 0.01659667);
    C_BESSEL_POLY(&f1,&x2,&k, 0.00000156);
    C_BESSEL_POLY(&f1,&x2,&k, 0.79788456);
    
    c_complex_p(-0.00029166 , 0.0, &t1);

    C_BESSEL_POLY(&t1,&x2,&k, 0.00079824);
    C_BESSEL_POLY(&t1,&x2,&k, 0.00074348);
    C_BESSEL_POLY(&t1,&x2,&k,-0.00637879);
    C_BESSEL_POLY(&t1,&x2,&k, 0.00005650);
    C_BESSEL_POLY(&t1,&x2,&k, 0.12499612);
    C_BESSEL_POLY(&t1,&x2,&k,-2.35619449);

    c_add_p(x, &t1, &t1);

    c_sin_p(&t1,&k);
    Y1 = c_mul_p(&f1,&k,Y1);

    c_sqrt_p(x,&k);
    Y1 = c_div_p(Y1,&k,Y1);

    if (neg) {
      Y1->im = -Y1->im;
      x->re = -x->re;
    }
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
complex c_hankel0_1(complex x)
{
  complex y;
  complex j;
  
  j = c_complex(0.0,1.0);

  y = c_add(c_bessel_J0(x),
	    c_mul(j,c_bessel_Y0(x)));

  return y;
}

complex * c_hankel0_1_p(complex *x, complex *H)
{
  complex *Y0=NULL;
  complex *j=NULL;
  
  j = c_complex_p(0.0,1.0,j);

  H = c_bessel_J0_p(x,H);
  Y0 = c_bessel_Y0_p(x,Y0);
  Y0 = c_mul_p(j,Y0,Y0);
  H = c_add_p(H,Y0,H);

  free(Y0);
  free(j);

  return H;
}

/* Type 2, order 0 Hankel function: J0(x) - j*Y0(x) */
complex c_hankel0_2(complex x)
{
  complex y;
  complex j;
  
  j = c_complex(0.0,1.0);

  y = c_sub(c_bessel_J0(x),
	    c_mul(j,c_bessel_Y0(x)));

  return y;
}

complex * c_hankel0_2_p(complex *x, complex *H)
{
  complex *Y0=NULL;
  complex *j=NULL;
  
  j = c_complex_p(0.0,1.0,j);

  H = c_bessel_J0_p(x,H);
  Y0 = c_bessel_Y0_p(x,Y0);
  Y0 = c_mul_p(j,Y0,Y0);
  H = c_sub_p(H,Y0,H);

  free(Y0);
  free(j);

  return H;
}

/* Type 1, order 1 Hankel function: J1(x) + j*Y1(x) */
complex c_hankel1_1(complex x)
{
  complex y;
  complex j;
  
  j = c_complex(0.0,1.0);

  y = c_add(c_bessel_J1(x),
	    c_mul(j,c_bessel_Y1(x)));

  return y;
}

complex * c_hankel1_1_p(complex *x, complex *H)
{
  complex *Y1=NULL;
  complex *j=NULL;
  
  j = c_complex_p(0.0,1.0,j);

  H = c_bessel_J1_p(x,H);
  Y1 = c_bessel_Y1_p(x,Y1);
  Y1 = c_mul_p(j,Y1,Y1);
  H = c_add_p(H,Y1,H);

  free(Y1);
  free(j);

  return H;
}

/* Type 2, order 1 Hankel function: J1(x) - j*Y1(x) */
complex c_hankel1_2(complex x)
{
  complex y;
  complex j;
  
  j = c_complex(0.0,1.0);

  y = c_sub(c_bessel_J1(x),
	    c_mul(j,c_bessel_Y1(x)));

  return y;
}

complex * c_hankel1_2_p(complex *x, complex *H)
{
  complex *Y1=NULL;
  complex *j=NULL;
  
  j = c_complex_p(0.0,1.0,j);

  H = c_bessel_J1_p(x,H);
  Y1 = c_bessel_Y1_p(x,Y1);
  Y1 = c_mul_p(j,Y1,Y1);
  H = c_sub_p(H,Y1,H);

  free(Y1);
  free(j);

  return H;
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

  kp = sqrt(1.0-pow(k,2.0));
  r = 1.0;
  do {
    kf = (1.0 + k) / (1.0 + kp);
    r = r*kf;
    k = 2.0*sqrt(k) / (1.0 + k);
    kp = 2.0*sqrt(kp) / (1.0 + kp);
    i++;
  } while( (fabs(kf - 1.0) > 1e-15) && ( i < 20) );

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


