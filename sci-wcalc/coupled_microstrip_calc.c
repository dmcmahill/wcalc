

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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "coupled_microstrip.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [z0,k,z0e,z0o,kev,kodd] = 
 *       coupled_microstrip_calc(w,s,h,l,tmet,rho,rough,er,tand,f);
 */

/* Input Arguments */

#define	W_IN	  prhs[0]
#define	S_IN	  prhs[1]
#define	H_IN	  prhs[2]
#define	L_IN	  prhs[3]
#define	TMET_IN   prhs[4]
#define	RHO_IN    prhs[5]
#define	ROUGH_IN  prhs[6]
#define	ER_IN	  prhs[7]
#define	TAND_IN	  prhs[8]
#define	FREQ_IN	  prhs[9]

/* Output Arguments */

#define	Z0_OUT	   plhs[0]
#define	K_OUT	   plhs[1]
#define	Z0E_OUT	   plhs[2]
#define	Z0O_OUT	   plhs[3]
#define	KEV_OUT    plhs[4]
#define	KODD_OUT   plhs[5]
#define	LOSSE_OUT  plhs[6]
#define	LOSSO_OUT  plhs[7]
#define	DELTAE_OUT  plhs[8]
#define	DELTAO_OUT  plhs[9]


#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in COUPLED_MICROSTRIP_CALC.");                   \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in COUPLED_MICROSTRIP_CALC.");       \
    (z) = NULL;                                                       \
}                                                                     \
else if(m*n > 1) {                                                    \
 vector=1;                                                            \
 rows=m;                                                              \
 cols=n;                                                              \
 (z) = &ind;                                                          \
}                                                                     \
else {                                                                \
 (z) = &fixed;                                                        \
}                                                                     \
(v) = mxGetPr(x);                                                     \


/*
 * Note that the V4_COMPAT is for compiling with Matlab.
 * For Scilab, we always want to avoid the `const' part.
 */
void mexFunction(
                 int nlhs,
		 mxArray *plhs[],
                 int nrhs,
#if !defined(V4_COMPAT) && !defined(SCI_MEX)
		 const mxArray *prhs[]
#else
		 mxArray *prhs[]
#endif
		 )
{
  /* inputs */
  double *w,*s,*h,*l,*tmet,*rho,*rough,*er,*tand,*freq;
  unsigned int *ind_w,*ind_s,*ind_h,*ind_l,*ind_tmet,*ind_rho;
  unsigned int *ind_rough,*ind_er,*ind_tand,*ind_freq;

  /* outputs */
  double	*z0,*k,*z0e,*z0o,*kev,*kodd, *losse, *losso, *deltale, *deltalo;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* our coupled_microstrip for calculations */
  coupled_microstrip_line *line;


  /*
   * Arguemnt checking and processing
   */

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs != 10) {
    mexErrMsgTxt("wrong number of input arguments to COUPLED_MICROSTRIP_CALC"
		 " (needs 10).");
  } 

  if (nlhs > 10) {
    mexErrMsgTxt("wrong number of output arguments to COUPLED_MICROSTRIP_CALC"
		 " (needs <= 10).");
  }
  
  
  /* 
   * Check the dimensions of the inputs and assign pointers to 
   * the various parameters 
   *
   * CHECK_INPUT( input_variable,
   *              variable_name (for messages),
   *              index,
   *              pointer)
   */
  CHECK_INPUT(W_IN, W, ind_w, w);
  CHECK_INPUT(S_IN, S, ind_s, s);
  CHECK_INPUT(H_IN, H, ind_h, h);
  CHECK_INPUT(L_IN, L, ind_l, l);
  CHECK_INPUT(TMET_IN, TMET, ind_tmet, tmet);
  CHECK_INPUT(RHO_IN, RHO, ind_rho, rho);
  CHECK_INPUT(ROUGH_IN, ROUGH, ind_rough, rough);
  CHECK_INPUT(ER_IN, ER, ind_er, er);
  CHECK_INPUT(TAND_IN, TAND, ind_tand, tand);
  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

  /* Create matrices for the return arguments */
  Z0_OUT     = mxCreateDoubleMatrix(rows, cols, mxREAL);
  K_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  Z0E_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  Z0O_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  KEV_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  KODD_OUT   = mxCreateDoubleMatrix(rows, cols, mxREAL);
  LOSSE_OUT  = mxCreateDoubleMatrix(rows, cols, mxREAL);
  LOSSO_OUT  = mxCreateDoubleMatrix(rows, cols, mxREAL);
  DELTAE_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  DELTAO_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);

  /* output pointers */
  z0     = mxGetPr(Z0_OUT);
  k      = mxGetPr(K_OUT);
  z0e    = mxGetPr(Z0E_OUT);
  z0o    = mxGetPr(Z0O_OUT);
  kev    = mxGetPr(KEV_OUT);
  kodd   = mxGetPr(KODD_OUT);
  losse  = mxGetPr(LOSSE_OUT);
  losso  = mxGetPr(LOSSO_OUT);
  deltale= mxGetPr(DELTAE_OUT);
  deltalo= mxGetPr(DELTAO_OUT);

  /* the actual computation */
  line = coupled_microstrip_line_new();

  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    line->w           = w[*ind_w];
    line->s           = s[*ind_s];
    line->l           = l[*ind_l];
    line->subs->h     = h[*ind_h];
    line->subs->tmet  = tmet[*ind_tmet];
    line->subs->rho   = rho[*ind_rho];
    line->subs->rough = rough[*ind_rough];
    line->subs->er    = er[*ind_er];
    line->subs->tand  = tand[*ind_tand];
    line->freq        = freq[*ind_freq];

    /* run the calculation */
    coupled_microstrip_calc(line,line->freq);

    /* extract the outputs */
    z0[ind]     = line->z0;
    k[ind]      = line->k;
    z0e[ind]    = line->z0e;
    z0o[ind]    = line->z0o;
    kev[ind]    = line->kev;
    kodd[ind]   = line->kodd;
    losse[ind]  = line->loss_ev;
    losso[ind]  = line->loss_odd;
    deltale[ind]= line->deltale;
    deltalo[ind]= line->deltalo;
  }

  /* clean up */
  coupled_microstrip_line_free(line);

  return;
}
