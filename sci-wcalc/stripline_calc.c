/* $Id: stripline_calc.c,v 1.3 2002/05/10 22:53:06 dan Exp $ */

static char vcid[] = "$Id: stripline_calc.c,v 1.3 2002/05/10 22:53:06 dan Exp $";

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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "stripline.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [z0,loss,deltal] = 
 *       stripline_calc(w,h,l,tmet,rho,rough,er,tand,f);
 */

/* Input Arguments */

#define	W_IN	  prhs[0]
#define	H_IN	  prhs[1]
#define	L_IN	  prhs[2]
#define	TMET_IN   prhs[3]
#define	RHO_IN    prhs[4]
#define	ROUGH_IN  prhs[5]
#define	ER_IN	  prhs[6]
#define	TAND_IN	  prhs[7]
#define	FREQ_IN	  prhs[8]

/* Output Arguments */

#define	Z0_OUT	   plhs[0]
#define	LOSS_OUT   plhs[1]
#define	DELTAL_OUT plhs[2]


#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in STRIPLINE_CALC.");                            \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in STRIPLINE_CALC.");                \
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
  double *w,*h,*l,*tmet,*rho,*rough,*er,*tand,*freq;
  unsigned int *ind_w,*ind_h,*ind_l,*ind_tmet,*ind_rho;
  unsigned int *ind_rough,*ind_er,*ind_tand,*ind_freq;

  /* outputs */
  double	*z0,*loss,*deltal;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* our stripline for calculations */
  stripline_line *line;


  /*
   * Arguemnt checking and processing
   */

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs != 9) {
    mexErrMsgTxt("wrong number of input arguments to STRIPLINE_CALC"
		 " (needs 9).");
  } 

  if (nlhs > 4) {
    mexErrMsgTxt("wrong number of output arguments to STRIPLINE_CALC"
		 " (needs <= 4).");
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
  LOSS_OUT   = mxCreateDoubleMatrix(rows, cols, mxREAL);
  DELTAL_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  z0     = mxGetPr(Z0_OUT);
  loss   = mxGetPr(LOSS_OUT);
  deltal = mxGetPr(DELTAL_OUT);

  /* the actual computation */
  line = stripline_line_new();

  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    line->w           = w[*ind_w];
    line->l           = l[*ind_l];
    line->subs->h     = h[*ind_h];
    line->subs->tmet  = tmet[*ind_tmet];
    line->subs->rho   = rho[*ind_rho];
    line->subs->rough = rough[*ind_rough];
    line->subs->er    = er[*ind_er];
    line->subs->tand  = tand[*ind_tand];
    line->freq        = freq[*ind_freq];

    /* run the calculation */
    stripline_calc(line,line->freq);

    /* extract the outputs */
    z0[ind]     = line->Ro;
    loss[ind]   = line->loss;
    deltal[ind] = line->deltal;
  }

  /* clean up */
  stripline_line_free(line);

  return;
}
