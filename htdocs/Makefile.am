## $Id: Makefile.am,v 1.5 2001/10/20 12:29:49 dan Exp $
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

# Note, we let the user pick some arbitrary directory for the PNG files
# because we simply don't know how the user will organize their web server.
# For example, someone might install this in their personal public_html
# directory and need to look for images in ~randomuser/images, or perhaps
# this is installed by the webadmin under /wcalc.

pkgimgdatadir= @HTMLDIR@
pkgimgdata_DATA= ${PNGS} ${HTML_OUT}

# the include= is a ':' seperated search path for SSI directives
SHTML2HTML  = ${AWK} -f $(top_srcdir)/utils/shtml2html include=$(srcdir)

SHTML_IN=	\
	about.shtml \
	air_coil.shtml \
	cgi-wcalc.shtml \
	coupled_microstrip.shtml \
	gtk-wcalc.shtml \
	ic_microstrip.shtml \
	index.shtml \
	microstrip.shtml \
	obtaining.shtml \
	projects.shtml \
	sci-wcalc.shtml \
	screenshots.shtml \
	stripline.shtml

INCL_FILES=\
	left_column.incl \
	main_footer.incl \
	page_start.incl

HTML_OUT=	${SHTML_IN:.shtml=.html}

PNGS=\
	screen1.png \
	screen2.png

BUILT_SOURCES = ${HTML_OUT}
EXTRA_DIST=	${BUILT_SOURCES} ${SHTML_IN} ${INCL_FILES} ${PNGS}

# add the dependency on the .incl files
${HTML_OUT} : ${INCL_FILES}

# All the new suffix rules

SUFFIXES = .shtml .html

.shtml.html :
	@AWK_GENSUB_YES@$(SHTML2HTML) $*.shtml > $*.html
	@@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@@AWK_GENSUB_NO@ echo "          .html files from the .shtml files.  If you need"
	@@AWK_GENSUB_NO@ echo "          this functionality, you will need to install gawk."
	@@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@@AWK_GENSUB_NO@ echo "          particular awk program."

