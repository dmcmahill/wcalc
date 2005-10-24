## $Id: Makefile.am,v 1.22 2005/10/24 16:12:05 dan Exp $
##

## Copyright (c) 2001, 2002, 2004, 2005 Dan McMahill
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

# to find the documentation source files
vpath %.xml $(top_srcdir)/sci-wcalc

# list of all the mex files
include $(top_srcdir)/sci-wcalc/mex.mk

# the 'mex' command to compile the mex files
MEX=	@MEX@

# the suffix for the compiled mex file
MEXSFX=	@MEXSFX@

# base directory for matlab installation
MATLAB_DIR=	@MATLAB_DIR@

pkglibexecdir=	${libexecdir}/matlab/${PACKAGE}-${VERSION}
pkglibexec_DATA=	${MEX_MANS}

noinst_SCRIPTS=		runmatlab

INCLUDES = -I.. -I$(top_srcdir)/include -I${MATLAB_DIR}/extern/include ${DMALLOC_CFLAGS}

MEX_FILES=		${MEX_SRCS:.c=.${MEXSFX}}
MEX_LA_FILES=	${MEX_FILES:=.la}
MEX_MANS=		${MEX_SRCS:.c=.m}

# various MEX flags
#MEXLDFLAGS=	@WCALC_MEXLIBS@ ${DMALLOC_LIBS} ${EFENCE_LIBS}
MEXLDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS}
MEXINCLUDES=	-I.. -I$(top_srcdir)/include @WCALC_CFLAGS@

all-local:	${MEX_LA_FILES} runmatlab ${MEX_MANS} matlab_startup.m

install-data-hook:
	for f in ${MEX_LA_FILES} ; do \
		${LIBTOOL} --mode=install ${INSTALL_DATA} $$f ${pkglibexecdir} ; \
	done

uninstall-hook:
	for f in ${MEX_LA_FILES} ; do \
		${LIBTOOL} --mode=uninstall ${RM} ${pkglibexecdir}/$$f ; \
	done


CLEANFILES=	${MEX_FILES} ${MEX_LA_FILES} runmatlab matlab_startup.m
MAINTAINERCLEANFILES=	${MEX_MANS}

runmatlab:	$(srcdir)/runmatlab.in
	sed "s;@BUILDDIR@;`pwd`;g" $(srcdir)/runmatlab.in > $@
	chmod 755 $@


EXTRA_DIST=	runmatlab.in copyright.m mexlink ${MEX_MANS} alert.c

matlab_startup.m: $(srcdir)/matlab_startup.m.in
	sed \
		-e 's;@matlabdir@;${pkglibexecdir};g' \
		-e 's;@pkgname@;${PACKAGE}-${VERSION};g' \
		$(srcdir)/matlab_startup.m.in > matlab_startup.m

# All the new suffix rules
SUFFIXES= .c .${MEXSFX}.la .m .txt

.c.${MEXSFX}.la :
	$(MEX) -v $(MEXINCLUDES) $< $(srcdir)/alert.c $(MEXLDFLAGS)

TARGETPROGRAM=	matlab
HTMLSUFFIX=		htm

# for the online help generation:
include $(top_srcdir)/sci-wcalc/sciman.mk

