/* $Id: units.h,v 1.1 2004/07/18 16:09:38 dan Exp $ */

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

#ifndef __UNITS_H__
#define __UNITS_H__

/*
 * Structs used by the units module
 */


/*
 * This is an association between a name like "nH" and a scale factor
 * like 1e-9 H/nH
 */
typedef struct WC_UNITS_DATA
{
  /* the name of the units, "inch" */
  char *name;

  /* the scale factor, mks_units/unit, for example meter/inch */
  double sf;
} wc_units_data;

/*
 * This is the main struct which should be used to store all units in
 * the various frontends.
 */
typedef struct WC_UNITS
{
  /*
   * the various units which appear in the numerator and denominator 
   * These are arrays of pointers to structs which actually hold all
   * the units strings and units scale factors.
   */
  const wc_units_data **num;
  const wc_units_data **den;

  /* 
   * indices which actually define the current scale factors for our
   * units.  For example, we might have
   * num[numi[0]]->name = "nH"
   * den[deni[0]]->name = "mm"
   */
  int *numi, *deni;

  /* number of numerator and denominator terms */
  int nnum, nden;

  /* what sort of units are these? */
  enum {
    WC_UNITS_FREQUENCY,
    WC_UNITS_RESISTIVITY,
    WC_UNITS_CAPACITANCE_PER_LEN,
    WC_UNITS_CONDUCTANCE_PER_LEN,
    WC_UNITS_INDUCTANCE_PER_LEN,
    WC_UNITS_RESISTANCE_PER_LEN,
  } type;

} wc_units;


extern const wc_units_data wc_units_capacitance[];
extern const wc_units_data wc_units_conductance[];
extern const wc_units_data wc_units_frequency[];
extern const wc_units_data wc_units_inductance[];
extern const wc_units_data wc_units_length[];
extern const wc_units_data wc_units_resistance[];
extern const wc_units_data wc_units_time[];


/*
 * Produce the scale factor mks_units / unit
 */
double wc_units_to_sf( const wc_units *units );

/*
 * Produce a string like "nH/mm" from the supplied units.
 * Memory is allocated for the string.
 */
char *wc_units_to_str( const wc_units *units );

/*
 * Produce a string that can be saved in a cookie or file which is the
 * indices in the wc_units struct.  This makes it easy to load in
 * without having issues over i18n.  Examples of output are:
 *
 * "1", "1-3-2-0".
 *
 * The integers are simply the numerator unit indices followed by the
 * denominator units indices.
 */
char *wc_units_to_savestr( const wc_units *units );


wc_units *wc_units_new(int type);

wc_units *wc_units_resistivity_new(void);

void wc_units_free(wc_units *u);

#endif /* __UNITS_H__ */
