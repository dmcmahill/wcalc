// $Id$
//
// Copyright (c) 2004 Dan McMahill
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
//    documentation and/or other materials provided with the distribution.
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

exec test_wcalc.sce;
lines(0);

disp('**** air_coil_calc ****');
N=5;              // number of turns
len=0;            // we'll use the fill parameter instead
fill=1.2;         // the coil is 20% longer than the minimum
AWG=22;           // wire size
rho=1.72e-8;      // resistivity of copper (ohm-m)
dia=0.25*0.0254;  // 0.25 inches inside diameter
f=10e6;           // 10 MHz operation
flag=1;           // use fill to calculate length
[L,Q,SRF,len_out,fill_out,Lmax] = air_coil_calc(N,len,fill,AWG,rho,dia,f,flag);
disp(sprintf('L=%g nH',L*1e9));
disp(sprintf('Q=%g at %g MHz',Q,f/1e6));
disp(sprintf('Self Resonant Freq=%g MHz',SRF/1e6));
disp(sprintf('Length=%g inches',len_out/0.0254));
disp(sprintf('Fill=%g',fill_out));
disp(sprintf('Lmax=%g nH',Lmax*1e9));

disp('**** air_coil_syn ****');
L=220e-9;         // we want 220 nH
N=0;              // we'll synthesize the number of turns
len=0;            // we'll synthesize the length/fill
fill=0;           // we'll synthesize the length/fill
AWG=22;           // wire size
rho=1.72e-8;      // Resistivity of copper (ohm-m)
dia=0.25*0.0254;  // 0.25 inches inside diameter
f=10e6;           // 10 MHz operation
flag=0;           // synthesize for minimum number of turns
[Nout,len_out,fill_out,Q,SRF,Lm] = air_coil_syn(L,N,len,fill,AWG,rho,dia,f,flag);
disp(sprintf('Number of turns = %g ',Nout));
disp(sprintf('Length=%g inches',len_out/0.0254));
disp(sprintf('Fill=%g',fill_out));
disp(sprintf('Inside diameter = %g inches ',dia/0.0254));
disp(sprintf('L=%g nH',L*1e9));
disp(sprintf('Q=%g at %g MHz',Q,f/1e6));
disp(sprintf('Self Resonant Freq=%g MHz',SRF/1e6));
disp(sprintf('Closewound inductance =%g nH',Lm*1e9));

quit

