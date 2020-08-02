

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

#include "parallel_rc.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * [Cs, Rs, Qs, Cp, Rp, Qp] = parallel_rc_s2p(Cs, Rs, Qs, freq, flag)
 */

/* Input Arguments */

#define	CS_IN    prhs[0]
#define	RS_IN    prhs[1]
#define	QS_IN    prhs[2]
#define	FREQ_IN  prhs[3]
#define	FLAG_IN  prhs[4]

/* Output Arguments */

#define	CS_OUT     plhs[0]
#define	RS_OUT     plhs[1]
#define	QS_OUT     plhs[2]
#define	CP_OUT     plhs[3]
#define	RP_OUT     plhs[4]
#define	QP_OUT     plhs[5]

#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in PARALLEL_RC_S2P.");                             \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in PARALLEL_RC_S2P.");                 \
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
  double *Cs, *Rs, *Qs, *freq, *flag;

  unsigned int *ind_Cs,*ind_Rs,*ind_Qs,*ind_freq,*ind_flag;

  /* outputs */
  double	*Cpo, *Rpo, *Qpo, *Cso, *Rso, *Qso;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* we've been given the fill/len flag */
  int has_flag=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* our air coil for calculations */
  parallel_rc *net;


  /*
   * Arguemnt checking and processing
   */

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs == 4) 
    has_flag=0;
  else if (nrhs == 5)
    has_flag=1;
  else
    {
      mexErrMsgTxt("wrong number of input arguments to PARALLEL_RC_S2P"
		   " (needs 4 or 5).");
    } 

  if (nlhs > 6)
    {
      mexErrMsgTxt("wrong number of output arguments to PARALLEL_RC_S2P"
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
  CHECK_INPUT(CS_IN, CS, ind_Cs, Cs);
  CHECK_INPUT(RS_IN, RS, ind_Rs, Rs);
  CHECK_INPUT(QS_IN, QS, ind_Qs, Qs);

  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

  if(has_flag){
    CHECK_INPUT(FLAG_IN,FLAG,ind_flag,flag);
  }
  else {
    if ( (flag = malloc(sizeof(double))) == NULL ) {
      fprintf(stderr,"parallel_rc_s2p.c:  malloc() failed\n");
      exit(1);
    }
    *flag=0;
    ind_flag=&fixed;
  }

  /* Create matrices for the return arguments */
  CS_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  RS_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  QS_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);

  CP_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  RP_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  QP_OUT = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  Cso = mxGetPr(CS_OUT);
  Rso = mxGetPr(RS_OUT);
  Qso = mxGetPr(QS_OUT);

  Cpo = mxGetPr(CP_OUT);
  Rpo = mxGetPr(RP_OUT);
  Qpo = mxGetPr(QP_OUT);

  /* the actual computation */
  net = parallel_rc_new();
  net->series_to_parallel = 1;

  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    net->Cs = Cs[*ind_Cs];
    net->Rs = Rs[*ind_Rs];
    net->Qs = Qs[*ind_Qs];
    net->freq = freq[*ind_freq];
    net->use_Q = flag[*ind_flag];    

    /* run the calculation */
    parallel_rc_calc(net, net->freq);

    /* extract the outputs */
    Cso[ind] = net->Cs;
    Rso[ind] = net->Rs;
    Qso[ind] = net->Qs;

    Cpo[ind] = net->Cp;
    Rpo[ind] = net->Rp;
    Qpo[ind] = net->Qp;


  }

  /* clean up */
  parallel_rc_free(net);

  if (!has_flag) 
    free(flag);

  return;
}
