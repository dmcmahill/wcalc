/*
 * Copyright (C) 2020 Dan McMahill
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
#include "rods.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [L1, L2, M, k, R1, R2] =
 *       rods_calc(d1, l1, d2, l2, distance, offset, rho, freq)
 */

/* Input Arguments */

#define	D1_IN    prhs[0]
#define	L1_IN    prhs[1]

#define	D2_IN    prhs[2]
#define	L2_IN    prhs[3]

#define	DISTANCE_IN prhs[4]
#define	OFFSET_IN   prhs[5]
#define	RHO_IN      prhs[6]

#define	FREQ_IN     prhs[7]

/* Output Arguments */

#define	L1_OUT    plhs[0]
#define	L2_OUT    plhs[1]
#define	M_OUT     plhs[2]
#define	K_OUT     plhs[3]
#define	R1_OUT    plhs[4]
#define	R2_OUT    plhs[5]

#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in RODS_CALC.");                             \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in RODS_CALC.");                 \
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
    double *d1, *l1, *d2, *l2, *distance, *offset, *rho, *freq;

    unsigned int *ind_d1, *ind_l1;
    unsigned int *ind_d2, *ind_l2;
    unsigned int *ind_distance, *ind_offset;
    unsigned int *ind_rho, *ind_freq;

    /* outputs */
    double	*L1, *L2, *M, *k, *R1, *R2;
    
    /* number of rows and columns */
    unsigned int rows=1, cols=1;

    /* temp variables */
    unsigned int	m,n;

    /* do we have any vectors in our input */
    int vector=0;

    /* indices for the main loop */
    unsigned int ind=0, fixed=0;

    /* our rods for calculations */
    rods *rod;


    /*
     * Argument checking and processing
     */
    
    if (nrhs == 0) {
	mexPrintf("%s\r\n",vcid);
	return;
    }
    
    /* Check for proper number of arguments */
    if (nrhs != 8) {
	mexErrMsgTxt("wrong number of input arguments to RODS_CALC"
		     " (needs 8).");
    } 

    if (nlhs > 6) {
	mexErrMsgTxt("wrong number of output arguments to RODS_CALC"
		     " (needs <= 6).");
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
    CHECK_INPUT(D1_IN, D1, ind_d1, d1);
    CHECK_INPUT(L1_IN, L1, ind_l1, l1);

    CHECK_INPUT(D2_IN, D2, ind_d2, d2);
    CHECK_INPUT(L2_IN, L2, ind_l2, l2);

    CHECK_INPUT(DISTANCE_IN, DISTANCE, ind_distance, distance);
    CHECK_INPUT(OFFSET_IN, OFFSET, ind_offset, offset);

    CHECK_INPUT(RHO_IN, RHO, ind_rho, rho);

    CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

    /* Create matrices for the return arguments */
    L1_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    L2_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    M_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    K_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    R1_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    R2_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
    
    /* output pointers */
    L1 = mxGetPr(L1_OUT);
    L2 = mxGetPr(L2_OUT);
    M  = mxGetPr(M_OUT);
    k  = mxGetPr(K_OUT);
    R1 = mxGetPr(R1_OUT);
    R2 = mxGetPr(R2_OUT);
    
    /* the actual computation */
    rod = rods_new();

    for (ind=0; ind<(rows*cols); ind++){
	/*
	 * copy over the parameters.  Note, some of the ind_* pointers
	 * point to 'ind' and others point to 'fixed'.  This lets us pick
	 * which inputs are scalar and which are matrices easily.  (It's
	 * done when processsing the input arguments.
	 */
	rod->d1      = d1[*ind_d1];
	rod->l1      = l1[*ind_l1];

	rod->d2      = d2[*ind_d2];
	rod->l2      = l2[*ind_l2];

	rod->distance = distance[*ind_distance];
	rod->offset   = offset[*ind_offset];

	rod->rho     = rho[*ind_rho];
	rod->freq    = freq[*ind_freq];

	/* run the calculation */
	rods_calc(rod, rod->freq);

	/* extract the outputs */
	L1[ind] = rod->L1;
	L2[ind] = rod->L2;
	M[ind]  = rod->M;
	k[ind]  = rod->k;
	R1[ind] = rod->R1;
	R2[ind] = rod->R2;

    }
    
    /* clean up */
    rods_free(rod);
    
    return;
}
