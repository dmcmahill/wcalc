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

function [w,l,s,loss,kev,kodd]=cmlisyn(z0e,z0o,len,f,subs)
%CMLISYN    Synthesize coupled microstrip transmission line from electrical parameters
%
%  [w,l,s,loss,kev,kodd]=cmlisyn(z0e,z0o,len,f,subs)
%  calculates:
%    w     = microstrip line width (mils)
%    l     = microstrip line length (mils)
%    s     = spacing between lines (mils)
%    loss  = insertion loss (dB)
%    kev   = even mode effective relative permitivity
%    kodd  = odd mode effective relative permitivity
%
%  from:
%    z0e   = even mode characteristic impedance (ohms)
%    z0o   = odd mode characteristic impedance (ohms)
%    len   = electrical length (degrees)
%    f     = frequency (Hz)
%    subs  = substrate parameters.  See TRSUBS for details.
%
%          |<--W-->|<---S--->|<--W-->|
%           _______           _______   
%          | metal |         | metal |  
%   ----------------------------------------------
%  (  dielectric,er                      /|\     (
%   )                                 H   |       )
%  (                                     \|/     (
%   ----------------------------------------------
%   /////////////////ground///////////////////////
%
%  Part of the Filter Design Toolbox
%  See Also:  CMLICALC, MLISYN, MLICALC, SLISYN, SLICALC, TRSUBS

%  Dan McMahill, 7/17/97
%  Copyright (c) 1997 by Dan McMahill.


if(nargin ~= 5),
   error('Wrong number of arguments to CMLISYN');
end


%subs(1) = Substrate dielectric thickness (mils)
%subs(2) = Substrate relative permittivity
%subs(3) = Metal resistivity relative to copper
%subs(4) = Loss tangent of the dielectric material
%subs(5) = Metal thickness (mils)
%subs(6) = Metalization roughness
h = subs(1);
er = subs(2);

% temp value for l used while finding w and s
l = 1000;


% limits on the allowed range for w
wmin = 0.5;
wmax = 1000;

% limits on the allowed range for s
smin = 0.5;
smax = 1000;


% impedance convergence tolerance (ohms)
abstol = 1e-6;

% width relative convergence tolerance (mils) (set to 0.1 micron)
reltol = 0.1/25.4;

maxiters = 50;


%%%%%%%%%%%%%%
% Initial guess at a solution
%
z0 = sqrt(z0e*z0o);
k = (z0e - z0o)/(z0e + z0o);
AW = exp(z0*sqrt(er+1)/42.4) - 1;
F1 = 8*sqrt(AW*(7 + 4/er)/11 + (1 + 1/er)/0.81)/AW;
ai =[1 -0.301 3.209 -27.282 56.609 -37.746];
bi = [0.020 -0.623 17.192 -68.946 104.740 -16.148];
ci = [0.002 -0.347 7.171 -36.910 76.132 -51.616];

F2 = 0;
for (i = 1:6),
   F2 = F2 + ai(i) * k^(i-1);
end

F3 = 0;
for (i = 1:6),
   F3 = F3 + (bi(i) - ci(i)*(9.6 - er))*(0.6 - k)^(i-1);
end
w = h*abs(F1*F2);
s = h*abs(F1*F3);


l=100;
loss=0;
kev=1;
kodd=1;


iters = 0;
done = 0;
delta = 1e-5;

cval = 1e-12*z0e*z0o;

while(~done),
   iters = iters + 1;
   
   [ze0,zo0,ltmp,loss,kev,kodd]=cmlicalc(w,l,s,f,subs);
   
   %tmps = sprintf('ze = %g\tzo = %g\tw = %g\ts = %g\t',ze0,zo0,w,s);
   %disp(tmps);
   
   % check for convergence
   err = (ze0-z0e)^2 + (zo0-z0o)^2;
   if(err < cval),
      done = 1;
   else
      % approximate the first jacobian
      [ze1,zo1,ltmp,loss,kev,kodd]=cmlicalc(w+delta,l,s,f,subs);
      [ze2,zo2,ltmp,loss,kev,kodd]=cmlicalc(w,l,s+delta,f,subs);
      dedw = (ze1 - ze0)/delta;
      dodw = (zo1 - zo0)/delta;
      deds = (ze2 - ze0)/delta;
      dods = (zo2 - zo0)/delta;
      
      % find the determinate
      d = dedw*dods - deds*dodw;
      
      w = abs(w - ((ze0-z0e)*dods - (zo0-z0o)*deds)/d);
      s = abs(s + ((ze0-z0e)*dodw - (zo0-z0o)*dedw)/d);
      
      %tmps = sprintf('dedw = %g\tdodw = %g\tdeds = %g\tdods = %g\t',dedw,dodw,deds,dods);
      %disp(tmps);
   end
   
end



% speed of light
c = 2.997925e8;
keff = sqrt(kev*kodd);
v = c / sqrt(keff);
l = (len/360)*(v/f)/25.4e-6;


%tmps = sprintf('Took %d iterations, err = %g',iters,err);
%disp(tmps);

%tmps = sprintf('ze = %g\tzo = %g\tz0e = %g\tz0o = %g\t',ze0,zo0,z0e,z0o);
%disp(tmps);



