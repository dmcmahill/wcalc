/* $Id: misc.c,v 1.12 2004/07/21 17:35:17 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004 Dan McMahill
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
  awg = rint(awg);

  return awg;
}

