## $Id: Makefile.am,v 1.25 2005/10/15 00:40:43 dan Exp $

##
## Copyright (c) 2005 Dan McMahill
## All rights reserved.
##
## This code is derived from software written by Dan McMahill
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. All advertising materials mentioning features or use of this software
##    must display the following acknowledgement:
##        This product includes software developed by Dan McMahill
##  4. The name of the author may not be used to endorse or promote products
##     derived from this software without specific prior written permission.
## 
##  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
##  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
##  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
##  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
##  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
##  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
##  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
##  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
##  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
##  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
##  SUCH DAMAGE.
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

