## $Id: Makefile.am,v 1.10 2001/12/13 05:10:54 dan Exp $
##

## Copyright (c) 2001, 2002 Dan McMahill
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

# Note, we let the user pick some arbitrary directory for the PNG files
# because we simply don't know how the user will organize their web server.
# For example, someone might install this in their personal public_html
# directory and need to look for images in ~randomuser/images, or perhaps
# this is installed by the webadmin under /wcalc.

pkgimgdatadir= @HTMLDIR@
pkgimgdata_DATA= ${PNGS}

pkgdata_DATA=	${EPSS}


EPS2C  = ${AWK} -f $(top_srcdir)/utils/eps2c

TGIF_SRCS=	\
		air_coil.obj \
		alert_fig.obj \
		coax.obj \
		coax_model.obj \
		coupled_microstrip.obj \
		ic_microstrip.obj \
		ic_microstrip_model.obj \
		logo.obj \
		microstrip.obj \
		microstrip_model.obj \
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

.obj.eps :
	@TGIF_YES@$(TGIF) $(TGIF_FLAGS) -eps $*.obj
	@TGIF_NO@echo "WARNING:  tgif was not found on your system but"
	@TGIF_NO@echo "          $*.eps is out of date and needs to be"
	@TGIF_NO@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@TGIF_NO@echo "          will be ignored."

.obj.gif :
	@TGIF_YES@$(TGIF) $(TGIF_FLAGS) -gif $*.obj
	@TGIF_NO@echo "WARNING:  tgif was not found on your system but"
	@TGIF_NO@echo "          $*.gif is out of date and needs to be"
	@TGIF_NO@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@TGIF_NO@echo "          will be ignored."

.obj.png :
	@TGIF_YES@$(TGIF) $(TGIF_FLAGS) -png $*.obj
	@TGIF_NO@echo "WARNING:  tgif was not found on your system but"
	@TGIF_NO@echo "          $*.png is out of date and needs to be"
	@TGIF_NO@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@TGIF_NO@echo "          will be ignored."

.obj.xpm :
	@TGIF_YES@$(TGIF) $(TGIF_FLAGS) -xpm $*.obj
	@TGIF_NO@echo "WARNING:  tgif was not found on your system but"
	@TGIF_NO@echo "          $*.xpm is out of date and needs to be"
	@TGIF_NO@echo "          rebuilt from $*.obj.  Changes to $*.obj"
	@TGIF_NO@echo "          will be ignored."

