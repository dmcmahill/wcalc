## -*- makefile -*-
## $Id: Makefile.am,v 1.18 2008/09/05 11:50:55 dan Exp $

##
## Copyright (C) 2004, 2005, 2006, 2008 Dan McMahill
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

## to find the .xml docs
vpath %.xml $(top_srcdir)/sci-wcalc

# list of all the mex files (needed for man pages)
include $(top_srcdir)/sci-wcalc/mex.mk


bin_PROGRAMS = stdio-wcalc

stdio_wcalc_SOURCES =  alert.c main.c 

man1_MANS=	stdio-wcalc.1
mann_MANS=  ${FNMAN}

MANSEC=		n
FNMAN=		${MEX_MANS:.man=.${MANSEC}}

MEX_MANS=	${MEX_SRCS:.c=.man}

# we always want to install manpages.  man good, info bad!
install-data-local: install-man

RUN_TESTS=	run_tests.sh
check_SCRIPTS=	${RUN_TESTS}

TESTS= ${RUN_TESTS}

DEFS= -DLOCALEDIR="\"$(datadir)/locale\"" @DEFS@

LIBS= @LIBINTL@ @LIBS@ @WCALC_LIBS@

INCLUDES =  -DVER=\"@VERSION@\" \
	-I$(top_srcdir)/include @WCALC_CFLAGS@ -I$(top_srcdir)/intl -I../intl \
	-I$(top_srcdir) ${DMALLOC_CFLAGS}

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS}

if WC_LOCAL_LIBWCALC
stdio_wcalc_LDADD = ../libwcalc/libwcalc.la
else
stdio_wcalc_LDADD =
endif

TEST_INPUTS= \
	air_coil.dat \
	coax.dat \
	coplanar.dat \
	coupled_microstrip.dat \
	coupled_stripline.dat \
	ic_microstrip.dat \
	microstrip.dat \
	stripline.dat

REF_FILES=	${TEST_INPUTS:.dat=.ref}
DIA_FILES=	${TEST_INPUTS:.dat=.dia}

EXTRA_DIST= ${man1_MANS} ${FNMAN} ${TEST_INPUTS} ${RUN_TESTS} ${REF_FILES} tests.list

DISTCLEANFILES= ${DIA_FILES} 


##
## Pull in stuff for the man pages
##

SUFFIXES=	.${MANSEC} .man

.man.${MANSEC}:
	${AWK} '/^\.NF/,/^\.FI/ {gsub(/\\/, "\\\\"); } {print}' $< > $@

TARGETPROGRAM=  stdio

include $(top_srcdir)/sci-wcalc/sciman.mk

