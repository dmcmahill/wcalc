## $Id: Makefile.am,v 1.18 2008/11/29 20:42:27 dan Exp $
##

## Copyright (C) 2001, 2002, 2005, 2006, 2008 Dan McMahill
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

pkgimgdatadir= @HTMLDIR@
pkgimgdata_DATA= ${PNGS}

pkgdatadir=	${datadir}/${PACKAGE}-${VERSION}
pkgdata_DATA=	${EPSS}

EPS2C  = ${AWK} -f $(top_srcdir)/utils/eps2c

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
GIFS=	${TGIF_SRCS:.obj=.gif}
PNGS=	${TGIF_SRCS:.obj=.png}
EPSS=	${TGIF_SRCS:.obj=.eps}
EPSC=	${TGIF_SRCS:.obj=.c}
OFILES=	${TGIF_SRCS:.obj=.o}

TGIF=		XENVIRONMENT=$(srcdir)/Xdefaults.tgif @TGIF@
TGIF_FLAGS=	-print -color

BUILT_SOURCES = ${PNGS} ${XPMS} ${EPSS} ${EPSC}
EXTRA_DIST=	Xdefaults.tgif ${TGIF_SRCS} ${BUILT_SOURCES}

# All the new suffix rules

SUFFIXES = .obj .xpm .xbm .gif .png .eps .c

.eps.c :
	$(EPS2C) fname=$*_eps $*.eps > $*.c
	${HOME}/src/pstoedit-3.41/src/pstoedit -dt -f "cairo: -header figure_$*.h -funcname figure_$*" $*.eps figure_$*.c

.obj.eps :
if MISSING_TGIF
	@echo "WARNING:  tgif was not found on your system but"
	@echo "          $*.eps is out of date and needs to be"
	@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@echo "          will be ignored."
else
	$(TGIF) $(TGIF_FLAGS) -eps $*.obj
endif

.obj.gif :
if MISSING_TGIF
	@echo "WARNING:  tgif was not found on your system but"
	@echo "          $*.gif is out of date and needs to be"
	@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@echo "          will be ignored."
else
	$(TGIF) $(TGIF_FLAGS) -gif $*.obj
endif

.obj.png :
if MISSING_TGIF
	@echo "WARNING:  tgif was not found on your system but"
	@echo "          $*.png is out of date and needs to be"
	@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@echo "          will be ignored."
else
	$(TGIF) $(TGIF_FLAGS) -png $*.obj
endif

.obj.xpm :
if MISSING_TGIF
	@echo "WARNING:  tgif was not found on your system but"
	@echo "          $*.xpm is out of date and needs to be"
	@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@echo "          will be ignored."
else
	$(TGIF) $(TGIF_FLAGS) -xpm $*.obj
endif


