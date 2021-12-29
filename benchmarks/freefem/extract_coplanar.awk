#!/usr/bin/awk -f
#
# Copyright (c) 2021 Dan McMahill
# All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
 
# extracts the geometries from the stdio-wcalc/stripline.dat
# test config file and formats for the FreeFEM analysis.

BEGIN {
    sf = 1e3;
    print "* W   H   Tmet   Er";
}

/^[ \t]*$/ { print ; next; }

/^[ \t]*[*]/ {print ; next; }

# coplanar_calc(w,s,h,l,tmet,rho,rough,er,tand,with_ground,f);
/^[ \t]*coplanar_calc/ {
    sf = 1.0;
    h = 4;
    i = 0;
    while (sf*$h < 1.0 && i < 6) { sf = 10*sf; i=i+1;}
    i = 0;
    while (sf*$h > 100.0 && i < 6) { sf = 0.1*sf; i=i+1;}

    W = sf*$2;
    S = sf*$3;
    H = sf*$h;
    Tmet = sf*$6;
    er = $9;
    gnd = $11;

    printf("%g    %g    %g    %g    %g    %d\n", W, S, H, Tmet, er, gnd);
    next;
}

