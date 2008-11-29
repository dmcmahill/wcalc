## -*- makefile -*-
## $Id: Makefile.am,v 1.38 2008/09/05 11:50:53 dan Exp $

##
## Copyright (C) 2001, 2002, 2004, 2005, 2006, 2008 Dan McMahill
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

bin_PROGRAMS = wcalc

wcalc_SOURCES = \
	about.c \
	about.h \
	alert.c \
	air_coil_gui.c \
	air_coil_gui.h \
	bars_gui.c \
	bars_gui.h \
	coax_gui.c \
	coax_gui.h \
	coplanar_gui.c \
	coplanar_gui.h \
	coupled_microstrip_gui.c \
	coupled_microstrip_gui.h \
	coupled_stripline_gui.c \
	coupled_stripline_gui.h \
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
	bars.wc \
	coax.wc \
	coupled_microstrip.wc \
	coupled_stripline.wc \
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

if WC_LOCAL_LIBWCALC
wcalc_LDADD = ../libwcalc/libwcalc.la
else
wcalc_LDADD =
endif

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

