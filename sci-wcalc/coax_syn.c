/* $Id: coax_syn.c,v 1.5 2002/06/12 11:30:36 dan Exp $ */

static char vcid[] = "$Id: coax_syn.c,v 1.5 2002/06/12 11:30:36 dan Exp $";

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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "coax.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * [a,b,c,er,len] = 
 *    coax_syn(z0,elen,a,b,c,tshield,rho_a,rho_b,er,tand,freq,flag);
 */

/* Input Arguments */

#define	Z0_IN	  prhs[0]
#define ELEN_IN   prhs[1]
#define	A_IN	  prhs[2]
#define	B_IN	  prhs[3]
#define	C_IN	  prhs[4]
#define	T_IN      prhs[5]
#define	RHO_A_IN  prhs[6]
#define	RHO_B_IN  prhs[7]
#define	ER_IN	  prhs[8]
#define	TAND_IN	  prhs[9]
#define	FREQ_IN	  prhs[10]
#define	FLAG_IN	  prhs[11]

/* Output Arguments */

#define	A_OUT     plhs[0]
#define	B_OUT     plhs[1]
#define	C_OUT     plhs[2]
#define	ER_OUT	  plhs[3]
#define	LEN_OUT   plhs[4]



#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in COAX_SYN.");                                  \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in COAX_SYN.");                      \
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
  double *a,*b,*c,*t,*rho_a,*rho_b,*er,*tand,*freq;
  double *z0, *elen;
  double *flag;
  unsigned int *ind_a,*ind_b,*ind_c,*ind_t,*ind_rho_a,*ind_rho_b;
  unsigned int *ind_er,*ind_tand,*ind_freq;
  unsigned int *ind_z0,*ind_elen;
  unsigned int *ind_flag;

  /* outputs */
  double *a_out,*b_out,*c_out,*er_out,*len_out;

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

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs != 12) 
    {
      mexErrMsgTxt("wrong number of input arguments to COAX_SYN"
		   " (needs 12).");
    } 

  if (nlhs > 5)
    {
      mexErrMsgTxt("wrong number of output arguments to COAX_SYN"
		   " (needs <= 5).");
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
  CHECK_INPUT(Z0_IN, Z0, ind_z0, z0);
  CHECK_INPUT(ELEN_IN, ELEN, ind_elen, elen);
  CHECK_INPUT(A_IN, A, ind_a, a);
  CHECK_INPUT(B_IN, B, ind_b, b);
  CHECK_INPUT(C_IN, C, ind_c, c);
  CHECK_INPUT(T_IN, T, ind_t, t);
  CHECK_INPUT(RHO_A_IN, RHO_A, ind_rho_a, rho_a);
  CHECK_INPUT(RHO_B_IN, RHO_B, ind_rho_b, rho_b);
  CHECK_INPUT(ER_IN, ER, ind_er, er);
  CHECK_INPUT(TAND_IN, TAND, ind_tand, tand);
  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);
  CHECK_INPUT(FLAG_IN, FLAG, ind_flag, flag);


  /* Create matrices for the return arguments */
  A_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  B_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  C_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  ER_OUT   = mxCreateDoubleMatrix(rows, cols, mxREAL);
  LEN_OUT  = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  a_out   = mxGetPr(A_OUT);
  b_out   = mxGetPr(B_OUT);
  c_out   = mxGetPr(C_OUT);
  er_out  = mxGetPr(ER_OUT);
  len_out = mxGetPr(LEN_OUT);

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
    line->z0      = z0[*ind_z0];
    line->elen    = elen[*ind_elen];
    line->a       = a[*ind_a];
    line->b       = b[*ind_b];
    line->c       = c[*ind_c];
    line->tshield = t[*ind_t];
    line->rho_a   = rho_a[*ind_rho_a];
    line->rho_b   = rho_a[*ind_rho_b];
    line->er      = er[*ind_er];
    line->tand    = tand[*ind_tand];
    line->freq    = freq[*ind_freq];

    /* run the calculation */
    coax_syn(line,line->freq,flag[*ind_flag]);

    /* extract the outputs */
    a_out[ind]    = line->a;
    b_out[ind]    = line->b;
    c_out[ind]    = line->c;
    er_out[ind]   = line->er;
    len_out[ind]  = line->len;
  }

  /* clean up */
  coax_free(line);

  return;
}
