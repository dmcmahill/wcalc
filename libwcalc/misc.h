/* $Id: misc.h,v 1.1 2001/10/05 00:37:35 dan Exp $ */

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

#ifndef __MISC_H__
#define __MISC_H__

typedef struct UNITS_DATA
{
  /* the name of the units, "inch" */
  char *name;

  /* the scale factor, mks_units/unit, for example meter/inch */
  double sf;
} units_data;

typedef struct COMPOSITE_UNITS_DATA
{
  /* the various units which appear in the numerator and denominator */
  const units_data **num;
  const units_data **den;

  /* indices */
  int *numi, *deni;

  /* number of numerator and denominator terms */
  int nnum, nden;

  /* what sort of units are these? */
  enum {UNITS_RESISTIVITY} type;

} composite_units_data;

extern const units_data capacitance_units[];
extern const units_data frequency_units[];
extern const units_data inductance_units[];
extern const units_data length_units[];
extern const units_data resistance_units[];
extern const units_data time_units[];
int units_get_index(const units_data *units, double sf);
composite_units_data * resistivity_units_new(void);
void resistivity_units_free(composite_units_data *u);
void resistivity_units_set(composite_units_data *units,char *str);
void units_update(composite_units_data *units, double *sf, char **name);

double dia2awg(double dia);
double awg2dia(double AWG);
double phys_units_get_sf(char *str);
double freq_units_get_sf(char *str);
double induct_units_get_sf(char *str);
char * eng_units(double value, const char *base_units, double *sf);

#endif /*__MISC_H__*/
