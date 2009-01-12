## $Id: Makefile.am,v 1.25 2008/11/29 20:42:25 dan Exp $
##

## Copyright (C) 2001, 2002, 2004, 2005 Dan McMahill
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


EXTRA_DIST=	runmatlab.in copyright.m mexlink ${MEX_MANS} alert.c matlab_startup.m.in

matlab_startup.m: $(srcdir)/matlab_startup.m.in
	sed \
		-e 's;@matlabdir@;${pkglibexecdir};g' \
		-e 's;@pkgname@;${PACKAGE}-${VERSION};g' \
		$(srcdir)/matlab_startup.m.in > matlab_startup.m

# All the new suffix rules
SUFFIXES= .c .${MEXSFX}.la .m .txt

# matlab mex is not parallel safe.  This is partly because I have listed
# alert.c on the build command line so each mex job compiles alert.c  But
# even without this issue, mex compiles mexversion.c from the matlab install
# tree as well and we have no control over that.  We could potentially
# arrange to use a temp directory for each mex file, but that sounds ugly
# so for now, just prevent parallel makes.
.NOTPARALLEL :
.c.${MEXSFX}.la :
	$(MEX) -v $(MEXINCLUDES) $< $(srcdir)/alert.c $(MEXLDFLAGS)

TARGETPROGRAM=	matlab
HTMLSUFFIX=		htm

# for the online help generation:
include $(top_srcdir)/sci-wcalc/sciman.mk

