// $Id$
//
// Copyright (c) 2001 Dan McMahill
// All rights reserved.
//
// This code is derived from software written by Dan McMahill
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and//or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this software
//    must display the following acknowledgement:
//        This product includes software developed by Dan McMahill
//  4. The name of the author may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
// 
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
//  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//  SUCH DAMAGE.
//

// execute this script demonstrate the wcalc interfaces.
//
//   -->  exec('demo.sce');
//

// -----------------------------------------------
// Air Core Inductor Analysis
// -----------------------------------------------

sf   = 25.4e-3; // inches to meters conversion
N    = 5;       // # of turns
len  = 0;       // length of solenoid (we'll use fill instead)
fill = 1.2;     // length/close wound length ratio
AWG  = 22;      // wire size (AWG)
rho=1;          // not used yet
dia  = 0.14*sf; // inside diameter of coil
f    = 10e6;    // operating frequency (Hz)
flag = 1;       // use fill to find length
[L,Qf,SRF,len_out,fill_out] = ...
  air_coil_calc(N,len,fill,AWG,rho,dia,f,flag);

printf("L   = %g nH\n",L*1e9);
printf("Qf  = %g @ %g MHz\n",Qf,f/1e6);
printf("SRF = %g MHz\n",SRF/1e6);

N=3:12;
[L,Qf,SRF,len_out,fill_out] = ...
  air_coil_calc(N,len,fill,AWG,rho,dia,f,flag);
xbasc(); xselect();
plot2d(N,L*1e9);
xgrid(3);
xtitle('Inductance vs. Number of Turns.  0.14 inch I.D.','N','nH');

// -----------------------------------------------
// Air Core Inductor Synthesis
// -----------------------------------------------

// synthesize N and len with N minimized
L    = 220e-9;  // Desired inductance
AWG  = 22;      // wire size (AWG)
dia  = 0.14*sf; // inside diameter of coil
f    = 50e6;    // operating frequency (Hz)



// synthesize len with N fixed
L    = 220e-9;  // Desired inductance
N    = 8;       // Fixed # of turns
AWG  = 22;      // wire size (AWG)
dia  = 0.14;    // inside diameter of coil
f    = 50e6;    // operating frequency (Hz)

