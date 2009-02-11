/* $Id: parallel_rc.c,v 1.2 2009/02/10 12:10:09 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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

/* #define DEBUG_CALC */  /* debug the parallel_rc_calc() function */
/* #define DEBUG_SYN  */  /* debug the parallel_rc_syn() function  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _(String) String

#include "parallel_rc.h"
#include "parallel_rc_loadsave.h"
#include "alert.h"
#include "defaults.h"
#include "mathutil.h"
#include "misc.h"
#include "physconst.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int parallel_rc_calc(parallel_rc *b, double freq)
{
  double Q2;
  
  b->freq = freq;

  if(b->freq <= 0.0) {
    alert(_("Frequency must be > 0"));
    return -1;
  }

    
  if( b->series_to_parallel ) {
    /* Series to parallel calculation */
    if( b->use_Q) {
      b->Qp = b->Qs;
      b->Rs = 1.0 / (b->Qs * 2.0 * M_PI * b->freq * b->Cs);
    } else {
      b->Qs = 1.0 / (2.0 * M_PI * b->freq * b->Cs * b->Rs);
      b->Qp = b->Qs;
    }
    
    Q2 = pow(b->Qs, 2.0);
    b->Cp = b->Cs * Q2 / ( 1.0 + Q2);
    b->Rp = b->Rs * ( 1.0 + Q2);
  } else {
    /* Parallel to series calculation */
    if( b->use_Q) {
      b->Qs = b->Qp;
      b->Rp = b->Qp / (2.0 * M_PI * b->freq * b->Cp);
    } else {
      b->Qp = 2.0 * M_PI * b->freq * b->Cp * b->Rp;
      b->Qs = b->Qp;
    }
    
    Q2 = pow(b->Qp, 2.0);
    b->Cs = b->Cp * ( 1.0 + Q2) / Q2;
    b->Rs = b->Rp / ( 1.0 + Q2);
  }


  return 0;
}


void parallel_rc_free(parallel_rc *b)
{

  wc_units_free(b->units_Rs);
  wc_units_free(b->units_Rp);
  wc_units_free(b->units_C);
  wc_units_free(b->units_freq);

  free(b);
}


parallel_rc *parallel_rc_new()
{
  parallel_rc *newb;

  newb = (parallel_rc *) malloc(sizeof(parallel_rc));
  if(newb == NULL)
    {
      fprintf(stderr,"parallel_rc.c:parallel_rc_new(): malloc() failed\n");
      exit(1);
    }

  /* Create the units */
  newb->units_Rs = wc_units_new(WC_UNITS_RESISTANCE);
  newb->units_Rp = wc_units_new(WC_UNITS_RESISTANCE);
  newb->units_C = wc_units_new(WC_UNITS_CAPACITANCE);
  newb->units_freq = wc_units_new(WC_UNITS_FREQUENCY);

  newb->freq = 100e6;

  /* load in the defaults */
  parallel_rc_load_string(newb, default_parallel_rc);

  /* get the rest of the entries in sync */
  parallel_rc_calc(newb, newb->freq);

  return(newb);
}


