/* $Id: mathutil.c,v 1.1 2001/10/05 00:37:34 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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


#include <math.h>
#include <stdio.h>
#include "mathutil.h"

double coth(double x)
{
  return (1.0/tanh(x));
}


/*
 * The methods for the complex math are from :
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

complex c_add(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a) + REAL(b);
  IMAG(z) = IMAG(a) + IMAG(b);
  return z;
}

complex c_sub(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a) - REAL(b);
  IMAG(z) = IMAG(a) - IMAG(b);
  return z;
}


complex c_mul(complex a, complex b)
{
  complex z;
  REAL(z) = REAL(a)*REAL(b) - IMAG(a)*IMAG(b);
  IMAG(z) = IMAG(a)*REAL(b) + REAL(a)*IMAG(b);
  return z;
}

complex c_rmul(double x, complex a)
{
  complex z;
  REAL(z) = x*REAL(a);
  IMAG(z) = x*IMAG(a);
  return z;
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

complex c_conj(complex a)
{
  complex z;
  REAL(z) = REAL(a);
  IMAG(z) = -IMAG(a);
  return z;
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

/* returns the angle */
double c_arg(complex x)
{
  double phi;
  phi = atan2(IMAG(x),REAL(x));
  return phi;
}

complex c_complex(double re, double im)
{
  complex z;
  REAL(z) = re;
  IMAG(z) = im;
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

complex c_cosh(complex x)
{
  complex y;

  y = c_complex( cosh(REAL(x))*cos(IMAG(x)),
		 sinh(REAL(x))*sin(IMAG(x)) );
  return y;
}

complex c_sin(complex x)
{
  complex y;

  y = c_complex( cosh(IMAG(x))*sin(REAL(x)),
		 sinh(IMAG(x))*cos(REAL(x)) );
  return y;
}

complex c_sinh(complex x)
{
  complex y;

  y = c_complex( sinh(REAL(x))*cos(IMAG(x)),
		 cosh(REAL(x))*sin(IMAG(x)) );
  return y;
}

complex c_tan(complex x)
{
  complex y;

  y = c_div(c_sin(x),c_cos(x));

  return y;
}

complex c_cot(complex x)
{
  complex y;

  y = c_div(c_cos(x),c_sin(x));

  return y;
}

complex c_tanh(complex x)
{
  complex y;

  y = c_div(c_sinh(x),c_cosh(x));

  return y;
}

complex c_coth(complex x)
{
  complex y;

  y = c_div(c_cosh(x),c_sinh(x));

  return y;
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
  return (J0);
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
      fprintf(stderr,"WARNING:  c_bessel_Y0 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
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
  return (Y0);
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
  return (J1);
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
      fprintf(stderr,"WARNING:  c_bessel_Y1 called with negative real arg.\n");
      fprintf(stderr,"          This is untested.\n");
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
  return (Y1);
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
  return (J0);
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
  return (Y0);
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
  return (J1);
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
  return (Y1);
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




