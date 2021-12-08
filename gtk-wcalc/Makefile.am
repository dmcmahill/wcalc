## -*- makefile -*-

##
## Copyright (C) 2001, 2002, 2004, 2005, 2006, 2008, 2009, 2017, 2020 Dan McMahill
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

#AUTOMAKE_OPTIONS = subdir-objects

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
	newprint.c \
	newprint.h \
	parallel_rc_gui.c \
	parallel_rc_gui.h \
	parallel_rl_gui.c \
	parallel_rl_gui.h \
	print.c \
	print.h \
	rods_gui.c \
	rods_gui.h \
	start.c \
	start.h \
	stripline_gui.c \
	stripline_gui.h \
	symbols.h \
	version.c \
	utils.c \
	utils.h \
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
	parallel_rc.wc \
	rods.wc \
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

AM_CPPFLAGS = @GTK_CFLAGS@ -DVER=\"@VERSION@\" \
	-DWCALC_DATADIR=\"$(pkgdatadir)\" \
	-I$(top_srcdir)/include @WCALC_CFLAGS@ -I$(top_srcdir)/intl \
	-I$(top_srcdir) ${DMALLOC_CFLAGS}

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS}

if WC_LOCAL_LIBWCALC
wcalc_LDADD = ../libwcalc/libwcalc.la
wcalc_DEPENDENCIES = ../libwcalc/libwcalc.la
else
wcalc_LDADD =
wcalc_DEPENDENCIES = 
endif

wcalc_LDFLAGS = @GTK_LIBS@ @WCALC_LIBS@

all-local: runwcalc

clean-local:
	-rm runwcalc

distclean-local:
	-rm AWG.asciitab AWG.c

if WIN32
wcalc_icon.${OBJEXT} : wcalc_icon.ico $(srcdir)/wcalc.rc
	$(WINDRES) $(srcdir)/wcalc.rc $@

wcalc_icon.ico: $(top_srcdir)/win32/wcalc_icon.ico
	cp $(top_srcdir)/win32/wcalc_icon.ico $@

wcalc_LDADD+=	wcalc_icon.${OBJEXT}
wcalc_DEPENDENCIES+=  wcalc_icon.${OBJEXT}

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

FIGSRCS=	\
	$(top_srcdir)/pixmaps/figure_air_coil.c \
	$(top_srcdir)/pixmaps/figure_air_coil.h \
	$(top_srcdir)/pixmaps/figure_bars_fig.c \
	$(top_srcdir)/pixmaps/figure_bars_fig.h \
	$(top_srcdir)/pixmaps/figure_coax.c \
	$(top_srcdir)/pixmaps/figure_coax.h \
	$(top_srcdir)/pixmaps/figure_coax_model.c \
	$(top_srcdir)/pixmaps/figure_coax_model.h \
	$(top_srcdir)/pixmaps/figure_coplanar.c \
	$(top_srcdir)/pixmaps/figure_coplanar.h \
	$(top_srcdir)/pixmaps/figure_coplanar_model.c \
	$(top_srcdir)/pixmaps/figure_coplanar_model.h \
	$(top_srcdir)/pixmaps/figure_coupled_microstrip.c \
	$(top_srcdir)/pixmaps/figure_coupled_microstrip.h \
	$(top_srcdir)/pixmaps/figure_coupled_stripline.c \
	$(top_srcdir)/pixmaps/figure_coupled_stripline.h \
	$(top_srcdir)/pixmaps/figure_ic_microstrip.c \
	$(top_srcdir)/pixmaps/figure_ic_microstrip.h \
	$(top_srcdir)/pixmaps/figure_ic_microstrip_model.c \
	$(top_srcdir)/pixmaps/figure_ic_microstrip_model.h \
	$(top_srcdir)/pixmaps/figure_microstrip.c \
	$(top_srcdir)/pixmaps/figure_microstrip.h \
	$(top_srcdir)/pixmaps/figure_microstrip_model.c \
	$(top_srcdir)/pixmaps/figure_microstrip_model.h \
	$(top_srcdir)/pixmaps/figure_rc.c \
	$(top_srcdir)/pixmaps/figure_rc.h \
	$(top_srcdir)/pixmaps/figure_rl.c \
	$(top_srcdir)/pixmaps/figure_rl.h \
	$(top_srcdir)/pixmaps/figure_rods_fig.c \
	$(top_srcdir)/pixmaps/figure_rods_fig.h \
	$(top_srcdir)/pixmaps/figure_stripline.c \
	$(top_srcdir)/pixmaps/figure_stripline.h \
	$(top_srcdir)/pixmaps/figure_stripline_model.c \
	$(top_srcdir)/pixmaps/figure_stripline_model.h

wcalc_SOURCES+=		${FIGSRCS}
wcalc_DEPENDENCIES+=	${FIGSRCS}

