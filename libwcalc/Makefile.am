## -*- makefile -*-
## $Id: Makefile.am,v 1.29 2008/07/02 15:03:54 dan Exp $

##
## Copyright (c) 2001, 2002, 2004, 2004, 2005, 2006 Dan McMahill
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

