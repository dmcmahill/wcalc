/*      $Id: mathutil.h,v 1.4 2002/05/04 20:19:16 dan Exp $ */

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

#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

#include <math.h>
#include "config.h"

#ifndef M_E
#define M_E             2.7182818284590452354   /* e */
#endif
#ifndef M_PI
#define M_PI            3.14159265358979323846  /* pi */
#endif

double coth(double x);

#ifndef HAVE_RINT
#define rint(x)  (ceil((x) - 0.5))
#endif

typedef struct COMPLEX
{
  double re,im;
} complex;

#define REAL(x)     (x.re)
#define IMAG(x)     (x.im)

/* addition (a+b) */
complex c_add(complex a, complex b);
complex * c_add_p(complex *a, complex *b, complex *c);

/* subtraction (a-b) */
complex c_sub(complex a, complex b);
complex * c_sub_p(complex *a, complex *b, complex *c);

/* multiplication (a*b) */
complex c_mul(complex a, complex b);
complex * c_mul_p(complex *a, complex *b, complex *c);

/* multiplication by a real number (x*a)*/
complex c_rmul(double x, complex a);
complex * c_rmul_p(double x, complex *a, complex *b);

/* division (x/y) */
complex c_div(complex x, complex y);
complex * c_div_p(complex *x, complex *y, complex *z);

/* complex conjugate (a*) */
complex c_conj(complex a);
complex * c_conj_p(complex *a, complex *b);

/* magnitude (|z|) */
double  c_abs(complex a);
double c_abs_p(complex *a);

/* angle */
double c_arg(complex x);
double c_arg_p(complex *a);

/* create complex number from its real and imaginary parts */
complex c_complex(double real, double imag);
complex * c_complex_p(double real, double imag, complex *z);

/* create a new unititialized complex variable */
complex * c_complex_new(void);

/* square-root */
complex c_sqrt(complex x);
complex * c_sqrt_p(complex *a, complex *b);

/* complex trig functions */
complex c_cos(complex x);
complex * c_cos_p(complex *a, complex *b);

complex c_cosh(complex x);
complex * c_cosh_p(complex *a, complex *b);

complex c_sin(complex x);
complex * c_sin_p(complex *a, complex *b);

complex c_sinh(complex x);
complex * c_sinh_p(complex *a, complex *b);

complex c_tan(complex x);
complex * c_tan_p(complex *a, complex *b);

complex c_cot(complex x);
complex * c_cot_p(complex *a, complex *b);

complex c_tanh(complex x);
complex * c_tanh_p(complex *a, complex *b);

complex c_coth(complex x);
complex * c_coth_p(complex *a, complex *b);

/* logarithm */
complex c_log(complex x);
complex * c_log_p(complex *x, complex *z);

/* Bessel functions of complex arguments */
complex c_bessel_J0(complex x);
complex * c_bessel_J0_p(complex *x, complex *J0);
complex c_bessel_J1(complex x);
complex * c_bessel_J1_p(complex *x, complex *J1);
complex c_bessel_Y0(complex x);
complex * c_bessel_Y0_p(complex *x, complex *Y0);
complex c_bessel_Y1(complex x);
complex * c_bessel_Y1_p(complex *x, complex *Y1);

/* Bessel functions of real arguments */
double bessel_J0(double x);
double bessel_J1(double x);
double bessel_Y0(double x);
double bessel_Y1(double x);

/* Derivatives of Bessel functions of real arguments */
double bessel_J1p(double x);
double bessel_Y1p(double x);

/* Hankel functions */
complex c_hankel0_1(complex x);
complex * c_hankel0_1_p(complex *x, complex *H);

complex c_hankel0_2(complex x);
complex * c_hankel0_2_p(complex *x, complex *H);

complex c_hankel1_1(complex x);
complex * c_hankel1_1_p(complex *x, complex *H);

complex c_hankel1_2(complex x);
complex * c_hankel1_2_p(complex *x, complex *H);


#endif /*__MATHUTIL_H__*/
