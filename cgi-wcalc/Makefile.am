## $Id: Makefile.am,v 1.17 2002/01/18 14:27:01 dan Exp $

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

cgibinexecdir = @CGIBINDIR@

cgibinexec_PROGRAMS = \
	air_coil.cgi \
	coax.cgi \
	coupled_microstrip.cgi \
	ic_microstrip.cgi \
	microstrip.cgi \
	stripline.cgi

noinst_PROGRAMS = \
	capture.cgi \
	cookie.cgi

# These are used by all programs:
COMMON = alert.c cgic.c cgic.h cgi-common.c cgi-common.h cgi-units.c cgi-units.h cookie.c cookie.h

air_coil_cgi_SOURCES = air_coil.cgi.c ${COMMON}

capture_cgi_SOURCES = capture.c ${COMMON}

coax_cgi_SOURCES = coax.cgi.c ${COMMON}

cookie_cgi_SOURCES = cookie.cgi.c ${COMMON}

coupled_microstrip_cgi_SOURCES = coupled_microstrip.cgi.c ${COMMON}

ic_microstrip_cgi_SOURCES = ic_microstrip.cgi.c ${COMMON}

microstrip_cgi_SOURCES = microstrip.cgi.c ${COMMON}

stripline_cgi_SOURCES = stripline.cgi.c ${COMMON}

INCLUDES =  -I$(top_srcdir)/include -I$(top_srcdir)/libwcalc -DHTMLPATH=\"@HTMLPATH@\"

LDADD = ../libwcalc/libwcalc.la
#LDFLAGS = -all-static

HTML_FILES= \
	air_coil.html \
	coax.html \
	coupled_microstrip.html \
	footer.html \
	ic_microstrip.html \
	microstrip.html \
	stripline.html 

ID_FILES=	${HTML_FILES:.html=_id.h}
CHTML_FILES=	${HTML_FILES:.html=_html.c}

BUILT_SOURCES = ${ID_FILES} ${CHTML_FILES}

EXTRA_DIST=	${BUILT_SOURCES} ${HTML_FILES} 

HTML2C  = ${AWK} -f $(top_srcdir)/utils/html2c out=cgiOut 
HTML2ID = $(AWK) '/\$$.*\$$/ {printf("const char id_string[]=\"%s\";\n",gensub(/.*(\$$.*\$$).*/,"\\1","g"));}'

SUFFIXES = .html 

%_html.c : %.html
	@AWK_GENSUB_YES@ $(HTML2C) $< > $@
	@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@AWK_GENSUB_NO@ echo "          .html.c files from the .html files.  If you need"
	@AWK_GENSUB_NO@ echo "          this functionality, you will need to install gawk."
	@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@AWK_GENSUB_NO@ echo "          particular awk program."


%_id.h : %.html
	$(HTML2ID) $< > $@
	@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@AWK_GENSUB_NO@ echo "          _id.h files from the .html files.  If you need"
	@AWK_GENSUB_NO@ echo "          this functionality, (which you should not),"
	@AWK_GENSUB_NO@ echo "          you will need to install gawk."
	@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@AWK_GENSUB_NO@ echo "          particular awk program."

