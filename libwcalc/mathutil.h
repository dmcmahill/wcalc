
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

#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

#include <math.h>
#include <complex.h>
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

/* Bessel functions of complex arguments */
complex double cbessel_J0(complex double x);
complex double cbessel_J1(complex double x);
complex double cbessel_Y0(complex double x);
complex double cbessel_Y1(complex double x);

/* Bessel functions of real arguments */
double bessel_J0(double x);
double bessel_J1(double x);
double bessel_Y0(double x);
double bessel_Y1(double x);

/* Derivatives of Bessel functions of real arguments */
double bessel_J1p(double x);
double bessel_Y1p(double x);

/* Hankel functions */
complex double chankel0_1(complex double x);
complex double chankel0_2(complex double x);
complex double chankel1_1(complex double x);
complex double chankel1_2(complex double x);

 /*
  *  compute K(k)/K'(k) where
  * K is the complete elliptic integral of the first kind,
  * K' is the complementary complete elliptic integral of the first kind
  */
double k_over_kp(double k);

#endif /*__MATHUTIL_H__*/
