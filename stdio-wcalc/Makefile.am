## -*- makefile -*-
## $Id: Makefile.am,v 1.17 2006/04/06 13:30:41 dan Exp $

##
## Copyright (c) 2004, 2005, 2006, 2008 Dan McMahill
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

