## $Id: Makefile.am,v 1.3 2001/10/17 00:54:07 dan Exp $

##
## Copyright (c) 2001 Dan McMahill
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
	epscat.c \
	epscat.h \
	files.c \
	files.h \
	menus.c \
	menus.h \
	microstrip_gui.c \
	microstrip_gui.h \
	print.c \
	print.h \
	start.c \
	start.h \
	version.c \
	wcalc.c \
	wcalc.h

EXTRA_DIST=	icon_bitmap

INCLUDES = @GTK_CFLAGS@ -DVER=\"@VERSION@\" -DWCALC_DATADIR=\"$(pkgdatadir)\" -I$(top_srcdir)/include -I$(top_srcdir)/libwcalc -I$(top_srcdir)

wcalc_LDADD = ../libwcalc/libwcalc.la
wcalc_LDFLAGS = @GTK_LIBS@  

man_MANS=	wcalc.1

TESTS= wcalc
TESTS_ENVIRONMENT=	"FOO=bar"