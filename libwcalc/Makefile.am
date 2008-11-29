## -*- makefile -*-
## $Id: Makefile.am,v 1.31 2008/09/05 11:50:53 dan Exp $

##
## Copyright (C) 2001, 2002, 2004, 2004, 2005, 2006 Dan McMahill
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

lib_LTLIBRARIES = libwcalc.la

bin_SCRIPTS = wcalc-config
pkgincludedir=	$(includedir)/${PACKAGE}-${VERSION}
pkginclude_DATA =  \
	air_coil.h \
	air_coil_loadsave.h \
	bars.h \
	bars_loadsave.h \
	coax.h \
	coax_loadsave.h \
	coplanar.h \
	coplanar_loadsave.h \
	coupled_microstrip.h \
	coupled_microstrip_loadsave.h \
	coupled_stripline.h \
	coupled_stripline_loadsave.h \
	ic_microstrip.h \
	ic_microstrip_loadsave.h \
	libwcalc.h \
	mathutil.h \
	microstrip.h \
	microstrip_loadsave.h \
	misc.h \
	physconst.h \
	stripline.h \
	stripline_loadsave.h \
	units.h \
	wcalc_loadsave.h

pkgconfigdir            = $(libdir)/pkgconfig
pkgconfig_DATA          = wcalc.pc

noinst_PROGRAMS = bars_test coax_test coplanar_test coupled_microstrip_test \
	ic_microstrip_test mathutil_test \
	make_defaults microstrip_test units_test

coax_test_SOURCES = alert.c coax_test.c
bars_test_SOURCES = alert.c bars_test.c
coplanar_test_SOURCES = alert.c coplanar_test.c
coupled_microstrip_test_SOURCES = alert.c coupled_microstrip_test.c 
ic_microstrip_test_SOURCES = alert.c ic_microstrip_test.c 
mathutil_test_SOURCES = alert.c mathutil_test.c 
microstrip_test_SOURCES = alert.c microstrip_test.c 
units_test_SOURCES = alert.c units_test.c
make_defaults_SOURCES = alert.c make_defaults.c

EXTRA_DIST=	coupled_microstrip.data wcalc-config.in wcalc.pc.in libwcalc.h

BUILT_SOURCES=	defaults.c

@MAINTAINER_MODE_TRUE@defaults.c: make_defaults
@MAINTAINER_MODE_TRUE@	./make_defaults > $@

LDADD = libwcalc.la

AM_LDFLAGS=	${LIBWALC_LDFLAGS} ${DMALLOC_LIBS} ${EFENCE_LIBS}

# libwcalc shared library version.  A fairly strict rule should
# be followed for when this version is updated.  Those rules,
# taken from the libtool manual, are as follows
#
# - Format is current:revision:age
#
# 1. Start with version information of `0:0:0' for each libtool library.
#
# 2. Update the version information only immediately before a public
#    release of your software. More frequent updates are unnecessary,
#    and only guarantee that the current interface number gets larger faster.
#
# 3. If the library source code has changed at all since the last update,
#    then increment revision (`c:r:a' becomes `c:r+1:a').
#
# 4. If any interfaces have been added, removed, or changed since the last
#    update, increment current, and set revision to 0.
#
# 5. If any interfaces have been added since the last public release, then
#    increment age.
#
# 6. If any interfaces have been removed since the last public release, then
#    set age to 0.
#
#libwcalc_la_LDFLAGS = -version-info 0:1:0

libwcalc_la_SOURCES = \
	air_coil.c \
	air_coil.h \
	air_coil_loadsave.c \
	air_coil_loadsave.h \
	bars.c \
	bars.h \
	bars_loadsave.c \
	bars_loadsave.h \
	coax.c \
	coax.h \
	coax_loadsave.c \
	coax_loadsave.h \
	coplanar.c \
	coplanar.h \
	coplanar_loadsave.c \
	coplanar_loadsave.h \
	coupled_microstrip.c \
	coupled_microstrip.h \
	coupled_microstrip_loadsave.c \
	coupled_microstrip_loadsave.h \
	coupled_stripline.c \
	coupled_stripline.h \
	coupled_stripline_loadsave.c \
	coupled_stripline_loadsave.h \
	defaults.c \
	defaults.h \
	ic_microstrip.c \
	ic_microstrip.h \
	ic_microstrip_loadsave.c \
	ic_microstrip_loadsave.h \
	mathutil.c \
	mathutil.h \
	microstrip.c \
	microstrip.h \
	microstrip_loadsave.c \
	microstrip_loadsave.h \
	misc.c \
	misc.h \
	physconst.h \
	stripline.c \
	stripline.h \
	stripline_loadsave.c \
	stripline_loadsave.h \
	units.c \
	units.h \
	wcalc_loadsave.c \
	wcalc_loadsave.h

INCLUDES = -I$(top_srcdir)/include -DVER=\"@VERSION@\" ${DMALLOC_CFLAGS}

