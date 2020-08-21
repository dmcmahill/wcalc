/*
 * Copyright (C) 2001, 2002, 2004, 2020 Dan McMahill
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"
#include "units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

const wc_units_data wc_units_capacitance[]=
{
  {"fF", 1e-15},
  {"pF", 1e-12},
  {"nF", 1e-9},
  {"uF", 1e-6},
  {"mF", 1e-3},
  {"F", 1.0},
  {"kF", 1e3},
  {NULL,0}
};

const wc_units_data wc_units_conductance[]=
{
  {"uMho", 1e-6},
  {"mMho", 1e-3},
  {"Mho", 1.0},
  {"kMho", 1e3},
  {"MMho", 1e6},
  {NULL,0}
};

const wc_units_data wc_units_current[]=
{
  {"fA", 1e-15},
  {"pA", 1e-12},
  {"nA", 1e-9},
  {"uA", 1e-6},
  {"mA", 1e-3},
  {"A", 1.0},
  {"kA", 1e3},
  {"MA", 1e6},
  {NULL,0}
};

const wc_units_data wc_units_db[]=
{
  {"dB", 1.0},
  {NULL,0}
};

const wc_units_data wc_units_frequency[]=
{
  {"mHz", 1e-3},
  {"Hz", 1.0},
  {"kHz", 1e3},
  {"MHz", 1e6},
  {"GHz", 1e9},
  {NULL,0}
};

const wc_units_data wc_units_inductance[]=
{
  {"pH", 1e-12},
  {"nH", 1e-9},
  {"uH", 1e-6},
  {"mH", 1e-3},
  {"H", 1.0},
  {"kH", 1e3},
  {NULL,0}
};

const wc_units_data wc_units_length[]=
{
  {"nm", 1e-9},
  {"um", 1e-6},
  {"mm", 1e-3},
  {"cm", 1e-2},
  {"m", 1.0},
  {"mil", 25.4e-6},
  {"inch", 25.4e-3},
  {"feet", 0.3048},
  {"yard", 0.9144},
  {NULL,0}
};

const wc_units_data wc_units_resistance[]=
{
  {"uOhm", 1e-6},
  {"mOhm", 1e-3},
  {"Ohm", 1.0},
  {"kOhm", 1e3},
  {"MOhm", 1e6},
  {NULL,0}
};

const wc_units_data wc_units_time[]=
{
  {"as", 1e-18},
  {"fs", 1e-15},
  {"ps", 1e-12},
  {"ns", 1e-9},
  {"us", 1e-6},
  {"ms", 1e-3},
  {"s", 1.0},
  {NULL,0}
};

const wc_units_data wc_units_voltage[]=
{
  {"pV", 1e-12},
  {"nV", 1e-9},
  {"uV", 1e-6},
  {"mV", 1e-3},
  {"V", 1.0},
  {"kV", 1.0e3},
  {"MV", 1.0e6},
  {NULL,0}
};

/*
 * Produce the scale factor mks_units / unit
 */
double wc_units_to_sf( const wc_units *units )
{
  double sf = 1.0;
  int i;

  for(i=0; i<units->nnum; i++)
    sf *= units->num[i][units->numi[i]].sf ;

  for(i=0; i<units->nden; i++)
    sf /= units->den[i][units->deni[i]].sf ;

#ifdef DEBUG
    printf("wc_units_to_sf():  final scale = %g\n", sf);
#endif
    
    return sf;
}

/*
 * Produce a string like "nH/mm" from the supplied units.
 * Memory is allocated for the string.
 */
