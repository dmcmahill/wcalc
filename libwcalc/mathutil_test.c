
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
  complex double a,b,c;

  char *sep="---------------------------";

  w=1.0;
  x=2.0;
  y=3.0;
  z=4.0;

  printf("%s c_div %s\n",sep,sep);
  c = (w + x*I) / (y + z*I);
  a = w + x*I;
  b = y + z*I;
  c = a / b;
  printf("(%g + %g i) / (%g + %g i) = (%g + %g i)\n",w,x,y,z,creal(c),cimag(c));
  printf("(%g + %g i) / (%g + %g i) = (%g + %g i)\n",w,x,y,z,creal(c),cimag(c));

  printf("%s cbessel_* %s\n",sep,sep);
  for(x = -10; x<10; x=x+0.05) {
    a  = x + 0.0i;

    b  = cbessel_J0(a);

    b  = cbessel_Y0(a);

    b  = cbessel_J1(a);

    b  = cbessel_Y1(a);
  }
  printf("Pass\n");
  return 0;
}


