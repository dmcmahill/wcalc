

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

#include "ic_microstrip.h"
#include "physconst.h"

#include "mex.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*
 * function [z0,keff,elen,loss,L,R,C,G] = 
 *       ic_microstrip_calc(w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f);
 */

/* Input Arguments */

#define	W_IN	  prhs[0]
#define	L_IN	  prhs[1]
#define	TOX_IN	  prhs[2]
#define	EOX_IN	  prhs[3]
#define	H_IN	  prhs[4]
#define	ES_IN	  prhs[5]
#define	SIGMAS_IN prhs[6]
#define	TMET_IN   prhs[7]
#define	RHO_IN    prhs[8]
#define	ROUGH_IN  prhs[9]
#define	FREQ_IN	  prhs[10]

/* Output Arguments */

#define	Z0_OUT   plhs[0]
#define	KEFF_OUT plhs[1]
#define	ELEN_OUT plhs[2]
#define	LOSS_OUT plhs[3]
#define	L_OUT    plhs[4]
#define	R_OUT    plhs[5]
#define	C_OUT    plhs[6]
#define	G_OUT    plhs[7]


#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in IC_MICROSTRIP_CALC.");                        \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in IC_MICROSTRIP_CALC.");            \
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
  double *w,*l,*tox,*eox,*h,*es,*sigmas,*tmet,*rho,*rough,*freq;
  unsigned int *ind_w,*ind_l,*ind_tox,*ind_eox,*ind_h;
  unsigned int *ind_es,*ind_sigmas,*ind_tmet,*ind_rho;
  unsigned int *ind_rough,*ind_freq;

  /* outputs */
  double	*Ro, *Xo, *keff, *elen, *loss, *Lmis,*Rmis,*Cmis,*Gmis;

  /* number of rows and columns */
  unsigned int rows=1,cols=1;

  /* temp variables */
  unsigned int	m,n;

  /* do we have any vectors in our input */
  int vector=0;

  /* indices for the main loop */
  unsigned int ind=0, fixed=0;

  /* our ic_microstrip for calculations */
  ic_microstrip_line *line;


  /*
   * Arguemnt checking and processing
   */

  if (nrhs == 0) {
    mexPrintf("%s\r\n",vcid);
    return;
  }

  /* Check for proper number of arguments */
  if (nrhs != 11) {
    mexErrMsgTxt("wrong number of input arguments to IC_MICROSTRIP_CALC"
		 " (needs 11).");
  } 

  if (nlhs > 8) {
    mexErrMsgTxt("wrong number of output arguments to IC_MICROSTRIP_CALC"
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
  CHECK_INPUT(W_IN, W, ind_w, w);
  CHECK_INPUT(L_IN, L, ind_l, l);
  CHECK_INPUT(TOX_IN, TOX, ind_tox, tox);
  CHECK_INPUT(EOX_IN, EOX, ind_eox, eox);
  CHECK_INPUT(H_IN, H, ind_h, h);
  CHECK_INPUT(ES_IN, ES, ind_es, es);
  CHECK_INPUT(SIGMAS_IN, SIGMAS, ind_sigmas, sigmas);
  CHECK_INPUT(TMET_IN, TMET, ind_tmet, tmet);
  CHECK_INPUT(RHO_IN, RHO, ind_rho, rho);
  CHECK_INPUT(ROUGH_IN, ROUGH, ind_rough, rough);
  CHECK_INPUT(FREQ_IN, FREQ, ind_freq, freq);

  /* Create matrices for the return arguments */
  Z0_OUT     = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX);
  KEFF_OUT   = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX);
  ELEN_OUT   = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX);
  LOSS_OUT   = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX);
  L_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  R_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  C_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  G_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  Ro   = mxGetPr(Z0_OUT);
  Xo   = mxGetPi(Z0_OUT);
  keff = mxGetPr(KEFF_OUT);
  elen = mxGetPr(ELEN_OUT);
  loss = mxGetPr(LOSS_OUT);
  Lmis = mxGetPr(L_OUT);
  Rmis = mxGetPr(R_OUT);
  Cmis = mxGetPr(C_OUT);
  Gmis = mxGetPr(G_OUT);

  /* the actual computation */
  line = ic_microstrip_line_new();

  for (ind=0; ind<(rows*cols); ind++){
    /*
     * copy over the parameters.  Note, some of the ind_* pointers
     * point to 'ind' and others point to 'fixed'.  This lets us pick
     * which inputs are scalar and which are matrices easily.  (It's
     * done when processsing the input arguments.
     */
    line->w           = w[*ind_w];
    line->l           = l[*ind_l];
    line->subs->tox   = tox[*ind_tox];
    line->subs->eox   = eox[*ind_eox];
    line->subs->h     = h[*ind_h];
    line->subs->es    = es[*ind_es];
    line->subs->sigmas= sigmas[*ind_sigmas];
    line->subs->tmet  = tmet[*ind_tmet];
    line->subs->rho   = rho[*ind_rho];
    line->subs->rough = rough[*ind_rough];
    line->freq        = freq[*ind_freq];

    /* run the calculation */
    ic_microstrip_calc(line,line->freq);

    /* extract the outputs */
    Ro[ind]   = line->Ro;
    Xo[ind]   = line->Xo;
    keff[ind] = line->keff;
    elen[ind] = line->len;
    loss[ind] = line->loss;
    Lmis[ind] = line->Lmis;
    Rmis[ind] = line->Rmis;
    Cmis[ind] = line->Cmis;
    Gmis[ind] = line->Gmis;
  }

  /* clean up */
  ic_microstrip_line_free(line);

  return;
}
