## -*- makefile -*-
## $Id: Makefile.am,v 1.34 2005/11/05 01:53:56 dan Exp $

##
## Copyright (c) 2001, 2002, 2004, 2005, 2006 Dan McMahill
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

bin_PROGRAMS = wcalc

wcalc_SOURCES = \
	about.c \
	about.h \
	alert.c \
	air_coil_gui.c \
	air_coil_gui.h \
	coax_gui.c \
	coax_gui.h \
	coplanar_gui.c \
	coplanar_gui.h \
	coupled_microstrip_gui.c \
	coupled_microstrip_gui.h \
	epscat.c \
	epscat.h \
	files.c \
	files.h \
	gtk-units.c \
	gtk-units.h \
	ic_microstrip_gui.c \
	ic_microstrip_gui.h \
	menus.c \
	menus.h \
	microstrip_gui.c \
	microstrip_gui.h \
	print.c \
	print.h \
	start.c \
	start.h \
	stripline_gui.c \
	stripline_gui.h \
	version.c \
	wcalc.c \
	wcalc.h \
	${GTKTABS}

man_MANS=	wcalc.1

# always install the man page!
install-data-local:	install-man

pkgdatadir=	${datadir}/${PACKAGE}-${VERSION}
pkgdata_DATA= \
	air_coil.wc \
	coax.wc \
	coupled_microstrip.wc \
	ic_microstrip.wc \
	microstrip.wc \
	stripline.wc

ASCIITABS=\
	AWG.asciitab \
	permeability.asciitab \
	permitivity.asciitab \
	resistivity.asciitab \
	units.asciitab

GTKTABS=	${ASCIITABS:.asciitab=.c}

BUILT_SOURCES=	${GTKTABS} AWG.asciitab

FORMAT_TAB  = ${AWK} -f $(top_srcdir)/utils/format_tab type=gtk

EXTRA_DIST=	icon_bitmap runwcalc.in ${man_MANS} ${ASCIITABS} ${pkgdata_DATA} wcalc.rc

DEFS= -DLOCALEDIR="\"$(datadir)/locale\"" -DWCALC_RCDIR="\"$(pkgdatadir)\"" @DEFS@

LIBS= @LIBINTL@ @LIBS@

INCLUDES = @GTK_CFLAGS@ -DVER=\"@VERSION@\" \
	-DWCALC_DATADIR=\"$(pkgdatadir)\" \
	-I$(top_srcdir)/include @WCALC_CFLAGS@ -I$(top_srcdir)/intl \
	-I$(top_srcdir) ${DMALLOC_CFLAGS}

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS}

wcalc_LDADD = @WCALC_LDADD@
wcalc_LDFLAGS = @GTK_LIBS@ @WCALC_LIBS@

all-local: runwcalc

clean-local:
	-rm runwcalc

distclean-local:
	-rm AWG.asciitab AWG.c

if WIN32
wcalc_icon.o : wcalc_icon.ico $(srcdir)/wcalc.rc
	windres $(srcdir)/wcalc.rc $@

wcalc_icon.ico: $(top_srcdir)/win32/wcalc_icon.ico
	cp $(top_srcdir)/win32/wcalc_icon.ico $@

wcalc_LDADD+=	wcalc_icon.o

endif

runwcalc:	$(srcdir)/runwcalc.in
	sed	-e "s;@BUILDDIR@;`pwd`;g" \
		-e 's;@TOPSRCDIR@;${top_srcdir};g' $(srcdir)/runwcalc.in > $@
	chmod 755 $@

AWG.c: AWG.asciitab

AWG.asciitab: $(srcdir)/Makefile.am
	${AWK}  'BEGIN{ \
				printf("Wire Size (AWG)|Diameter (mil)|Diameter (mm)\n\n"); \
				for(awg=8;awg<=42;awg++){ \
					dia = 0.005 / ((92.0^((awg - 36.0)/39.0)) ); \
					printf("%d\t|%10.2f\t|%6.3f\n",awg,1e3*dia,25.4*dia);\
					}\
				}' /dev/null > $@

SUFFIXES= .asciitab

%.c : %.asciitab $(top_srcdir)/utils/format_tab
	$(FORMAT_TAB) gtk_fname=`basename $*_table` $< > $@

