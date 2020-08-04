
##
## Copyright (C) 2001, 2002, 2004, 2006, 2008, 2009, 2017, 2020 Dan McMahill
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
	parallel_rc.cgi \
	parallel_rl.cgi \
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

parallel_rc_cgi_SOURCES = parallel_rc.cgi.c ${COMMON}

parallel_rl_cgi_SOURCES = parallel_rl.cgi.c ${COMMON}

stripline_cgi_SOURCES = stripline.cgi.c ${COMMON}

AM_CPPFLAGS =  -I$(top_srcdir)/include @WCALC_CFLAGS@ -DHTMLPATH=\"@HTMLPATH@\" ${DMALLOC_CFLAGS}

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
	parallel_rc.html \
	parallel_rl.html \
	stripline.html 

CHTML_FILES=	${HTML_FILES:.html=_html.c}

BUILT_SOURCES = ${CHTML_FILES}

EXTRA_DIST=	${BUILT_SOURCES} ${HTML_FILES} 

if SOURCEFORGE
HTML2C_SF= sourceforge=1
endif

HTML2C  = ${AWK} -f $(top_srcdir)/utils/html2c out=cgiOut ${HTML2C_SF}

SUFFIXES = .html 

%_html.c : %.html
if AWK_GENSUB
	$(HTML2C) $< > $@
else
	@echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@echo "          function.  This prevents the rebuilding of the"
	@echo "          .html.c files from the .html files.  If you need"
	@echo "          this functionality, you will need to install gawk."
	@echo "          By setting the variable AWK in your configure"
	@echo "          environment, you can force configure to find a"
	@echo "          particular awk program."
endif

