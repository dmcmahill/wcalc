/* $Id: coax_test.c,v 1.3 2002/05/09 23:49:57 dan Exp $ */

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

#include <math.h>
#include <stdio.h>

#include "coax.h"
#include "coax_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{
  /* inches to meters */
  double sf=0.0254;
  double a=7;
  double b[]={3.040,3.038,3.026,3.040,3.040,3.040};
  double c[]={0,1,2,1,2,3};
  int i;
  int npts = sizeof(b)/sizeof(double);
  double freq;
  char *str;

  coax_line *line;

  line = coax_new();

  line->tshield=0.002*sf;
  line->er=1.0;

  line->rho_a = 3.5e-8;
  line->rho_b = 3.5e-8;
  line->freq = 100e6;
  freq=line->freq;

  line->b=exp(1);
  line->a=1;
  line->c=0;
  coax_calc(line,freq);

  printf("Coefficient for Rosloniec (6.2) page 184 = %g\n",line->z0);
  printf("Adjusting er to produce 59.952\n");
  line->er = pow(line->z0/59.952,2.0);
  coax_calc(line,freq);
  printf("After adjustment, coefficient for Rosloniec (6.2) page 184 = %g\n",line->z0);

  printf("\n");
  printf("------------ Table 6.1 in Rosloneic ------------\n");
  printf("Z0    \ta    \tb        \tc\n");
  for (i=0; i<npts; i++) {
    /* switch from the Rosloniec notation to my notation */
    line->a = 0.5*b[i]*1e-3;
    line->b = 0.5*a*1e-3;;
    line->c = c[i]*1e-4;
    coax_calc(line,freq);
    printf("%-6.5g\t%.0e\t%.3e\t%.0e\n",line->z0,2*line->b,2*line->a,line->c);
  }
  printf("------------ ---------------------- ------------\n");
  printf("\n");

  printf("------------ ---------------------- ------------\n");
  printf("Z0    \ta    \tb        \tc     \ter\n");

  line->a=1.5e-3;
  line->b=3.5e-3;
  line->c=0;
  line->z0=75;
  line->elen=90;
  coax_syn(line,freq,COAXSYN_A);
  printf("%-6.5g\t%.0e\t%.3e\t%.0e\t%.3g\n",line->z0,2*line->b,2*line->a,line->c,line->er);
  
  line->a=1.5e-3;
  line->b=3.5e-3;
  line->c=0;
  line->z0=75;
  line->elen=90;
  coax_syn(line,freq,COAXSYN_B);
  printf("%-6.5g\t%.0e\t%.3e\t%.0e\t%.3g\n",line->z0,2*line->b,2*line->a,line->c,line->er);

  line->a=0.0015;
  line->b=0.005;
  line->c=0;
  line->z0=70;
  line->elen=90;
  coax_syn(line,freq,COAXSYN_C);
  printf("%-6.5g\t%.0e\t%.3e\t%.0e\t%.3g\n",line->z0,2*line->b,2*line->a,line->c,line->er);

  line->a=1.0e-3;
  line->b=2e-2;
  line->c=0;
  line->z0=75;
  line->elen=90;
  coax_syn(line,freq,COAXSYN_ER);
  printf("%-6.5g\t%.0e\t%.3e\t%.0e\t%.3g\n",line->z0,2*line->b,2*line->a,line->c,line->er);
  printf("------------ ---------------------- ------------\n");


  str=coax_save_string(line);
  printf("Example of coax_save_string() output:\n\"%s\"\n\n",str);
	
  return 0;
}
