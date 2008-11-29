/* $Id: ic_microstrip_syn.c,v 1.5 2004/11/09 12:55:46 dan Exp $ */

static char vcid[] = "$Id: ic_microstrip_syn.c,v 1.5 2004/11/09 12:55:46 dan Exp $";

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
 * function [w, h, tox, l] =
 *       ic_microstrip_syn(z0,elen,w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f,flag);
 */

/* Input Arguments */

#define	Z0_IN	  prhs[0]
#define	ELEN_IN	  prhs[1]
#define	W_IN	  prhs[2]
#define	L_IN	  prhs[3]
#define	TOX_IN	  prhs[4]
#define	EOX_IN	  prhs[5]
#define	H_IN	  prhs[6]
#define	ES_IN	  prhs[7]
#define	SIGMAS_IN prhs[8]
#define	TMET_IN   prhs[9]
#define	RHO_IN    prhs[10]
#define	ROUGH_IN  prhs[11]
#define	FREQ_IN	  prhs[12]
#define	FLAG_IN	  prhs[13]

/* Output Arguments */

#define	W_OUT	   plhs[0]
#define	H_OUT	   plhs[1]
#define	TOX_OUT	   plhs[2]
#define	L_OUT	   plhs[3]


#define CHECK_INPUT(x,y,z,v)                                          \
m = mxGetM(x);                                                        \
n = mxGetN(x);                                                        \
if (!mxIsNumeric(x) ||                                                \
     mxIsComplex(x) ||                                                \
     mxIsSparse(x)  ||                                                \
    !mxIsDouble(x) ) {                                                \
    mexErrMsgTxt(#y " must be a real valued vector "                  \
                    "in IC_MICROSTRIP_SYN.");                         \
}                                                                     \
if (vector && (m>1 || n>1) &&                                         \
    ( (m != rows) || (n != cols) ) ){                                 \
    mexErrMsgTxt(#y ": all vector/matrix inputs must have the same"   \
                    " dimensions in IC_MICROSTRIP_SYN.");             \
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
  double *z0,*elen,*w,*l,*tox,*eox,*h,*es,*sigmas;
  double *tmet,*rho,*rough,*freq,*flag;
  unsigned int *ind_z0,*ind_elen,*ind_w,*ind_l,*ind_tox,*ind_eox,*ind_h;
  unsigned int *ind_es,*ind_sigmas,*ind_tmet,*ind_rho;
  unsigned int *ind_rough,*ind_freq,*ind_flag;

  /* outputs */
  double *w_out, *l_out, *h_out, *tox_out;

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
  if (nrhs != 14) {
    mexErrMsgTxt("wrong number of input arguments to IC_MICROSTRIP_SYN"
		 " (needs 14).");
  } 

  if (nlhs > 4) {
    mexErrMsgTxt("wrong number of output arguments to IC_MICROSTRIP_SYN"
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
  CHECK_INPUT(Z0_IN, Z0, ind_z0, z0);
  CHECK_INPUT(ELEN_IN, ELEN, ind_elen, elen);
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
  CHECK_INPUT(FLAG_IN, FLAG, ind_flag, flag);

  /* Create matrices for the return arguments */
  W_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  L_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  H_OUT      = mxCreateDoubleMatrix(rows, cols, mxREAL);
  TOX_OUT    = mxCreateDoubleMatrix(rows, cols, mxREAL);
  
  /* output pointers */
  w_out  = mxGetPr(W_OUT);
  l_out  = mxGetPr(L_OUT);
  h_out  = mxGetPr(H_OUT);
  tox_out= mxGetPr(TOX_OUT);

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

    line->Ro          = z0[*ind_z0];
    line->len         = elen[*ind_elen];
    
    if ((flag[*ind_flag] > 2) || (flag[*ind_flag] < 0) ) {
      mexErrMsgTxt("flag must be one of 0,1,2 in IC_MICROSTRIP_SYN");
    }

    /* run the calculation */
    ic_microstrip_syn(line,line->freq,flag[*ind_flag]);

    /* extract the outputs */
    w_out[ind]  = line->w;
    l_out[ind]  = line->l;
    h_out[ind]  = line->subs->h;
    tox_out[ind]= line->subs->tox;
  }

  /* clean up */
  ic_microstrip_line_free(line);

  return;
}
