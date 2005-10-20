## -*- makefile -*-
## $Id: Makefile.am,v 1.12 2005/10/20 00:37:09 dan Exp $
##

## Copyright (c) 2002, 2004, 2005 Dan McMahill
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

# to find the mex source files
vpath %.c $(top_srcdir)/sci-wcalc
vpath %.xml $(top_srcdir)/sci-wcalc

# list of all the mex files
include $(top_srcdir)/sci-wcalc/mex.mk

# the 'mex' command to compile the mex files
MEX=	@OCTMEX@

# the suffix for the compiled mex file
MEXSFX=	oct

octfiledir=	${libexecdir}/@MYSITEOCT@
fcnfiledir=	${libexecdir}/@MYSITEOCTM@
octfile_DATA=	${MEX_FILES}
fcnfile_DATA=	${MEX_MANS}

noinst_SCRIPTS=		runoctave

MEX_FILES=	${MEX_SRCS:.c=.${MEXSFX}}
MEX_MANS=	${MEX_SRCS:.c=.m}
MEX_TMP_CXX_FILES=	${addprefix mex_,${MEX_SRCS:.c=.cc}}
MEX_TMP_OBJ_FILES=	${MEX_TMP_CXX_FILES:.cc=.o}

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS} @WCALC_LIBS@

# various MEX flags
MEXLDFLAGS=	@WCALC_OCTLIBS@ ${DMALLOC_LIBS} ${EFENCE_LIBS}
MEXINCLUDES=	-I$(top_srcdir) -I$(top_srcdir)/include @WCALC_CFLAGS@


all-local:	${MEX_FILES} runoctave ${MEX_MANS}

clean-local:
	-rm alert.o ${MEX_FILES} runoctave ${MEX_MANS} ${MEX_TMP_CXX_FILES} ${MEX_TMP_OBJ_FILES}

runoctave:	$(srcdir)/runoctave.in
	sed "s;@BUILDDIR@;`pwd`;g" $(srcdir)/runoctave.in > $@
	chmod 755 $@


EXTRA_DIST=	runoctave.in copyright.m alert.c ${MEX_MANS}

# All the new suffix rules
SUFFIXES= .c .${MEXSFX} .m .cat .man .txt

.c.${MEXSFX} :
	$(MEX) $(MEXINCLUDES) $< $(srcdir)/alert.c $(MEXLDFLAGS)


TARGETPROGRAM=	octave
HTMLSUFFIX=		htm

include $(top_srcdir)/sci-wcalc/sciman.mk

