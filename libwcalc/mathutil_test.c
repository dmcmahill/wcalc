/* $Id: mathutil_test.c,v 1.5 2002/05/10 22:52:55 dan Exp $ */

/*
 * Copyright (c) 2002 Dan McMahill
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
      fprintf(stderr,"c_bessel_Y1_p() = %g + %g i \n",x,bp->re,bp->im);
      exit(1);
    }

  }
  printf("Pass\n");

  free(ap);
  free(bp);
  free(cp);
  return 0;
}


