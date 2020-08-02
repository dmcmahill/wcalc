

/*
 * Copyright (C) 2009 Dan McMahill
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "alert.h"
#include "bars.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [L1, L2, M, k] =
 *       bars_calc(a, b, l1, d, c, l2, E, P, l3, freq)
 */

/* Input Arguments */

#define	A_IN     prhs[0]
#define	B_IN     prhs[1]
#define	L1_IN    prhs[2]

#define	D_IN     prhs[3]
#define	C_IN     prhs[4]
#define	L2_IN    prhs[5]

#define	E_IN     prhs[6]
#define	P_IN     prhs[7]
#define	L3_IN    prhs[8]

#define	FREQ_IN  prhs[9]

/* Output Arguments */

#define	L1_OUT    plhs[0]
#define	L2_OUT    plhs[1]
#define	M_OUT     plhs[2]
#define	K_OUT     plhs[3]

#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in BARS_CALC.");                             \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in BARS_CALC.");                 \
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
    double *a, *b, *l1, *d, *c, *l2, *P, *E, *l3, *freq;

    unsigned int *ind_a,*ind_b,*ind_l1;
    unsigned int *ind_d,*ind_c,*ind_l2;
    unsigned int *ind_P,*ind_E,*ind_l3;
    unsigned int *ind_freq;

    /* outputs */
    double	*L1, *L2, *M, *k;
    
    /* number of rows and columns */
    unsigned int rows=1, cols=1;

    /* temp variables */
    unsigned int	m,n;

    /* do we have any vectors in our input */
    int vector=0;

    /* indices for the main loop */
    unsigned int ind=0, fixed=0;

    /* our bars for calculations */
    bars *bar;


    /*
     * Argument checking and processing
     */
    
    if (nrhs == 0) {
	mexPrintf("%s\r\n",vcid);
	return;
    }
    
    /* Check for proper number of arguments */
    if (nrhs != 10) {
	mexErrMsgTxt("wrong number of input arguments to BARS_CALC"
		     " (needs 10).");
    } 

    if (nlhs > 4) {
	mexErrMsgTxt("wrong number of output arguments to BARS_CALC"
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
    CHECK_INPUT(A_IN, A, ind_a, a);
    CHECK_INPUT(B_IN, B, ind_b, b);
    CHECK_INPUT(L1_IN, L1, ind_l1, l1);

    CHECK_INPUT(D_IN, D, ind_d, d);
    CHECK_INPUT(C_IN, C, ind_c, c);
    CHECK_INPUT(L2_IN, L2, ind_l2, l2);

    CHECK_INPUT(E_IN, E, ind_E, E);
    CHECK_INPUT(P_IN, P, ind_P, P);
    CHECK_INPUT(L3_IN, L3, ind_l3, l3);

    CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

    /* Create matrices for the return arguments */
    L1_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    L2_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    M_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    K_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    
    /* output pointers */
    L1 = mxGetPr(L1_OUT);
    L2 = mxGetPr(L2_OUT);
    M  = mxGetPr(M_OUT);
    k  = mxGetPr(K_OUT);
    
    /* the actual computation */
    bar = bars_new();

    for (ind=0; ind<(rows*cols); ind++){
	/*
	 * copy over the parameters.  Note, some of the ind_* pointers
	 * point to 'ind' and others point to 'fixed'.  This lets us pick
	 * which inputs are scalar and which are matrices easily.  (It's
	 * done when processsing the input arguments.
	 */
	bar->a       = a[*ind_a];
	bar->b       = b[*ind_b];
	bar->l1      = l1[*ind_l1];

	bar->d       = d[*ind_d];
	bar->c       = c[*ind_c];
	bar->l2      = l2[*ind_l2];

	bar->E       = E[*ind_E];
	bar->P       = P[*ind_P];
	bar->l3      = l3[*ind_l3];

	bar->freq    = freq[*ind_freq];

	/* run the calculation */
	bars_calc(bar, bar->freq);

	/* extract the outputs */
	L1[ind] = bar->L1;
	L2[ind] = bar->L2;
	M[ind]  = bar->M;
	k[ind]  = bar->k;

    }
    
    /* clean up */
    bars_free(bar);
    
    return;
}