char *wc_units_to_str( const wc_units *units )
{
  char *str;

  /* 
   * we'll need 1 for the terminating null character and 1 for
   * a '/' between numerator and denominator
   */
  size_t len = 2;
  int i;

  /* make a first pass through to figure out storage size */

  /* numerator units */
  for(i=0; i<units->nnum; i++) {
    /* the 1 extra is for the '-' which seperates units.  Ie, "V-s" */
    len += strlen(units->num[i][units->numi[i]].name) + 1;
  }
  
  /* denominator units */
  for(i=0; i<units->nden; i++) {
    /* the 1 extra is for the '-' which seperates units.  Ie, "V-s" */
    len += strlen(units->den[i][units->deni[i]].name) + 1;
  }

  /* allocate memory for string */
  if ( (str = malloc(len*sizeof(char))) == NULL ) {
    fprintf(stderr,"wc_units_to_str():  malloc() failed\n");
    exit(1);
  }

  /* and populate the string */
  if (units->nnum > 0)
    sprintf(str, "%s", units->num[0][units->numi[0]].name);
  else
    sprintf(str, "1");

  for(i=1; i<units->nnum; i++)
    sprintf(str, "%s-%s", str, units->num[i][units->numi[i]].name);

  if (units->nden > 0)
    sprintf(str, "%s/%s", str, units->den[0][units->deni[0]].name);

  for(i=1; i<units->nden; i++)
    sprintf(str, "%s-%s", str,units->den[i][units->deni[i]].name);

#ifdef DEBUG
    printf("wc_units_to_str():  final units = \"%s\"\n", str);
#endif
    
    return str;
}


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
char *wc_units_to_savestr( const wc_units *units )
{
  char *str;
  size_t len;
  int i, j;

  /*
   * assume that we'll never have more than 1000 choices for a
   * particular type of unit (i.e. no more that 1000 different length
   * units) 
   */

  /*
   * 4 characters plus 1 for the "-" per unit plus 1 for the
   * terminating \0 
   */
  len = (units->nnum + units->nden) * 5 + 1;

  /* allocate memory for string */
  if ( (str = malloc(len*sizeof(char))) == NULL ) {
    fprintf(stderr,"wc_units_to_savestr():  malloc() failed\n");
    exit(1);
  }

  /* and populate the string */
  if( units->nnum > 0)
    sprintf(str, "%d", units->numi[0]);
  else
    str[0] = '\0';

  for(i=1; i < units->nnum; i++)
    sprintf(str, "%s-%d", str, units->numi[i]);

  if ( (units->nnum == 0) && (units->nden > 0) ) {
    sprintf(str, "units->deni[0]");
    j = 1;
  } else {
    j = 0;
  }

  for(i=j; i < units->nden; i++)
    sprintf(str, "%s-%d", str, units->deni[i]);

#ifdef DEBUG
    printf("wc_units_to_savestr():  final units = \"%s\"\n", str);
#endif
    
    return str;
}

/*
 * Take a save string and use it to initialize a wc_units.
 */
int wc_savestr_to_units( const char *str,  wc_units *units )
{
  char *tmps1, *tmps2;
  size_t len;
  int i, j;

#ifdef DEBUG
  printf("wc_savestr_to_units(\"%s\", %p)\n", str, units);
#endif

  /* do a quick sanity check of the string */
  len = 0;
  for(i = 0 ; i < strlen(str) ; i++) {
    if (str[i] == '-')
      len++;
    if ( str[i] != '-' && ( str[i] < '0' || str[i] > '9' ) ) {
      alert("wc_savestr_to_units():  Illegal character \"%c\"\n"
	    "found in string \"%s\"\n", str[i], str);
      return -1;
    }
  }

#ifdef DEBUG
  printf("wc_savestr_to_units():  nnum = %d, nden = %d\n", 
		units->nnum, units->nden);
#endif
  if (len != units->nnum + units->nden - 1) {
    alert("wc_savestr_to_units():  Found %d indices in \"%s\""
	  "but I needed %d.\n", len + 1,
	  units->nnum + units->nden - 1);
      return -1;
  }

  tmps1 = tmps2 = strdup(str);


  for(j = 0; j < units->nnum; j++) {
    /* figure out where the '-' character is */
    i = 0;
    while(tmps2[i] != '-' && tmps2[i] != '\0')
      i++;
    
    tmps2[i] = '\0';
    units->numi[j] = atoi(tmps2);

#ifdef DEBUG
    printf("tmps2 = %s and gave units->numi[%d] = %d\n",
	   tmps2, j, units->numi[j]);
#endif
    tmps2 = &tmps2[i+1];
  }

  for(j = 0; j < units->nden; j++) {
    /* figure out where the '-' character is */
    i = 0;
    while(tmps2[i] != '-' && tmps2[i] != '\0')
      i++;
    
    tmps2[i] = '\0';
    units->deni[j] = atoi(tmps2);
#ifdef DEBUG
    printf("tmps2 = %s and gave units->deni[%d] = %d\n",
	   tmps2, j, units->deni[j]);
#endif
    tmps2 = &tmps2[i+1];
  }
  
  free( tmps1 );


  units->sf = wc_units_to_sf( units );
  if( units->name != NULL )
    free(units->name);
  units->name = wc_units_to_str( units );
  
  return 0;
}

