## $Id: Makefile.am,v 1.26 2008/11/29 20:41:30 dan Exp $

##
## Copyright (C) 2001, 2002, 2004, 2006, 2008, 2009 Dan McMahill
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

cgibinexecdir = @CGIBINDIR@

cgibinexec_PROGRAMS = \
	air_coil.cgi \
	bars.cgi \
	coax.cgi \
	coplanar.cgi \
	coupled_microstrip.cgi \
	coupled_stripline.cgi \
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

bars_cgi_SOURCES = bars.cgi.c ${COMMON}

coax_cgi_SOURCES = coax.cgi.c ${COMMON}

cookie_cgi_SOURCES = cookie.cgi.c ${COMMON}

coplanar_cgi_SOURCES = coplanar.cgi.c ${COMMON}

coupled_microstrip_cgi_SOURCES = coupled_microstrip.cgi.c ${COMMON}

coupled_stripline_cgi_SOURCES = coupled_stripline.cgi.c ${COMMON}

ic_microstrip_cgi_SOURCES = ic_microstrip.cgi.c ${COMMON}

microstrip_cgi_SOURCES = microstrip.cgi.c ${COMMON}

stripline_cgi_SOURCES = stripline.cgi.c ${COMMON}

INCLUDES =  -I$(top_srcdir)/include @WCALC_CFLAGS@ -DHTMLPATH=\"@HTMLPATH@\" ${DMALLOC_CFLAGS}

if WC_LOCAL_LIBWCALC
LDADD = ../libwcalc/libwcalc.la
else
LDADD = 
endif

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS} @WCALC_LIBS@ @CGI_STATIC@

HTML_FILES= \
	air_coil.html \
	bars.html \
	coax.html \
	coplanar.html \
	coupled_microstrip.html \
	coupled_stripline.html \
	footer.html \
	header.html \
	ic_microstrip.html \
	microstrip.html \
	stripline.html 

ID_FILES=	${HTML_FILES:.html=_id.h}
CHTML_FILES=	${HTML_FILES:.html=_html.c}

BUILT_SOURCES = ${ID_FILES} ${CHTML_FILES}

EXTRA_DIST=	${BUILT_SOURCES} ${HTML_FILES} 

@SOURCEFORGE_YES@HTML2C_SF= sourceforge=1

HTML2C  = ${AWK} -f $(top_srcdir)/utils/html2c out=cgiOut ${HTML2C_SF}
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

