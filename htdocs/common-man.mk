## $Id: common-man.mk,v 1.5 2005/10/21 03:48:24 dan Exp $

##
## Copyright (c) 2005 Dan McMahill
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

HTMLSUFFIX=	html

# path to find the .xml sources for the documentation
vpath %.xml $(top_srcdir)/sci-wcalc

# gets a list of the functions
include $(top_srcdir)/sci-wcalc/mex.mk

pkgimgdatadir= @HTMLDIR@/${MPATH}
pkgimgdata_DATA= ${SCIMANHTML}

# the include= is a ':' seperated search path for SSI directives
SHTML_INCLUDE_PATH= $(srcdir):$(top_srcdir)/htdocs

SCIMANSHTML=	${MEX_SRCS:.c=.shtml}
SCIMANHTML=		${MEX_SRCS:.c=.html}
SCIMANS=			${MEX_SRCS:.c=.man}
SCICATS=			${SCIMANS:.man=.cat}

all-local:  whatis.incl

MAINTAINERCLEANFILES=	${SCIMANHTML} ${SCIMANSHTML} whatis.incl ${BUILT_SOURCES}


BUILT_SOURCES= left_column.incl main_footer.incl
EXTRA_DIST= man_start.incl man_end.incl ${SCIMANHTML} ${BUILT_SOURCES}

CP_INCL= sed -e 's;a href=";a href="../;g' \
	-e 's;img src=";img src="../;g'

${SCIMANHTML}: left_column.incl main_footer.incl

@SOURCEFORGE_YES@SHTML2HTML_SF= sourceforge=1

left_column.incl: $(srcdir)/../left_column.incl
	$(CP_INCL) $< > $@

main_footer.incl: $(srcdir)/../main_footer.incl
	$(CP_INCL) $< > $@

whatis.incl: ${WC_XML} Makefile $(top_srcdir)/sci-wcalc/mex.mk $(top_srcdir)/sci-wcalc/whatis.xsl
	@echo "****************************************************"
	@echo "Generating the HTML whatis table"
	@echo "****************************************************"
	rm -f $@
	for f in ${WC_XML} ; do \
		${XSLTPROC} --stringparam suffix "${HTMLSUFFIX}" \
			--stringparam path "${MPATH}/" \
			$(top_srcdir)/sci-wcalc/whatis.xsl \
			$(top_srcdir)/sci-wcalc/$$f >> $@ ; \
	done

SUFFIXES=	.shtml .html .cat .xml

.xml.shtml :
	sed 's;@fname@;$*;g' $(srcdir)/man_start.incl > $@
	${XSLTPROC} --stringparam program "${TARGETPROGRAM}" \
		--stringparam suffix "${HTMLSUFFIX}" \
		--stringparam header "no" $(top_srcdir)/sci-wcalc/htmlpage.xsl $< >> $@
	cat $(srcdir)/man_end.incl >> $@

