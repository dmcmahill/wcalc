
/*
 * Copyright (C) 2002, 2004 Dan McMahill
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

#include "ic_microstrip.h"
#include "ic_microstrip_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{
  double w=.050;
  ic_microstrip_line *line;
  double freq;

  line = ic_microstrip_line_new();

  /* Metal resistivity */
  line->subs->rho = 1.0;
  /* Metal thickness (m) */
  line->subs->tmet = 5.0e-6;
  /* Metalization roughness */
  line->subs->rough = 1.0e-6;

  /* Microstrip width */
  line->w = 160e-6;

  /* Oxide thickness */
  line->subs->tox = 1.0e-6;
  /* Oxide relative permittivity */
  line->subs->eox = 4.0;


  /* Substrate dielectric thickness */
  line->subs->h = 250e-6;
  /* Substrate relative permittivity */
  line->subs->es = 10.0;
  /* Substrate conductivity s/m*/
  line->subs->sigmas = 100.0;

  freq = 1.0e9;

  if (argc == 9){
    printf("Using external data\n");
    /* Metal resistivity */
    line->subs->rho = 1.0;
    /* Metal thickness (m) */
    line->subs->tmet = atof(argv[1]);
    /* Metalization roughness */
    line->subs->rough = 1.0;
    
    /* Microstrip width */
    line->w = atof(argv[2]);
    
    /* Oxide thickness */
    line->subs->tox = atof(argv[3]);
    /* Oxide relative permittivity */
    line->subs->eox = atof(argv[4]);
    
    
    /* Substrate dielectric thickness */
    line->subs->h = atof(argv[5]);
    /* Substrate relative permittivity */
    line->subs->es = atof(argv[6]);
    /* Substrate conductivity s/m*/
    line->subs->sigmas = atof(argv[7]);
    
    freq = atof(argv[8]);
  }


  ic_microstrip_calc(line,freq);

  if (argc > 1) {
    printf("-------------- IC Microstrip Analysis ----------\n");
    printf("Metal width                 = %g um\n",line->w*1e6);
    printf("Oxide thickness             = %g um\n",line->subs->tox*1e6);
    printf("Oxide dielectric const.     = %g \n",line->subs->eox);
    printf("Substrate thickness         = %g um\n",line->subs->h*1e6);
    printf("Substrate dielectric const. = %g \n",line->subs->es);
    printf("Substrate conductivity      = %g 1/(ohm-cm)\n",
	   line->subs->sigmas*0.01);
    printf("Substrate resistivity       = %g ohm-cm\n",
	   100.0/(line->subs->sigmas));
    printf("Frequency                   = %g GHz\n",freq/1e9); 
    printf("-------------- ---------------------- ----------\n");
    printf("Zo (ohms)                   = %g + j%g\n",line->Ro,line->Xo);
    printf("Slow wave factor            = %g\n",line->keff);
    printf("Lmis                        = %g nH/mm\n",1e6*line->Lmis);
    printf("Rmis                        = %g Ohm/mm\n",1e-3*line->Rmis);
    printf("Cmis                        = %g pF/mm\n",1e9*line->Cmis);
    printf("Gmis                        = %g S/mm\n",1e-3*line->Gmis);
    printf("-------------- ---------------------- ----------\n");
  }

  else {
  printf("------------------------------------\n");
  w=50.0e-6;
  while(w<=120.0e-6){
    line->w = w;
    ic_microstrip_calc(line,freq);
    printf("w=%8.3f um, Zo=(%6.3f+j%6.3f) ohms, Lmis=%8.3f nH/mm,",
	   line->w*1e6,line->Ro,line->Xo,line->Lmis*1e6);
    printf("Rmis=%8.3f Ohm/mm, Cmis=%8.3f pF/mm, Gmis=%8.3f S/mm\n",
	   line->Rmis*1e-3,line->Cmis*1e9,line->Gmis*1e-3);
	   
    w = w + 10.0e-6;
  }


  printf("------------------------------------\n");
  w=50.0e-6;
  line->w = w;
  freq=100e6;
  while(freq<=10e9){
    line->freq = freq;
    ic_microstrip_calc(line,freq);
    printf("freq=%8.3f GHz, Zo=(%6.3f+j%6.3f) ohms, Lmis=%8.3f nH/mm,",
	   freq*1e-9,line->Ro,line->Xo,line->Lmis*1e6);
    printf("Rmis=%8.3f Ohm/mm, Cmis=%8.3f pF/mm, Gmis=%8.3f S/mm\n",
	   line->Rmis*1e-3,line->Cmis*1e9,line->Gmis*1e-3);
	   
    freq=1.2*freq;
  }

  printf("------------------------------------\n");
  freq=2.4e9;
  line->freq = freq;
  w=1;
  line->subs->sigmas=w;
  while(w<=100){
    line->subs->sigmas = w;
    ic_microstrip_calc(line,freq);
    printf("sigma_s=%8.3f [ohm-cm]^-1, Zo=(%6.3f+j%6.3f) ohms, Lmis=%8.3f nH/mm,",
	   w*0.01,line->Ro,line->Xo,line->Lmis*1e6);
    printf("Rmis=%8.3f Ohm/mm, Cmis=%8.3f pF/mm, Gmis=%8.3f S/mm\n",
	   line->Rmis*1e-3,line->Cmis*1e9,line->Gmis*1e-3);
	   
    w=1.2*w;
  }
  }

  ic_microstrip_line_free(line);

  return 0;
}


