/* $Id: misc.c,v 1.4 2002/01/11 15:36:44 dan Exp $ */

/*
 * Copyright (c) 2001, 2002 Dan McMahill
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


const units_data capacitance_units[]=
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

const units_data conductance_units[]=
{
  {"uMho", 1e-6},
  {"mMho", 1e-3},
  {"Mho", 1.0},
  {"kMho", 1e3},
  {"MMho", 1e6},
  {NULL,0}
};

const units_data frequency_units[]=
{
  {"mHz", 1e-3},
  {"Hz", 1.0},
  {"kHz", 1e3},
  {"MHz", 1e6},
  {"GHz", 1e9},
  {NULL,0}
};

const units_data inductance_units[]=
{
  {"pH", 1e-12},
  {"nH", 1e-9},
  {"uH", 1e-6},
  {"mH", 1e-3},
  {"H", 1.0},
  {"kH", 1e3},
  {NULL,0}
};

const units_data length_units[]=
{
  {"nm", 1e-9},
  {"um", 1e-6},
  {"mm", 1e-3},
  {"cm", 1e-2},
  {"m", 1.0},
  {"mil", 25.4e-6},
  {"inch", 25.4e-3},
  {NULL,0}
};

const units_data resistance_units[]=
{
  {"mOhm", 1e-3},
  {"Ohm", 1.0},
  {"kOhm", 1e3},
  {"MOhm", 1e6},
  {NULL,0}
};

const units_data time_units[]=
{
  {"ps", 1e-12},
  {"ns", 1e-9},
  {"us", 1e-6},
  {"ms", 1e-3},
  {"s", 1.0},
  {NULL,0}
};

void units_autoscale(const units_data *units, 
		     double *sf, 
		     char **name,
		     double val)
{
  int i=0;

  while ((units[i].name != NULL) && ((val / units[i].sf) > 1000) ){
    i++;
  }
  
  if (units[i].name == NULL)
    i--;

#ifdef DEBUG
  printf("units_autoscale():  index = %d\n",i);
#endif

  *sf   = units[i].sf;
  *name = units[i].name;

}

int units_size(const units_data *units)
{
  int i;

  i=0;
  while (units[i].name != NULL){
    i++;
  }

  return i;
}

char ** units_strings_get(const units_data *units)
{
  int i;
  char ** u;

  i=units_size(units);
  
  /* allocate memory */
  if ( (u = malloc(i*sizeof(char *))) == NULL ) {
    fprintf(stderr,"cgi_units_strings():  malloc() failed\n");
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

int units_get_index(const units_data *units, double sf)
{
  int i;
  int found_sf=0;
  
  i=0;
  while (units[i].name != NULL){
#ifdef DEBUG
    printf("units_get_index():  is %g == %g ?\n",
	   sf,units[i].sf);
#endif
    if (units[i].sf == sf) {
      found_sf=1;
      break;
    }
    i++;
  }  

  if (!found_sf) {
    fprintf(stderr,"units_get_index():  error.  could not locate sf=%g\n",sf);
    exit(1);
  }

  /* set the menu */
#ifdef DEBUG
  printf("get_units_index():  units[%d] = %p , sf = %g)\n",
	  i,(void *) units,sf);
#endif

  return i;
}

int units_get_index_name(const units_data *units, char *name)
{
  int i;
  int found_it=0;
  
  i=0;
  while (units[i].name != NULL){
#ifdef DEBUG
    printf("units_get_index():  \"%s\" ?= \"%s\"\n",
	   name,units[i].name);
#endif
    if (strcmp(units[i].name,name)==0) {
      found_it=1;
      break;
    }
    i++;
  }  

  if (!found_it) {
    fprintf(stderr,"units_get_index_name():  error.  could not locate name=\"%s\"\n",
	    name);
    exit(1);
  }

  /* set the menu */
#ifdef DEBUG
  printf("get_units_index_name():  units[%d] = %p , name = \"%s\")\n",
	  i,(void *) units,name);
#endif

  return i;
}

/* ohm-meters */
composite_units_data * resistivity_units_new(void)
{
  composite_units_data *u;
  
  if ( (u = malloc(sizeof(composite_units_data))) == NULL ) {
    fprintf(stderr,"resistivity_units_new():  malloc failed.\n");
    exit(1);
  }

  u->type = UNITS_RESISTIVITY;

  u->nnum = 2;
  u->nden = 0;

  u->den = NULL;
  if ( (u->num = malloc(u->nnum*sizeof(units_data))) == NULL ) {
    fprintf(stderr,"resistivity_units_new():  malloc failed.\n");
    exit(1);
  }

  /* 
   * allocate memory for the arrays that specify the current
   * value of the units 
   */
  if ( (u->numi = malloc(u->nnum*sizeof(int))) == NULL ) {
    fprintf(stderr,"resistivity_units_new():  malloc failed.\n");
    exit(1);
  }
  u->deni=NULL;
  
  u->num[0] = resistance_units;
  u->num[1] = length_units;

  u->numi[0] = units_get_index(u->num[0], 1.0);
  u->numi[1] = units_get_index(u->num[1], 1.0);

  return u;
}

void resistivity_units_free(composite_units_data *u)
{
  free(u->num);
  free(u->numi);
  free(u);
}

/* initialize a resistivity composite units from a string */
int resistivity_units_set(composite_units_data *units,char *str) 
{
  char *tok;
  char *mystr;
  int i;

  assert(units->type == UNITS_RESISTIVITY);

#ifdef DEBUG
  printf("resistivity_units_set():  initializing %p from \"%s\"\n",units,str);
#endif

  /* make a local copy because strtok is destructive */
  mystr=strdup(str);
  if ( (tok = strtok(mystr,"-")) == NULL ) {
    return -1;
  }
  i=units_get_index_name(resistance_units,tok);

#ifdef DEBUG
  printf("resistivity_units_set():  found \"%s\" at index %d (mystr=\"%s\")\n",tok,i,mystr);
#endif
  units->numi[0] = i;

  if ( (tok = strtok(NULL,"-")) == NULL ) {
    return -1;
  }
  i=units_get_index_name(length_units,tok);

#ifdef DEBUG
  printf("resistivity_units_set():  found \"%s\" at index %d (mystr=\"%s\")\n",tok,i,mystr);
#endif
  units->numi[1] = i;

  return 0;
}

/* units/meters */
composite_units_data * inc_units_new(int type, const units_data *nu)
{
  composite_units_data *u;
  
  if ( (u = malloc(sizeof(composite_units_data))) == NULL ) {
    fprintf(stderr,"inc_units_new():  malloc failed.\n");
    exit(1);
  }

  u->type = type;

  u->nnum = 1;
  u->nden = 1;

  if ( (u->num = malloc(u->nnum*sizeof(units_data))) == NULL ) {
    fprintf(stderr,"inc_units_new():  malloc failed.\n");
    exit(1);
  }
  if ( (u->den = malloc(u->nden*sizeof(units_data))) == NULL ) {
    fprintf(stderr,"inc_units_new():  malloc failed.\n");
    exit(1);
  }

  /* 
   * allocate memory for the arrays that specify the current
   * value of the units 
   */
  if ( (u->numi = malloc(u->nnum*sizeof(int))) == NULL ) {
    fprintf(stderr,"inc_units_new():  malloc failed.\n");
    exit(1);
  }
  if ( (u->deni = malloc(u->nden*sizeof(int))) == NULL ) {
    fprintf(stderr,"inc_units_new():  malloc failed.\n");
    exit(1);
  }
  
  u->num[0] = nu;
  u->den[0] = length_units;

  u->numi[0] = units_get_index(u->num[0], 1.0);
  u->deni[0] = units_get_index(u->den[0], 1.0);

  return u;
}

void inc_units_free(composite_units_data *u)
{
  free(u->num);
  free(u->numi);
  free(u->den);
  free(u->deni);
  free(u);
}

/* initialize a inc_units composite units from a string */
void inc_units_set(int type, const units_data *nu, composite_units_data *units,char *str) 
{
  char *tok;
  int i;

  assert(units->type == type);

  tok = strtok(str,"/");
  i=units_get_index_name(nu,tok);

#ifdef DEBUG
  printf("inc_units_set():  found \"%s\" at index %d\n",str,i);
#endif
  units->numi[0] = i;

  tok = strtok(NULL,"/");
  i=units_get_index_name(length_units,tok);

#ifdef DEBUG
  printf("inc_units_set():  found \"%s\" at index %d\n",str,i);
#endif
  units->deni[0] = i;

}

/* calculates the scale factor and string for the composite units */
void units_update(composite_units_data *units, double *sf, char **name)
{
  double s=1.0;
  char *str;

  /* 
   * we'll need 1 for the terminating null character and 1 for
   * a '/' between numerator and denominator
   */
  size_t len=2;
  int i;

#ifdef DEBUG
  printf("units_update():  units->nnum = %d\n",units->nnum);
  printf("units_update():  units->nden = %d\n",units->nden);
  printf("units->num[0][1].name = %s\n",units->num[0][1].name);
#endif

  for(i=0; i<units->nnum; i++) {
#ifdef DEBUG
    printf("units_update():  adding numerator # %d\n",i);
    printf("                 units->numi[%d] = %d\n",i,
	   units->numi[i]);
    printf("                 sf = %.8g\tname = \"%s\"\n",
	   units->num[i][units->numi[i]].sf,
	   units->num[i][units->numi[i]].name);
#endif
    s = s * units->num[i][units->numi[i]].sf;
    /* the 1 extra is for the '-' which seperates units.  Ie, "V-s" */
    len += strlen(units->num[i][units->numi[i]].name) + 1;
  }
  
  for(i=0; i<units->nden; i++) {
    s = s / units->den[i][units->deni[i]].sf;
    /* the 1 extra is for the '-' which seperates units.  Ie, "V-s" */
    len += strlen(units->den[i][units->deni[i]].name) + 1;

#ifdef DEBUG
    printf("units_update():  adding denominator #%d\n",i);
    printf("                 sf = %.8g\tname = \"%s\"\n",
	   units->den[units->deni[i]]->sf,
	   units->den[units->deni[i]]->name);
#endif
  }

  if ( (str = malloc(len*sizeof(char))) == NULL ) {
    fprintf(stderr,"units_update():  malloc() failed\n");
    exit(1);
  }

  if (units->nnum > 0)
    sprintf(str,"%s",units->num[0][units->numi[0]].name);
  else
    sprintf(str,"1");

  for(i=1; i<units->nnum; i++)
    sprintf(str,"%s-%s",str,units->num[i][units->numi[i]].name);

  if (units->nden > 0)
    sprintf(str,"%s/%s",str,units->den[0][units->deni[0]].name);

  for(i=1; i<units->nden; i++)
    sprintf(str,"%s-%s",str,units->den[i][units->deni[i]].name);

#ifdef DEBUG
    printf("units_update():  final sf    = %g\n",s);
    printf("units_update():  final units = \"%s\"\n",str);
#endif

  *sf   = s;
  /* XXX do I need to free() the old *name? */
  *name = str;
}

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

/*
 * convert a value to a string in engineering units.
 *
 * eng_units(123.456789e6, "Hz", 2)
 * will return a pointer to "120 MHz"
 *
 * eng_units(123.456789e6, "Hz", 5)
 * will return a pointer to "123.45 MHz"
 *
 */
char *eng_units(double value, const char *base_units, double *sf)
{
  size_t len;
  static char *prefix="afpnum kMGT";
  int p;
  char *str;

  /* space for base_units */
  len  = strlen(base_units);

  /* space for prefix and '\0' terminator */
  len += 2;

  if ( (str = malloc(len*sizeof(char))) == NULL ){
    fprintf(stderr,"misc.c:eng_units():  malloc failed\n");
    exit(1);
  }
  (*sf)=1e-12;

  p=strlen(prefix);
  while( (--p > 0) && (value*(*sf) < 1.0))
    *sf = (*sf)*1e3;

  if (prefix[p] == ' '){
    sprintf(str,"%s",base_units);
  }
  else{
    sprintf(str,"%c%s",prefix[p],base_units);
  }
  return str;
}