wc_units * wc_units_new(int type)
{
  wc_units *u;
  int i;

  if ( (u = malloc(sizeof(wc_units))) == NULL ) {
    fprintf(stderr, "wc_units_new():  malloc failed.\n");
    exit(1);
  }

  u->type = type;

  /* fill in the number of terms in numerator and denominator */
  switch (type) {
  case WC_UNITS_CAPACITANCE:
  case WC_UNITS_CONDUCTANCE:
  case WC_UNITS_CURRENT:
  case WC_UNITS_DB:
  case WC_UNITS_FREQUENCY:
  case WC_UNITS_INDUCTANCE:
  case WC_UNITS_LENGTH:
  case WC_UNITS_RESISTANCE:
  case WC_UNITS_TIME:
  case WC_UNITS_VOLTAGE:
    u->nnum = 1;
    u->nden = 0;
    break;

  case WC_UNITS_RESISTIVITY:
    u->nnum = 2;
    u->nden = 0;
    break;

  case WC_UNITS_CAPACITANCE_PER_LEN:
  case WC_UNITS_CONDUCTANCE_PER_LEN:
  case WC_UNITS_CONDUCTIVITY:
  case WC_UNITS_DB_PER_LEN:
  case WC_UNITS_ELECTRIC_FIELD:
  case WC_UNITS_INDUCTANCE_PER_LEN:
  case WC_UNITS_RESISTANCE_PER_LEN:
    u->nnum = 1;
    u->nden = 1;
    break;

  default:
    fprintf(stderr, "wc_units_new():  Invalid type %d.\n", type);
    exit(1);
    break;

  }

  /* 
   * allocate memory for the numerator and denominator units and
   * indices 
   */
  u->num = NULL;
  u->numi = NULL;

  if (u->nnum > 0) {
    if ( (u->num = malloc(u->nnum*sizeof(wc_units_data))) == NULL ) {
      fprintf(stderr, "wc_units_new():  malloc failed.\n");
      exit(1);
    }
    if ( (u->numi = malloc(u->nnum*sizeof(int))) == NULL ) {
      fprintf(stderr, "wc_units_new():  malloc failed.\n");
      exit(1);
    }
  }
  
  u->den = NULL;
  u->deni = NULL;
  if (u->nden > 0) {
    if ( (u->den = malloc(u->nden*sizeof(wc_units_data))) == NULL ) {
      fprintf(stderr, "wc_units_new():  malloc failed.\n");
      exit(1);
    }
    if ( (u->deni = malloc(u->nden*sizeof(int))) == NULL ) {
      fprintf(stderr, "wc_units_new():  malloc failed.\n");
      exit(1);
    }
  }

  /* initialize the units */
  switch (type) {
  case WC_UNITS_CAPACITANCE:
    u->num[0] = wc_units_capacitance;
    break;

  case WC_UNITS_CAPACITANCE_PER_LEN:
    u->num[0] = wc_units_capacitance;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_CONDUCTANCE:
    u->num[0] = wc_units_conductance;
    break;

  case WC_UNITS_CONDUCTANCE_PER_LEN:
    u->num[0] = wc_units_conductance;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_CONDUCTIVITY:
    u->num[0] = wc_units_conductance;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_CURRENT:
    u->num[0] = wc_units_current;
    break;

  case WC_UNITS_DB:
    u->num[0] = wc_units_db;
    break;

  case WC_UNITS_DB_PER_LEN:
    u->num[0] = wc_units_db;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_ELECTRIC_FIELD:
    u->num[0] = wc_units_voltage;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_FREQUENCY:
    u->num[0] = wc_units_frequency;
    break;

  case WC_UNITS_INDUCTANCE:
    u->num[0] = wc_units_inductance;
    break;

  case WC_UNITS_INDUCTANCE_PER_LEN:
    u->num[0] = wc_units_inductance;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_LENGTH:
    u->num[0] = wc_units_length;
    break;

  case WC_UNITS_RESISTANCE:
    u->num[0] = wc_units_resistance;
    break;

  case WC_UNITS_RESISTANCE_PER_LEN:
    u->num[0] = wc_units_resistance;
    u->den[0] = wc_units_length;
    break;

  case WC_UNITS_RESISTIVITY:
    u->num[0] = wc_units_resistance;
    u->num[1] = wc_units_length;
    break;

  case WC_UNITS_TIME:
    u->num[0] = wc_units_time;
    break;

  case WC_UNITS_VOLTAGE:
    u->num[0] = wc_units_voltage;
    break;

  default:
    fprintf(stderr, "wc_units_new():  Invalid type %d.\n", type);
    exit(1);
    break;

  }

  /* initialize the indices */
  for( i = 0; i < u->nnum; i++ ) 
    u->numi[i] = 0;

  for( i = 0; i < u->nden; i++ ) 
    u->deni[i] = 0;

  u->sf = wc_units_to_sf( u );
  u->name = wc_units_to_str( u );

  return u;
}


void wc_units_free(wc_units *u)
{
  if( u == NULL ) {
    alert("Warning:  wc_units_free() attempted to free a NULL pointer\n");
    return ;
  }

  if( u->num != NULL )
    free( u->num );
  
  if( u->den != NULL )
    free( u->den );

  if( u->numi != NULL )
    free( u->numi );

  if( u->deni != NULL )
    free( u->deni );

  free(u);
}

int wc_units_size(const wc_units_data *units)
{
  int i;

  i=0;
  while (units[i].name != NULL){
    i++;
  }

  return i;
}

char ** wc_units_strings_get(const wc_units_data *units)
{
  int i;
  char ** u;

  i = wc_units_size(units);
  
  /* allocate memory */
  if ( (u = malloc(i*sizeof(char *))) == NULL ) {
    fprintf(stderr,"wc_units_strings_get():  malloc() failed\n");
    exit(1);
  }

  /* copy over pointers to the strings */
  i=0;
  while (units[i].name != NULL){
    u[i] = units[i].name;
    i++;
  }

  return u;
}
