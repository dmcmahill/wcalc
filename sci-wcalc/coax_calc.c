/* $Id: coax_calc.c,v 1.1 2001/12/09 15:14:40 dan Exp $ */

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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "coax.h"
#include "physconst.h"

#include "mex.h"

/*
 * [z0,elen,loss,L,R,C,G] = 
 *    coax_calc(a,b,c,tshield,rho_a,rho_b,er,tand,len,freq);
 */

/* Input Arguments */

#define	A_IN	  prhs[0]
#define	B_IN	  prhs[1]
#define	C_IN	  prhs[2]
#define	T_IN      prhs[3]
#define	RHO_A_IN  prhs[4]
#define	RHO_B_IN  prhs[5]
#define	ER_IN	  prhs[6]
#define	TAND_IN	  prhs[7]
#define LEN_IN    prhs[8]
#define	FREQ_IN	  prhs[9]

/* Output Arguments */

#define	Z0_OUT    plhs[0]
#define	ELEN_OUT  plhs[1]
#define	LOSS_OUT  plhs[2]
#define	L_OUT	  plhs[3]
#define	R_OUT     plhs[4]
#define	C_OUT     plhs[5]
#define	G_OUT     plhs[6]


#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in COAX_CALC.");                                 \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in COAX_CALC.");                     \
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
  double *a,*b,*c,*t,*rho_a,*rho_b,*er,*tand,*len,*freq;

  unsigned int *ind_a,*ind_b,*ind_c,*ind_t,*ind_rho_a,*ind_rho_b;
  unsigned int *ind_er,*ind_tand,*ind_len;
  unsigned int *ind_freq;

  /* outputs */
  double	*z0,*elen,*loss,*L,*R,*C,*G;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* our air line for calculations */
  coax_line *line;


  /*
   * Arguemnt checking and processing
   */

  /* Check for proper number of arguments */
  if (nrhs != 10) 
    {
      mexErrMsgTxt("wrong number of input arguments to COAX_CALC"
		   " (needs 10).");
    } 

  if (nlhs > 7)
    {
      mexErrMsgTxt("wrong number of output arguments to COAX_CALC"
		   " (needs <= 7).");
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
  CHECK_INPUT(A_IN, A, ind_a, a);
  CHECK_INPUT(B_IN, B, ind_b, b);
  CHECK_INPUT(C_IN, C, ind_c, c);
  CHECK_INPUT(T_IN, T, ind_t, t);
  CHECK_INPUT(RHO_A_IN, RHO_A, ind_rho_a, rho_a);
  CHECK_INPUT(RHO_B_IN, RHO_B, ind_rho_b, rho_b);
  CHECK_INPUT(ER_IN, ER, ind_er, er);
  CHECK_INPUT(TAND_IN, TAND, ind_tand, tand);
  CHECK_INPUT(LEN_IN, LEN, ind_len, len);
  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);


  /* Create matrices for the return arguments */
  Z0_OUT   = mxCreateDoubleMatrix(rows, cols, mxREAL);
  ELEN_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  LOSS_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  L_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  R_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  C_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  G_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  z0   = mxGetPr(Z0_OUT);
  elen = mxGetPr(ELEN_OUT);
  loss = mxGetPr(Z0_OUT);
  L    = mxGetPr(L_OUT);
  R    = mxGetPr(R_OUT);
  C    = mxGetPr(C_OUT);
  G    = mxGetPr(G_OUT);

  /* the actual computation */
  line = coax_new();

  /* XXX get from flags */


  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    line->a       = a[*ind_a];
    line->b       = b[*ind_b];
    line->c       = c[*ind_c];
    line->tshield = t[*ind_t];
    line->rho_a   = rho_a[*ind_rho_a];
    line->rho_b   = rho_a[*ind_rho_b];
    line->er      = len[*ind_er];
    line->tand    = len[*ind_tand];
    line->len     = len[*ind_len];
    line->freq    = freq[*ind_freq];

    /* run the calculation */
    coax_calc(line,line->freq);

    /* extract the outputs */
    L[ind]    = line->L;
    R[ind]    = line->R;
    C[ind]    = line->C;
    G[ind]    = line->G;
    z0[ind]   = line->z0;
    elen[ind] = line->elen;
    loss[ind] = line->loss;
  }

  /* clean up */
  coax_free(line);

  return;
}
