## $Id: Makefile.am,v 1.8 2001/10/14 03:30:28 dan Exp $

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

pkgdata_DATA = \
	copying.html \
	index.html

cgibinexecdir = $(libexecdir)/cgi-bin

cgibinexec_PROGRAMS = \
	air_coil.cgi \
	coupled_microstrip.cgi \
	ic_microstrip.cgi \
	microstrip.cgi \
	stripline.cgi

# These are used by all programs:
COMMON = alert.c cgic.c cgic.h

air_coil_cgi_SOURCES = air_coil.cgi.c ${COMMON}

coupled_microstrip_cgi_SOURCES = coupled_microstrip.cgi.c ${COMMON}

ic_microstrip_cgi_SOURCES = ic_microstrip.cgi.c ${COMMON}

microstrip_cgi_SOURCES = microstrip.cgi.c ${COMMON}

stripline_cgi_SOURCES = stripline.cgi.c ${COMMON}

INCLUDES =  -I$(top_srcdir)/include -I$(top_srcdir)/libwcalc -DHTMLPATH=\"@HTMLPATH@\"

LDADD = ../libwcalc/libwcalc.la

BUILT_SOURCES = \
	air_coil_html.c \
	air_coil_id.h \
	coupled_microstrip_html.c \
	coupled_microstrip_id.h \
	footer_html.c \
	ic_microstrip_html.c \
	ic_microstrip_id.h \
	microstrip_html.c \
	microstrip_id.h \
	stripline_html.c \
	stripline_id.h 

EXTRA_DIST=	${BUILT_SOURCES}

HTML2C  = ${AWK} -f $(top_srcdir)/utils/html2c out=cgiOut 
HTML2ID = $(AWK) '/\$$.*\$$/ {printf("const char id_string[]=\"%s\";\n",gensub(/.*(\$$.*\$$).*/,"\\1","g"));}'

SUFFIXES = .html 

%_html.c : %.html
	@AWK_GENSUB_YES@ $(HTML2C) $(srcdir)/$< > $(srcdir)/$@
	@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@AWK_GENSUB_NO@ echo "          .html.c files from the .html files.  If you need"
	@AWK_GENSUB_NO@ echo "          this functionality, you will need to install gawk."
	@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@AWK_GENSUB_NO@ echo "          particular awk program."


%_id.h : %.html
	$(HTML2ID) $(srcdir)/$< > $(srcdir)/$@
	@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@AWK_GENSUB_NO@ echo "          _id.h files from the .html files.  If you need"
	@AWK_GENSUB_NO@ echo "          this functionality, (which you should not),"
	@AWK_GENSUB_NO@ echo "          you will need to install gawk."
	@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@AWK_GENSUB_NO@ echo "          particular awk program."

