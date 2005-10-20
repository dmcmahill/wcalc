## $Id: Makefile.am,v 1.13 2005/10/20 02:30:43 dan Exp $

##
## Copyright (c) 2001, 2002, 2005 Dan McMahill
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

vpath %.xml $(top_srcdir)/sci-wcalc

include $(top_srcdir)/sci-wcalc/mex.mk

pkgimgdatadir= @HTMLDIR@/sci-man
pkgimgdata_DATA= ${SCIMANHTML}

# the include= is a ':' seperated search path for SSI directives
SHTML_INCLUDE_PATH= $(srcdir):$(top_srcdir)/htdocs

SCIMANSHTML=	${MEX_SRCS:.c=.shtml}
SCIMANHTML=	${MEX_SRCS:.c=.html}
SCIMANS=	${MEX_SRCS:.c=.man}
SCICATS=	${SCIMANS:.man=.cat}

all-local:	whatis.incl

clean-local:	
	-rm ${SCIMANHTML} ${SCIMANSHTML} whatis.incl ${BUILT_SOURCES}


BUILT_SOURCES= left_column.incl main_footer.incl
EXTRA_DIST=	man_start.incl man_end.incl

CP_INCL=	sed  -e 's;a href=";a href="../;g' \
		     -e 's;img src=";img src="../;g'

@SOURCEFORGE_YES@SHTML2HTML_SF= sourceforge=1 

left_column.incl: $(srcdir)/../left_column.incl
	$(CP_INCL) $< > $@

main_footer.incl: $(srcdir)/../main_footer.incl
	$(CP_INCL) $< > $@

include $(top_srcdir)/htdocs/common-man.mk

TARGETPROGRAM=	scilab
MPATH=		sci-man
include $(top_srcdir)/sci-wcalc/sciman.mk
include $(top_srcdir)/htdocs/shtml.mk
