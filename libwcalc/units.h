
/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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
  /* the name of the units, "nH/inch" */
  char *name;

  /* the scale factor, mks_units/unit, for example meter/inch */
  double sf;

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
    WC_UNITS_CAPACITANCE,
    WC_UNITS_CAPACITANCE_PER_LEN,
    WC_UNITS_CONDUCTANCE,
    WC_UNITS_CONDUCTANCE_PER_LEN,
    WC_UNITS_CONDUCTIVITY,
    WC_UNITS_CURRENT,
    WC_UNITS_DB,
    WC_UNITS_DB_PER_LEN,
    WC_UNITS_ELECTRIC_FIELD,
    WC_UNITS_FREQUENCY,
    WC_UNITS_INDUCTANCE,
    WC_UNITS_INDUCTANCE_PER_LEN,
    WC_UNITS_LENGTH,
    WC_UNITS_RESISTANCE,
    WC_UNITS_RESISTANCE_PER_LEN,
    WC_UNITS_RESISTIVITY,
    WC_UNITS_TIME,
    WC_UNITS_VOLTAGE
  } type;

} wc_units;


extern const wc_units_data wc_units_capacitance[];
extern const wc_units_data wc_units_conductance[];
extern const wc_units_data wc_units_current[];
extern const wc_units_data wc_units_db[];
extern const wc_units_data wc_units_frequency[];
extern const wc_units_data wc_units_inductance[];
extern const wc_units_data wc_units_length[];
extern const wc_units_data wc_units_resistance[];
extern const wc_units_data wc_units_time[];
extern const wc_units_data wc_units_voltage[];


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

int wc_savestr_to_units( const char *str,  wc_units *units );

wc_units *wc_units_new(int type);

wc_units *wc_units_resistivity_new(void);

void wc_units_free(wc_units *u);

/*
 * return how many choices for the particular units we have 
 *
 * This is for a single part of the units.  i.e., it's for something
 * like inductance, not inductance per length
 */
int wc_units_size(const wc_units_data *units);

char ** wc_units_strings_get(const wc_units_data *units);

#endif /* __UNITS_H__ */
