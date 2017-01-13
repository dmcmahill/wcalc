// $Id$
//
// Copyright (c) 2001, 2009 Dan McMahill
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

// 
// This script compares the impedance and velocity calculated by wcalc
// to some other published formulae 
//

// Load wcalc
exec("test_wcalc.sce");

// mils to meters conversion factor
sf=25.4e-6;

// width (meters)
w_mil = linspace(4.0, 20.0, 100);
w = w_mil * sf;

// height (meters)
h = 7.0*sf;

// length (meters)
l = 1000*sf;

// metal thickness (meters)
tmet = 0.7*sf;

// resitivity (ohms/meter)
rho=1.72e-8;

// metal surface roughness (meters)
rough = 0.05*sf;

// relative dielectric constant
er = 3.38;

// loss tangent
tand=0.001;

// frequency (Hz)
f = 1.0e9;

[z0,keff,elen,loss,L,R,C,G,lc,ld,deltal,depth] = ...
    microstrip_calc(w,h,l,tmet,rho,rough,er,tand,f);


z0_aculab = 60.0 ./ sqrt(0.475*er + 0.67) .* ...
    log((4.0*h)./(0.67*(0.8*w + tmet)));


err_aculab = 100.0 * (z0_aculab - z0) ./ z0;

xselect();
xbasc();
plot2d([w_mil; w_mil], [z0 ; z0_aculab]);
xgrid(4);

xselect();
xbasc();
plot(w_mil, err_aculab);
xgrid(4);



all_s = sf*linspace(4.0, 16.0, 7);
all_s = sf * [4, 8, 15];
err_z0o_aculab = zeros(length(all_s), length(w));
for i=1:length(all_s),
  s = all_s(i);
  [z0_coupled, k, z0e, z0o, kev, kodd] = ...
      coupled_microstrip_calc(w,s, h, l, tmet, rho, rough, er, tand, f);
  
  z0o_aculab = 0.5 * (1.0 - 0.48*exp(-0.96*(s ./ h)))*2.0.*z0_aculab;
  
  err_z0o_aculab(i,:) = 100.0 * (z0o_aculab - z0o) . / z0o;
end

xselect();
xbasc();
plot(w_mil, err_z0o_aculab);
xgrid(4);
xtitle('Differential Impedance Error', 'width [mil]', 'error [%]');

