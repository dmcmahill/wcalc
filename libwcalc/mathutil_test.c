
/*
 * Copyright (C) 2002 Dan McMahill
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

#include "config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "mathutil.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{

  double w,x,y,z;
  complex a,b,c;
  complex *ap=NULL, *bp=NULL, *cp=NULL;

  char *sep="---------------------------";

  w=1.0;
  x=2.0;
  y=3.0;
  z=4.0;

  printf("%s c_div %s\n",sep,sep);
  c=c_div(c_complex(w,x),c_complex(y,z));
  ap = c_complex_p(w,x,ap);
  bp = c_complex_p(y,z,bp);
  cp = c_div_p(ap,bp,cp);
  printf("(%g + %g i) / (%g + %g i) = (%g + %g i)\n",w,x,y,z,REAL(c),IMAG(c));
  printf("(%g + %g i) / (%g + %g i) = (%g + %g i)\n",w,x,y,z,REAL_P(cp),IMAG_P(cp));

  printf("%s c_bessel_* %s\n",sep,sep);
  for(x = -10; x<10; x=x+0.05) {
    a  = c_complex(x,0.0);
    ap = c_complex_p(x,0.0,ap);

    b  = c_bessel_J0(a);
    bp = c_bessel_J0_p(ap,bp);

    if( (REAL(b) != bp->re) || (IMAG(b) != bp->im) ) {
      fprintf(stderr,"c_bessel_J0(%g + 0.0 i) != c_bessel_J0_p()\n",x);
      exit(1);
    }

    b  = c_bessel_Y0(a);
    bp = c_bessel_Y0_p(ap,bp);

    if( (REAL(b) != bp->re) || (IMAG(b) != bp->im) ) {
      fprintf(stderr,"c_bessel_Y0(%g + 0.0 i) != c_bessel_Y0_p()\n",x);
      exit(1);
    }

    b  = c_bessel_J1(a);
    bp = c_bessel_J1_p(ap,bp);

    if( (REAL(b) != bp->re) || (IMAG(b) != bp->im) ) {
      fprintf(stderr,"c_bessel_J1(%g + 0.0 i) = %g + %g i !=",x,REAL(b),IMAG(b));
      fprintf(stderr,"c_bessel_J1_p(%g + 0.0 i) = %g + %g i \n",x,bp->re,bp->im);
      exit(1);
    }


    b  = c_bessel_Y1(a);
    bp = c_bessel_Y1_p(ap,bp);

    if( (REAL(b) != bp->re) || (IMAG(b) != bp->im) ) {
      fprintf(stderr,"c_bessel_Y1(%g + 0.0 i) = %g + %g i !=",x,REAL(b),IMAG(b));
      fprintf(stderr,"c_bessel_Y1_p(%g + 0.0 i) = %g + %g i \n",x,bp->re,bp->im);
      exit(1);
    }

  }
  printf("Pass\n");

  free(ap);
  free(bp);
  free(cp);
  return 0;
}


