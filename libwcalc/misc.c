
/*
 * Copyright (C) 2001, 2002, 2004, 2012 Dan McMahill
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

/* #define DEBUG */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* for rint() if its missing from math.h */
#include "mathutil.h"

#include "misc.h"
#include "physconst.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * Calculate diameter of wire from A.W.G.
 *    AWG = Wire size (American Wire Gauge)
 *    dia = Wire diameter (meters)
 */

double awg2dia(double AWG)
{
  double dia;
  dia = 0.005 / ( pow(92.0 , ((AWG - 36.0)/39.0)) );

  /* convert to meters */
  dia = INCH2M(dia);

  return dia;
}


/*
 * DIA2AWG    Calculate A.W.G. of wire from diameter 
 *    AWG = Wire size (A.W.G.)
 *    dia = Diameter of Wire (meters)
 */

double dia2awg(double dia)
{
  double awg;

  if ( dia <= 0.0 ){
    fprintf(stderr,"dia2awg():  Wire must have a diameter > 0\n");
    exit(1);
  }

  /* convert to inches */
  dia = M2INCH(dia);

  awg = 36.0 + (39.0*(log(0.005) - log(dia)))/log(92.0);

  /*
   * don't convert to integer because we want to be able to support
   * countries outside of the US where wire diameters are commonly not
   * in units of AWG.  However, since we've had several releases where
   * interface to the air coil routines has been stable, we'll keep it
   * that way and the user will have to deal with converting mm to AWG.
   * If they want to round to the nearest AWG, it is up to the user.

  /* awg = rint(awg); */

  return awg;
}

