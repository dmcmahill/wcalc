##

## Copyright (c) 2009 Dan McMahill
## All rights reserved.
##
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
## 

# Note, we let the user pick some arbitrary directory for the PNG files
# because we simply don't know how the user will organize their web server.
# For example, someone might install this in their personal public_html
# directory and need to look for images in ~randomuser/images, or perhaps
# this is installed by the webadmin under /wcalc.

TGIF_SRCS=	\
		air_coil.obj \
		alert_fig.obj \
		bars_fig.obj \
		coax.obj \
		coax_model.obj \
		coplanar.obj \
		coplanar_model.obj \
		coupled_microstrip.obj \
		coupled_stripline.obj \
		ic_microstrip.obj \
		ic_microstrip_model.obj \
		logo.obj \
		microstrip.obj \
		microstrip_model.obj \
		rc.obj \
		rl.obj \
		splash.obj \
		stripline.obj \
		stripline_model.obj

XPMS=	${TGIF_SRCS:.obj=.xpm}
PNGS=	${TGIF_SRCS:.obj=.png}
EPSS=	${TGIF_SRCS:.obj=.eps}

TGIF=		XENVIRONMENT=$(srcdir)/Xdefaults.tgif tgif
TGIF_FLAGS=	-print -color

default : ${PNGS} ${XPMS} ${EPSS} 

# All the new suffix rules

SUFFIXES = .obj .xpm .png .eps

.obj.eps :
	$(TGIF) $(TGIF_FLAGS) -eps $*.obj

.obj.png :
	$(TGIF) $(TGIF_FLAGS) -png $*.obj

.obj.xpm :
	$(TGIF) $(TGIF_FLAGS) -xpm $*.obj


