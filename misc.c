/* $Id: misc.c,v 1.2 2001/09/18 21:08:55 dan Exp $ */

/*
 * Copyright (c) 2001 Dan McMahill
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

//#define DEBUG

#include <stdio.h>
#include <math.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* for rint() if its missing from math.h */
#include "mathutil.h"

#include "misc.h"

/*
 * AWG2DIA    Calculate diameter of wire from A.W.G.
 *
 *    dia = awg2dia(AWG)
 * calculates:
 *    dia = Diameter of Wire (inches)
 *  from:
 *    AWG = Wire size (American Wire Gauge)
 * 
 */

double awg2dia(double AWG)
{
  double dia;
  dia = 0.005 / ( pow(92.0 , ((AWG - 36.0)/39.0)) );
  return dia;
}


/*
 * DIA2AWG    Calculate A.W.G. of wire from diameter 
 *
 *  AWG = dia2awg(dia)
 *  calculates:
 *    AWG = Wire size (A.W.G.)
 *  from:
 *    dia = Diameter of Wire (inches)
 */

double dia2awg(double dia)
{
  double awg;

  if ( dia <= 0.0 ){
    fprintf(stderr,"dia2awg():  Wire must have a diameter > 0\n");
    exit(1);
  }

  awg = 36.0 + (39.0*(log(0.005) - log(dia)))/log(92.0);
  awg = rint(awg);

  return awg;
}


/* 
 * given the units, return the scale factor required to obtain meters
 * Ie, to convert 5 inches to meters, do
 *  5.0 * phys_units_get_sf("inches")
 */

double phys_units_get_sf(char *str)
{
  double sf;

  if (strcmp(str,"um") == 0)
    sf = 1e-6;
  else if (strcmp(str,"mm") == 0)
    sf = 1e-3;
  else if (strcmp(str,"cm") == 0)
    sf = 1e-2;
  else if (strcmp(str,"m") == 0)
    sf = 1.0;
  else if (strcmp(str,"mil") == 0)
    sf = 25.4e-6;
  else if (strcmp(str,"inches") == 0)
    sf = 25.4e-3;
  else{
    fprintf(stderr,"ERROR:  physical units string \"%s\" not recognised\n",
	    str);
    exit(1);
  }

#ifdef DEBUG
  printf("misc.c:phys_units_get_sf():  units = \"%s\", sf=%g\n",
	 str,sf);
#endif

  return sf;
}

/* 
 * given the units, return the scale factor required to obtain Hz
 * Ie, to convert 5 kHz to Hz, do
 *  5.0 * phys_units_get_sf("kHz")
 */

double freq_units_get_sf(char *str)
{
  if (strcmp(str,"mHz") == 0)
    return 1e-3;
  else if (strcmp(str,"Hz") == 0)
    return 1.0;
  else if (strcmp(str,"kHz") == 0)
    return 1e3;
  else if (strcmp(str,"MHz") == 0)
    return 1e6;
  else if (strcmp(str,"GHz") == 0)
    return 1e9;
  else if (strcmp(str,"THz") == 0)
    return 1e12;
  else{
    fprintf(stderr,"ERROR:  physical units string \"%s\" not recognised\n",
	    str);
    exit(1);
  }

}

double induct_units_get_sf(char *str)
{
  if (strcmp(str,"pH") == 0)
    return 1e-12;
  else if (strcmp(str,"nH") == 0)
    return 1e-9;
  else if (strcmp(str,"uH") == 0)
    return 1e-6;
  else if (strcmp(str,"mH") == 0)
    return 1e-3;
  else if (strcmp(str,"H") == 0)
    return 1.0;
  else if (strcmp(str,"kH") == 0)
    return 1e3;
  else{
    fprintf(stderr,"ERROR:  inductance units string \"%s\" not recognised\n",
	    str);
    exit(1);
  }

}



