## $Id$

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

libexec_PROGRAMS = \
	air_coil.cgi \
	coupled_microstrip.cgi \
	ic_microstrip.cgi \
	microstrip.cgi \
	stripline.cgi

# These are used by all programs:
COMMON = alert.c cgic.c

air_coil_cgi_SOURCES = air_coil.cgi.c ${COMMON}

coupled_microstrip_cgi_SOURCES = coupled_microstrip.cgi.c ${COMMON}

ic_microstrip_cgi_SOURCES = ic_microstrip.cgi.c ${COMMON}

microstrip_cgi_SOURCES = microstrip.cgi.c ${COMMON}

stripline_cgi_SOURCES = stripline.cgi.c ${COMMON}

INCLUDES =  -I$(top_srcdir)/include -I$(top_srcdir)/libwcalc

LDADD = ../libwcalc/libwcalc.la

BUILT_SOURCES = \
	air_coil_html.c \
	coupled_microstrip_html.c \
	footer_html.c \
	ic_microstrip_html.c \
	microstrip_html.c \
	stripline_html.c

HTML2C  = ${AWK} -f $(top_srcdir)/utils/html2c out=cgiOut 
HTML2ID = $(AWK) '/\$$.*\$$/ {printf("const char id_string[]=\"%s\";\n",gensub(/.*(\$$.*\$$).*/,"\\1","g"));}'

air_coil_html.c:        air_coil.html
	$(HTML2C) $(srcdir)/air_coil.html > $(srcdir)/air_coil_html.c

footer_html.c:	footer.html
	$(HTML2C) $(srcdir)/footer.html > $(srcdir)/footer_html.c

