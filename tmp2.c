/* $Id$ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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

function [w,l,loss]=slisyn(z0,len,f,subs)
%SLISYN     Synthesize microstrip transmission line from electrical parameters
%
%  [w,l,loss]=slisyn(z0,len,f,subs)
%  calculates:
%    w     = microstrip line width (mils)
%    l     = microstrip line length (mils)
%    loss  = insertion loss (dB)
%
%  from:
%    z0    = characteristic impedance (ohms)
%    len   = electrical length (degrees)
%    f     = frequency (Hz)
%    subs  = substrate parameters.  See TRSUBS for details.
%
%  The dielectric thickness above the stripline is assumed
%  to be the same as below the stripline
%
%   /////////////////ground///////////////////////
%   ----------------------------------------------
%  (  dielectric,er         \/           /|\     (
%   )             -------   --            |       )
%  (             | metal | Tmet           | H    (
%   )             -------   --            |       )
%  (             <---W--->  /\           \|/     (
%   ----------------------------------------------
%   /////////////////ground///////////////////////
%
%  Part of the Filter Design Toolbox
%  See Also:  SLICALC, MLICALC

%  Dan McMahill, 8/11/97
%  Copyright (c) 1997 by Dan McMahill.


%  Reference:
%
%
%    Stanislaw Rosloniec
%    "Algorithms For Computer-Aided Design of Linear Microwave Circuits"
%    Archtech House, 1990
%    ISBN 0-89006-354-0
%    TK7876.R67 1990
%

if(nargin ~= 4),
   error('Wrong number of arguments to SLISYN');
end


%    subs(1) = Substrate total dielectric thickness (mils)
b = 25.4e-6*subs(1);
%    subs(2) = Substrate relative permittivity
er = subs(2);
%    subs(3) = Metal resistivity relative to copper
rho = subs(3);
%    subs(4) = Loss tangent of the dielectric material
tand = subs(4);
%    subs(5) = Metal thickness (mils)
t = 25.4e-6*subs(5);
%    subs(6) = Metalization roughness
rough = subs(6);


%%%%%%
% Start of stripline calculations
%%%%%%



if(t < b/1000),
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   % Thin strip case:
   %disp('Doing thin strip synthesis');
   q = exp(-29.976*pi*pi/(z0*sqrt(er)));
   N=0;
   D=0.5;
   i=0;
   No=1;
   Do=1;
   while((N-No) | (D-Do)),
      i=i+1;
      No=N;
      Do=D;
      N = N + q^(i*(i-1));
      D = D + q^(i^2);
   end
   %tmps =sprintf('Took %d iters',i);
   %disp(tmps);
   k = sqrt(q)*((N/D)^2);
   w = (2*b/pi)*log(1/k + sqrt((1/k)^2 - 1));
   %
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
else

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   % Finite strip case: 
   %
   %disp('doing finite thickness strip synthesis');
   m = 6*(b-t)./(3*b-t);
   B = exp(z0*sqrt(er)/30);
   w0 = (8*(b-t)/pi)*sqrt(B + 0.568)/(B-1);

   deltaW = (t/pi)*(1 - 0.5*log( (t./(2*b-t)).^2 + (0.0796*t/(w0 - 0.26*t)).^m ));
   w = w0 - deltaW;

   %
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end

% convert to mils
w = w/25.4e-6;

%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Electrical Length

% propagation velocity (meters/sec)
c = 2.997925e8;
v = c / sqrt(er);
l = (len/360)*(v/f)/25.4e-6;
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Run analysis to get loss

[ztmp,ltmp,loss]=slicalc(w,l,f,subs);

%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

