/*      $Id: mathutil.h,v 1.3 2001/09/18 19:44:04 dan Exp $ */

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

#define REAL(x)     ((x).re)
#define IMAG(x)     ((x).im)

/* addition (a+b) */
complex c_add(complex a, complex b);

/* subtraction (a-b) */
complex c_sub(complex a, complex b);

/* multiplication (a*b) */
complex c_mul(complex a, complex b);

/* multiplication by a real number (x*a)*/
complex c_rmul(double x, complex a);

/* division (x/y) */
complex c_div(complex x, complex y);

/* complex conjugate (a*) */
complex c_conj(complex a);

/* magnitude (|z|) */
double  c_abs(complex a);

/* angle */
double c_arg(complex x);

/* create complex number from its real and imaginary parts */
complex c_complex(double real, double imag);

/* square-root */
complex c_sqrt(complex x);

/* complex trig functions */
complex c_cos(complex x);
complex c_cosh(complex x);
complex c_sin(complex x);
complex c_sinh(complex x);
complex c_tan(complex x);
complex c_cot(complex x);
complex c_tanh(complex x);
complex c_coth(complex x);

/* logarithm */
complex c_log(complex x);

/* Bessel functions */
complex c_bessel_J0(complex x);
complex c_bessel_J1(complex x);
complex c_bessel_Y0(complex x);
complex c_bessel_Y1(complex x);

/* Hankel functions */
complex c_hankel0_1(complex x);
complex c_hankel0_2(complex x);
complex c_hankel1_1(complex x);
complex c_hankel1_2(complex x);


#endif /*__MATHUTIL_H__*/
