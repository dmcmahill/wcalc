## $Id: Makefile.am,v 1.19 2002/05/16 19:29:19 dan Exp $
##

## Copyright (c) 2001, 2002 Dan McMahill
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

# search for the .asciitab files (for the resistivity table, etc)
vpath %.asciitab ../gtk-wcalc:$(top_srcdir)/gtk-wcalc

SUBDIRS=	sci-man  mex-man octave-man

# Note, we let the user pick some arbitrary directory for the PNG files
# because we simply don't know how the user will organize their web server.
# For example, someone might install this in their personal public_html
# directory and need to look for images in ~randomuser/images, or perhaps
# this is installed by the webadmin under /wcalc.
pkgimgdatadir= @HTMLDIR@
pkgimgdata_DATA= ${PNGS} ${HTML_OUT}

# the include= is a ':' seperated search path for SSI directives
SHTML_INCLUDE_PATH= $(srcdir)

SHTML_IN=	\
	about.shtml \
	air_coil.shtml \
	AWG.shtml \
	cgi-wcalc.shtml \
	coax.shtml \
	coupled_microstrip.shtml \
	faq.shtml \
	gtk-wcalc.shtml \
	ic_microstrip.shtml \
	index.shtml \
	mex-wcalc.shtml \
	microstrip.shtml \
	obtaining.shtml \
	octave-wcalc.shtml \
	permeability.shtml \
	permitivity.shtml \
	projects.shtml \
	resistivity.shtml \
	sci-wcalc.shtml \
	screenshots.shtml \
	stripline.shtml

INCL_FILES=\
	disclaimer_properties.incl \
	left_column.incl \
	main_footer.incl \
	page_start.incl \
	${BUILT_INCL_FILES}

BUILT_INCL_FILES=\
	AWG.incl \
	permeability.incl \
	permitivity.incl \
	resistivity.incl

HTML_OUT=	${SHTML_IN:.shtml=.html}

PNGS=\
	screen1.png \
	screen2.png

clean-local:	
	-rm ${HTML_OUT}

distclean-local:
	-rm AWG.incl

BUILT_SOURCES=	${BUILT_INCL_FILES}
EXTRA_DIST=	${SHTML_IN} ${INCL_FILES} ${PNGS} shtml.mk

# add the dependency on the .incl files
${HTML_OUT} : ${INCL_FILES}

CGIPATH=	@CGIPATH@


FORMAT_TAB  = ${AWK} -f $(top_srcdir)/utils/format_tab type=html html_no_table=yes

SUFFIXES=	.asciitab

.asciitab.incl:
	${FORMAT_TAB} $< > $@

include $(srcdir)/shtml.mk
