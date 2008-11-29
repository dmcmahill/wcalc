## $Id: Makefile.am,v 1.1 2005/11/05 01:53:57 dan Exp $

##
## Copyright (C) 2005 Dan McMahill
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

EXTRA_DIST= build_wcalc wcalc.nsi.in Readme.txt \
	${TGIF_OBJS} ${XPMS} ${PPMS_16} ${PPMS_256} ${ICONS}

TGIF_OBJS=	wcalc_icon_big.obj wcalc_icon_med.obj wcalc_icon_sml.obj
XPMS=		${TGIF_OBJS:.obj=.xpm}
PPMS_16=	${TGIF_OBJS:.obj=_16.ppm}
PPMS_256=	${TGIF_OBJS:.obj=_256.ppm}
ICONS=		wcalc_icon.ico

TGIF_PROG=	XENVIRONMENT=$(top_srcdir)/pixmaps/Xdefaults.tgif @TGIF@ -print -color -xpm

pkgdatadir=     ${datadir}/${PACKAGE}-${VERSION}

if WIN32
pkgdata_DATA=	Readme.txt
endif


SUFFIXES=	.xpm .obj _16.ppm _256.ppm .ico

%.xpm : %.obj
if MISSING_TGIF
	@echo "Tgif is missing on your system so I will not rebuild $@"
	@echo "even though $< seems newer.  You do not need this unless"
	@echo "you are building for win32 anyway.  And even without it"
	@echo "it just means your icon modifications will not take."
else
	${TGIF_PROG} $<
endif

%_16.ppm : %.xpm
if MISSING_CONVERT
	@echo "convert (from ImageMagick) is missing on your system so I will not rebuild $@"
	@echo "even though $< seems newer.  You do not need this unless"
	@echo "you are building for win32 anyway.  And even without it"
	@echo "it just means your icon modifications will not take."
else
	${CONVERT} -colors 16 $< $@
endif

%_256.ppm : %.xpm
if MISSING_CONVERT
	@echo "convert (from ImageMagick) is missing on your system so I will not rebuild $@"
	@echo "even though $< seems newer.  You do not need this unless"
	@echo "you are building for win32 anyway.  And even without it"
	@echo "it just means your icon modifications will not take."
else
	${CONVERT} -colors 256 $< $@
endif

wcalc_icon.ico : ${PPMS_16} ${PPMS_256}
if MISSING_XPMTOPPM
	@echo "xpmtoppm/ppmtowinicon are missing on your system so I will not rebuild $@"
	@echo "even though $< seems newer.  You do not need this unless"
	@echo "you are building for win32 anyway.  And even without it"
	@echo "it just means your icon modifications will not take."
else
	${PPMTOWINICON} -output $@ ${PPMS_16} ${PPMS_256}
endif

