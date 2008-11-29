/* $Id: air_coil_syn.c,v 1.8 2004/11/22 22:36:30 dan Exp $ */

static char vcid[] = "$Id: air_coil_syn.c,v 1.8 2004/11/22 22:36:30 dan Exp $";

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

#include "air_coil.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [N,LEN,FILL] = 
 *      air_coil_syn(L,N,len,fill,AWG,rho,dia,freq,flag) 
 */

/* Input Arguments */

#define	L_IN	  prhs[0]
#define	N_IN	  prhs[1]
#define	LEN_IN	  prhs[2]
#define	FILL_IN	  prhs[3]
#define	AWG_IN    prhs[4]
#define	RHO_IN    prhs[5]
#define	DIA_IN	  prhs[6]
#define	FREQ_IN	  prhs[7]
#define	FLAG_IN	  prhs[8]

/* Output Arguments */

#define	N_OUT	  plhs[0]
#define	LEN_OUT   plhs[1]
#define	FILL_OUT  plhs[2]

#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in AIR_COIL_SYN.");                              \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in AIR_COIL_SYN.");                  \
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
  double *L,*N,*AWG,*rho,*len,*dia,*freq,*fill,*flag;

  unsigned int *ind_L,*ind_N,*ind_AWG,*ind_rho,*ind_len,*ind_dia;
  unsigned int *ind_freq,*ind_fill,*ind_flag;

  /* outputs */
  double	*N_out,*len_out,*fill_out;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* we've been given the flag */
  int has_flag=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* synthesis type flag */
  int sflag;

  /* our air coil for calculations */
  air_coil_coil *coil;


  /*
   * Argument checking and processing
   */

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs == 8) 
    has_flag=0;
  else if (nrhs == 9)
    has_flag=1;
  else
    {
      mexErrMsgTxt("wrong number of input arguments to AIR_COIL_SYN"
		   " (needs 8 or 9).");
    } 

  if (nlhs > 3)
    {
      mexErrMsgTxt("wrong number of output arguments to AIR_COIL_SYN"
		   " (needs <= 3).");
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
  CHECK_INPUT(L_IN, L, ind_L, L);
  CHECK_INPUT(N_IN, N, ind_N, N);
  CHECK_INPUT(AWG_IN, AWG, ind_AWG, AWG);
  CHECK_INPUT(RHO_IN, RHO, ind_rho, rho);
  CHECK_INPUT(LEN_IN, LEN, ind_len, len);
  CHECK_INPUT(FILL_IN, FILL, ind_fill, fill);
  CHECK_INPUT(DIA_IN, DIA, ind_dia, dia);
  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

  if(has_flag){
    CHECK_INPUT(FLAG_IN,FLAG,ind_flag,flag);
  }
  else {
    if ( (flag = malloc(sizeof(double))) == NULL ) {
      fprintf(stderr,"air_coil_calc.c:  malloc() failed\n");
      exit(1);
    }
    *flag=0;
    ind_flag=&fixed;
  }

  /* Create matrices for the return arguments */
  N_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  LEN_OUT  = mxCreateDoubleMatrix(rows, cols, mxREAL);
  FILL_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  N_out    = mxGetPr(N_OUT);
  len_out  = mxGetPr(LEN_OUT);
  fill_out = mxGetPr(FILL_OUT);

  /* the actual computation */
  coil = air_coil_new();

  coil->use_fill = 0;

  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    coil->L        = L[*ind_L];
    coil->Nf       = N[*ind_N];
    coil->len      = len[*ind_len];
    coil->fill     = fill[*ind_fill];
    coil->AWGf     = AWG[*ind_AWG];
    coil->rho      = rho[*ind_rho];
    coil->dia      = dia[*ind_dia];
    coil->freq     = freq[*ind_freq];

    if(flag[*ind_flag]==0) {
      sflag = AIRCOILSYN_NMIN;
    }
    else {
      sflag = AIRCOILSYN_NFIX;
    }

    /* run the calculation */
    air_coil_syn(coil,coil->freq,sflag);

    /* extract the outputs */
    N_out[ind]    = coil->Nf;
    len_out[ind]  = coil->len;
    fill_out[ind] = coil->fill;
  }

  /* clean up */
  air_coil_free(coil);

  if (!has_flag) 
    free(flag);

  return;
}
