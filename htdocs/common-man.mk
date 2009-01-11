## $Id: common-man.mk,v 1.9 2009/01/11 15:22:44 dan Exp $

##
## Copyright (c) 2005 Dan McMahill
## All rights reserved.
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

MAINTAINERCLEANFILES=	${SCIMANHTML} ${SCIMANSHTML} ${BUILT_SOURCES}

# if we don't have xsltproc then we don't want to remove the provided
# whatis.incl.   If we have xsltproc, then go ahead.
if MISSING_XSLT
else
CLEANFILES=		whatis.incl
endif

BUILT_SOURCES= left_column.incl main_footer.incl
EXTRA_DIST= man_start.incl man_end.incl ${SCIMANHTML} ${SCIMANSHTML} ${BUILT_SOURCES} whatis.incl

CP_INCL= sed -e 's;a href=";a href="../;g' \
	-e 's;img src=";img src="../;g'

${SCIMANHTML}: left_column.incl main_footer.incl whatis.incl

if SOURCEFORGE
SHTML2HTML_SF= sourceforge=1
endif

left_column.incl: $(srcdir)/../left_column.incl
	$(CP_INCL) $< > $@

main_footer.incl: $(srcdir)/../main_footer.incl
	$(CP_INCL) $< > $@

whatis.incl: ${WC_XML} $(srcdir)/Makefile.am $(top_srcdir)/sci-wcalc/mex.mk $(top_srcdir)/sci-wcalc/whatis.xsl
if MISSING_XSLT
	@echo "****************************************************"
	@echo "WARNING:  xsltproc and/or w3m were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to the sources for "
	@echo "          $@ will be ignored"
	@echo "****************************************************"
else
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
endif

SUFFIXES=	.shtml .html .cat .xml

.xml.shtml :
if MISSING_XSLT
	@echo "****************************************************"
	@echo "WARNING:  xsltproc and/or w3m were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to $<"
	@echo "          will be ignored"
	@echo "****************************************************"
else
	sed 's;@fname@;$*;g' $(srcdir)/man_start.incl > $@
	${XSLTPROC} --stringparam program "${TARGETPROGRAM}" \
		--stringparam suffix "${HTMLSUFFIX}" \
		--stringparam header "no" $(top_srcdir)/sci-wcalc/htmlpage.xsl $< >> $@
	cat $(srcdir)/man_end.incl >> $@
endif

